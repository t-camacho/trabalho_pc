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

// constructor: head and tail have extreme values, point to each other
DList::DList(int max) : head(new DNode(-1)), tail(new DNode(max)) {
    head = new DNode;
	tail = new DNode(max);
	
	head->next = tail;
	tail->prev = head;
}

// insert method; find the right place in the list, add val so that it is in
// sorted order; if val is already in the list, exit without inserting
void DList::insert(int val) {     
	// traverse the list to find the insertion point
	DNode* prev = head;
	DNode* curr = prev->next;

	while (curr != NULL) {
		if (curr->val >= val)
			break;

		prev = curr;
		curr = prev->next;
	}

	// now insert new_node between prev and curr
	if (curr->val > val) {
		DNode* before = prev;
		DNode* after = curr;
		DNode* between = new DNode(val, before, after);
		before->next = between;
		after->prev = between;
	}
}

// remove a node if its value == val
void DList::remove(int val) {
	// find the node whose val matches the request
	DNode* prev = head;
	DNode* curr = prev->next;

	while (curr != NULL) {
		// if we find the node, disconnect it and end the search
		if (curr->val == val) {
			DNode* before = prev;
			DNode* after = curr->next;
			before->next = after;
			after->prev = before;

			// delete curr...
			delete curr;
			break;
		}
		else if (curr->val > val) {
			// this means the search failed
			break;
		}

		prev = curr;
		curr = prev->next;
	}
}

// print the list
void DList::print() const {
	DNode* curr = head;

	std::cout << "list :: ";
	while (curr != NULL) {
		std::cout << curr->val << "->";
		curr = curr->next;
	}
	std::cout << "NULL" << std::endl;
}

// increment every seqth element, starting with start, moving forward
void DList::increment_chunk(int chunk_num, int chunk_size) {
    int startpoint = chunk_num * chunk_size;
	int sum = 0;

	DNode* chunk_start = NULL;
	int ctr = 0;

	// forward traversal to compute sum and to find chunk_start
	DNode* curr = head->next;
	
	mutex.lock();
	while (curr != tail) {
		// if this is the start of our chunk, save the pointer
		if (ctr++ == startpoint)
			chunk_start = curr;
		// add this value to the sum
		sum += curr->val;
		// move to next node
		curr = curr->next;
	}
	
	// OK, at this point we should have the ID of the chunk we're going to
	// work on.  increment everything in our chunk
	if (chunk_start != NULL) {
		DNode* wr = chunk_start;
		// increment /chunk_size/ elements
		for (int i = 0; i < chunk_size; i++) {
			// don't increment if we reach the tail
			if (wr == tail)
				break;
			// increment, move to next
			(wr->val)++;
			wr = wr->next;
		}
	}
	mutex.unlock();
}
