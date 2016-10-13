#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "list.h"

namespace list {

	struct node_t {
		struct node_t* next_;
		const void* data_;
	};


	void delete_list (node_t* list) {
		assert (list);

		const node_t* loop_begin;
		unsigned loop_size  = find_loop (list, &loop_begin);

		while (list && loop_begin != list) {
			node_t* tmp = list->next_;
			delete list;
			list = tmp;
		}
		while (loop_size > 0){
			node_t* tmp = list->next_;
			delete list;
			list = tmp;
			loop_size--;
		}
	}


	node_t* create_list (const void* data) {
		node_t* list = new node_t;
		list->data_ = data;
		list->next_ = nullptr;

		return list;
	}


	void push_back (node_t* list, const void* data) {
		assert (list);

		while (list->next_)
			list = list->next_;

		node_t* tmp = new node_t;
		list->next_ = tmp;
		tmp->data_  = data;
		tmp->next_  = nullptr;
	}


	void push_front (node_t** list, const void* data) {
		assert (list);
		assert (*list);

		node_t* nd = new node_t;
		nd ->data_ = data;
		nd->next_  = *list;
		*list = nd; 
	}


	bool is_empty (const node_t* list) {
		if (list)
			return false;
		return true;
	}


	const void* pop_front	(node_t** list) {
		assert (list);
		assert (*list);

		unsigned loop_size  = find_loop (*list, nullptr);
		assert ( ! loop_size);

		const void* res = (*list)->data_;

		node_t* nd = (*list)->next_;
		delete *list;
		*list = nd;

		return res;
	}


	void print_list (const node_t* list) {
		assert (list);
		const node_t* loop_begin;
		unsigned loop_size  = find_loop (list, &loop_begin);

		while (list && loop_begin != list) {
			printf ("%p\t", list->data_);
			list = list->next_;
		}
		while (loop_size > 0){
			printf ("%p\t", list->data_);
			list = list->next_;
			loop_size--;
		}
		printf ("\n");
	}


	node_t* reverse (node_t* list) {
		assert (list);

		node_t* prev = list;
		node_t* cur  = list->next_;
		list = cur;
		prev->next_ = nullptr;

		while (list) {
			list = cur->next_;
			cur->next_ = prev;
			prev = cur;
			cur = list;
		}
		
		return prev;
	}


	void random_loop (node_t* list){
		assert (list);

		unsigned list_sz = 0;
		node_t* cur = list;
		for (; cur->next_; cur = cur->next_)
			list_sz++;

		srand (time (nullptr));
		unsigned n = rand ()  % (list_sz + 1);

		node_t* cycle_begin = list;
		for (unsigned i = 0; i < n; i++)
			cycle_begin = cycle_begin->next_;

		cur->next_ = cycle_begin;
	}


	unsigned find_loop (const node_t* list, const node_t** loop_begin){
		assert (list);

		const node_t* turtle = list;
		const node_t* rabbit = list;
		const unsigned rabbit_speed = 2u;

		while (1){
			for (unsigned i = 0; i < rabbit_speed; i++){
				if ( ! rabbit->next_)
					return 0;
				rabbit = rabbit->next_;
			}

			turtle = turtle->next_;

			if (rabbit == turtle)
				break;
		}

		unsigned loop_size = 1;
		for (turtle = turtle->next_; turtle != rabbit; turtle = turtle->next_)
			loop_size++;
		

		if (loop_begin){
			const node_t* first  = list;
			const node_t* second = rabbit;

			while (first != second){
				first  = first->next_;
				second = second->next_;
			}

			*loop_begin = first;
		}

		return loop_size;
	}
}
