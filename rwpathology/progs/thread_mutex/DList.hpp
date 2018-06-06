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

#ifndef DLIST_HPP__
#define DLIST_HPP__

namespace bench {
  // Doubly-Linked List workload
  class DList {
      // DNode is a node in the DList
      class DNode {
		public:
			int val;
			DNode* prev;
			DNode* next;

          // basic constructor
          DNode(int v = -1) : val(v), prev(NULL), next(NULL) { }

          // parameterized constructor for inserts
          DNode(int v,
                DNode* p,
                DNode* n)
              : val(v), prev(p), next(n)
          { }
      };

      // the dlist keeps head and tail pointers, for bidirectional traversal
      DNode *head, *tail;
	  
    public:
      DList(int max);

      // insert a node if it doesn't already exist
      virtual void insert(int val);

      // remove a node if its value = val
      virtual void remove(int val);

      // print the whole list (assumes isolation)
      virtual void print() const;

      // read the whole list, then increment every element in the chunk
      // starting at chunk_num*chunk_size
      void increment_chunk(int chunk_num, int chunk_size);
  };

} // namespace bench
#endif // DLIST_HPP__
