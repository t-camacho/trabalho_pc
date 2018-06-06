///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2005, 2006, 2007, 2008, 2009
// University of Rochester
// Department of Computer Science
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the University of Rochester nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <iostream>
#include <stdio.h>

#include "DList.hpp"
#include "primitives.hpp"

using namespace bench;

key_t list_key = 567890;

DList::DNode* DList::shmalloc(key_t key) {
	DNode* node = NULL;
	int shmid = shmget(key, sizeof(DNode), IPC_CREAT | 0666);
	if (shmid != -1) {
		node = (DNode*) shmat(shmid, NULL, 0);
	}
	return node;
}

DList::DNode* DList::get_node(key_t key) {
	DNode* node = NULL;
	int shmid = shmget(key, sizeof(DNode), 0666);
	if (shmid != -1) {
		node = (DNode*) shmat(shmid, NULL, 0);
	}
	return node;
}

void DList::shfree(DNode* node) {
	shmdt(node);
}

void DList::refresh() {
	DNode* node = NULL;
	int next = head;
	int shmid = -1;
	
	do {
		shmid = shmget(next, sizeof(DNode), 0666);
		if (shmid == -1) {
			std::cerr << "Error: could not get memory id in refresh" << std::endl;
			exit(-1);
		}
		node = (DNode*) shmat(shmid, NULL, 0);
		next = node->next;
		shmdt(node);
		shmctl(shmid, IPC_RMID, NULL);
	} while(next != -1);
}

// constructor: head and tail have extreme values, point to each other
DList::DList(int max) {
	head = list_key++;
	tail = list_key++;
	
	DNode *head_node, *tail_node;
	if ((head_node = shmalloc(head)) == NULL) {
		std::cerr << "Error: could not create list's head" << std::endl;
		exit(-1);
	}
	if ((tail_node = shmalloc(tail)) == NULL) {
		std::cerr << "Error: could not create list's tail" << std::endl;
		exit(-1);
	}
	
	head_node->set_values(-1, -1, tail);
	tail_node->set_values(max, head, -1);
	shfree(head_node);
	shfree(tail_node);
}

// insert method; find the right place in the list, add val so that it is in
// sorted order; if val is already in the list, exit without inserting
void DList::insert(int val) {     
	// traverse the list to find the insertion point
	int prev = head;
	DNode* node = get_node(head);
	int curr = node->next;

	while (curr != -1) {
		shfree(node);
		node = get_node(curr);
		if (node->val >= val)
			break;

		prev = curr;
		curr = node->next;
	}

	// now insert new_node between prev and curr
	if (node->val > val) {
		int before = prev;
		int after = curr;
		int between = list_key++;
		DNode* new_node = NULL;
		if ((new_node = shmalloc(between)) == NULL) {
			std::cerr << "Error: could not insert node into the list" << std::endl;
			std::cerr << "Key: " << between << std::endl;
			exit(-1);
		}
		new_node->set_values(val, before, after);
		shfree(new_node);
		
		node->prev = between;
		shfree(node);
		node = get_node(before);
		node->next = between;
		shfree(node);
	}
}

// print the list
void DList::print() {
	DNode* curr = get_node(head);

	std::cout << "list :: ";
	while (curr != NULL) {
		std::cout << curr->val << "->";
		int next = curr->next;
		shfree(curr);
		curr = get_node(next);
	}
	std::cout << "NULL" << std::endl;
}

// increment every seqth element, starting with start, moving forward
void DList::increment_chunk(int chunk_num, int chunk_size) {
    int startpoint = chunk_num * chunk_size;
	int sum = 0;

	int chunk_start = -1;
	int ctr = 0;

	// forward traversal to compute sum and to find chunk_start
	DNode* node = get_node(head);
	int curr = node->next;
	
	sem_wait(&(shm->sem));
	while (curr != tail) {
		// if this is the start of our chunk, save the pointer
		shfree(node);
		node = get_node(curr);
		if (ctr++ == startpoint)
			chunk_start = curr;
		// add this value to the sum
		sum += node->val;
		// move to next node
		curr = node->next;
	}
	shfree(node);
	
	// OK, at this point we should have the ID of the chunk we're going to
	// work on.  increment everything in our chunk
	if (chunk_start != -1) {
		DNode* wr = get_node(chunk_start);
		// increment /chunk_size/ elements
		for (int i = 0; i < chunk_size; i++) {
			// don't increment if we reach the tail
			if (wr->next == -1)
				break;
			// increment, move to next
			(wr->val)++;
			int next = wr->next;
			shfree(wr);
			wr = get_node(next);
		}
		shfree(wr);
	}
	sem_post(&(shm->sem));
}
