#ifndef AVL_H
#define AVL_H

#include <utility>

namespace avl {

	struct AVL_node {
		unsigned char height_ = 1;
		int key_;
		unsigned size_ = 1;
		AVL_node *left_ = nullptr, *right_ = nullptr;
	};


	class AVL_tree {
	private:
		AVL_node* root_ = nullptr;

	public:
		explicit AVL_tree (int key) { root_ = new AVL_node; root_->key_ = key; }
		AVL_tree (const AVL_tree& that);
		AVL_tree (AVL_tree&& that);
		~AVL_tree ();

		AVL_tree& operator= (const AVL_tree& that);
		AVL_tree& operator= (AVL_tree&& that);

		void insert (int key);
		void remove (int key);
		void print (const char* filename) const;
	};

}

#endif