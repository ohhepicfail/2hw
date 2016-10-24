#include <cassert>
#include "queue.h"


namespace queue {
	struct queue_node_t {
		queue_node_t* next_;
		queue_node_t* prev_;
		const void* data_;
	};


	struct queue_t {
		queue_node_t* head_;
		queue_node_t* tail_;
	};


	queue_t* create_or_push (queue_t* q, const void* data) {
		queue_node_t* new_q = new queue_node_t;
		new_q->next_ = nullptr;
		new_q->data_ = data;

		if (!q || !q->head_) {
			if (!q)
				q = new queue_t;
			q->head_ = new_q;
			q->tail_ = new_q;
			new_q->prev_ = nullptr;
		}
		else {
			q->tail_->next_ = new_q;
			new_q->prev_ = q->tail_;
			q->tail_ = new_q;
		}

		return q;
	}


	const void* pop (queue_t* q) {
		assert (q);
		assert (!is_empty (q));

		const void* res = q->tail_->data_;
		queue_node_t* last = q->tail_;

		q->tail_ = q->tail_->prev_;
		if (last == q->head_)
			q->head_ = nullptr;
		delete last;

		return res;
	}


	bool is_empty (const queue_t* q) {
		assert (q);

		if (q->tail_)
			return 0;
		return 1;
	}


	void delete_queue (queue_t* q) {
		assert (q);

		while (!is_empty (q))
			pop (q);

		delete q;
	}
}