#ifndef TREAP_H
#define TREAP_H

namespace treap {

	namespace query {
		struct query_t;

		query_t* create 		(unsigned size);
		void	 set 			(query_t* q, unsigned idx, unsigned value1, unsigned value2);
		int 	 get_answer		(query_t* q, unsigned idx);
		void 	 print 			(query_t* q, unsigned size);
		void 	 delete_query	(query_t* q);
		int 	 sort_cmp		(const void * x1, const void * x2);
		void	 sort 			(query_t* q, unsigned size);
		int 	 bsearch_cmp	(const void * x1, const void * x2);
		long	 binary_search	(query_t* q, unsigned val, unsigned size);
	}

	struct treap_t;

	treap_t* create_treap  (const int* data, unsigned size);
	void 	 printin_dot   (const treap_t* treap);
	void 	 delete_treap  (treap_t* treap);
	int		 find_min	   (treap_t* treap, const query::query_t& q);
	void	 tarjan_olca   (treap_t* treap, query::query_t* q, unsigned size);
}

#endif