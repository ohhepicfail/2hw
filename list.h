#ifndef LIST_H
#define LIST_H

namespace list {

	struct node_t;

	void 	 	delete_list (node_t* list);
	node_t*	 	create_list (const void* data);
	void 	 	push_back   (node_t* list, const void* data);
	void	 	push_front	(node_t** list, const void* data);
	const void* pop_front	(node_t** list);
	bool		is_empty	(const node_t* list);
	void	 	print_list  (const node_t* list);
	node_t*  	reverse	 	(node_t* list);
	void	 	random_loop (node_t* list);
	unsigned 	find_loop	(const node_t* list, const node_t** loop_begin);
}


#endif