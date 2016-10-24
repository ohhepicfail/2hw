#ifndef QUEUE_H
#define QUEUE_H

namespace queue {
	struct queue_node_t;
	struct queue_t;

	queue_t* 	create_or_push  (queue_t* q, const void* data);
	const void* pop 			(queue_t* q);
	bool		is_empty		(const queue_t* q);
	void		delete_queue 	(queue_t* q);
}


#endif