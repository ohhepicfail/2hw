#ifndef TREAP_H
#define TREAP_H

namespace treap {

	struct treap_t;

	treap_t* create_treap  (const int* data, unsigned size);
	void 	 printin_dot   (const treap_t* treap);
	void 	 delete_treap  (treap_t* treap);
	int		 find_min	   (treap_t* treap, unsigned fst, unsigned snd);
	void	 tarjan_olca   (treap_t* treap, unsigned* fst, unsigned* snd, int* answers, unsigned size);
}

#endif