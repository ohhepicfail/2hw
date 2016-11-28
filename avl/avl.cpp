#include "avl.h"
#include <cstdio>
#include <list>
#include <cassert>
#include <climits>


namespace avl {

	static inline auto		height 			(const AVL_node* node) -> decltype (node->height_);
	static inline auto		size 			(const AVL_node* node) -> decltype (node->size_);
	static inline auto		balance_factor 	(const AVL_node* node);
	static inline void		fix_height 		(AVL_node* node);
	static inline AVL_node*	rotate_right 	(AVL_node* node);
	static inline AVL_node*	rotate_left  	(AVL_node* node);
	static inline AVL_node* balance 	 	(AVL_node* node);


	static inline auto height (const AVL_node* node) -> decltype (node->height_) {
		if (!node)
			return 0;
		else
			return node->height_;
	}


	static inline auto size (const AVL_node* node) -> decltype (node->size_) {
		if (!node)
			return 0;
		else return node->size_;
	}


	static inline auto balance_factor (const AVL_node* node) {
		assert (node);
		return height (node->right_) - height (node->left_);
	}


	static inline void fix_height (AVL_node* node) {
		assert (node);
		auto rh = height (node->right_);
		auto lh = height (node->left_);

		node->height_ = (rh > lh) ? rh : lh ;
		node->height_++;

		auto rs = size (node->right_);
		auto ls = size (node->left_);

		node->size_ = ls + rs + 1;
	}


	static inline AVL_node* rotate_right (AVL_node* node)
	{
		auto tmp   = node->left_;
		node->left_ = tmp->right_;
		tmp->right_ = node;

		fix_height (node);
		fix_height (tmp);

		return tmp;
	}


	static inline AVL_node* rotate_left (AVL_node* node)
	{
		auto tmp = node->right_;

		node->right_ = tmp->left_;
		tmp->left_   = node;

		fix_height (node);
		fix_height (tmp);

		return tmp;
	}


	static inline AVL_node* balance (AVL_node* node)
	{
		fix_height (node);
		if(balance_factor (node) == 2)
		{
			if(balance_factor (node->right_) < 0)
				node->right_ = rotate_right (node->right_);
			return rotate_left (node);
		}
		if(balance_factor (node) == -2)
		{
			if(balance_factor (node->left_) > 0)
				node->left_ = rotate_left (node->left_);
			return rotate_right (node);
		}
		return node;
	}


	void AVL_tree::print (const char* filename) const {
		assert (filename);

		FILE* dot_file = fopen (filename, "wb");
		assert (dot_file);

		fprintf (dot_file, "digraph avltree\n{\n");
		fprintf (dot_file, "\t%u [label = \"root\"]\n", UINT_MAX);

		if (!root_) {
			fprintf (dot_file, "}\n");
			fclose (dot_file);
			return;
		}

		std::list<const AVL_node*> nodes {root_};

		fprintf (dot_file, "\t%u -> %d\n", UINT_MAX, root_->key_); 

		while (!nodes.empty ()) {
			auto cur = nodes.front ();
			nodes.pop_front ();

			fprintf (dot_file, "\t%d [label = \"key = %d\\n height = %u\\n size = %u\"]\n", cur->key_, cur->key_, cur->height_, cur->size_);

			if (cur->left_) {
				nodes.push_back (cur->left_);
				fprintf (dot_file, "\t%d -> %d\n", cur->key_, cur->left_->key_);
			}
			if (cur->right_) {
				nodes.push_back (cur->right_);
				fprintf (dot_file, "\t%d -> %d\n", cur->key_, cur->right_->key_);
			}
		}

		fprintf (dot_file, "}\n");

		fclose (dot_file);
	}


	void AVL_tree::insert (int key) {
		if (!root_) {
			root_ = new AVL_node;
			root_->key_ = key;
			return;
		}

		std::list<AVL_node**> path {&root_};

		for (;;) {
			auto cur = *path.back ();

			if (cur->key_ == key)
				return;

			else if (key < cur->key_) {
				if (cur->left_)
					path.push_back (&cur->left_);
				else {
					cur->left_ = new AVL_node;
					cur->left_->key_ = key;
					break;
				}
			}
			else if (key > cur->key_) {
				if (cur->right_)
					path.push_back (&cur->right_);
				else {
					cur->right_ = new AVL_node;
					cur->right_->key_ = key;
					break;
				}
			}

		}

		while (!path.empty ()) {
			auto cur = path.back ();
			path.pop_back ();

			*cur = balance (*cur);
		}
	}


	void AVL_tree::remove (int key) {
		if (!root_)
			return;

		std::list<AVL_node**> path {&root_};

		for (;;) {
			auto cur = *path.back ();

			if (key == cur->key_)
				break;
			else if (key < cur->key_ && cur->left_)
				path.push_back (&cur->left_);
			else if (key > cur->key_ && cur->right_)
				path.push_back (&cur->right_);
			else 
				return;
		}

		auto for_del = path.back ();
		auto lch = &(*for_del)->left_;
		auto rch = &(*for_del)->right_;


		if (!(*rch)) {
			if (!(*lch)) {
				path.pop_back ();
				delete *for_del;
				*for_del = nullptr;
			}
			else {
				(*for_del)->key_ = (*lch)->key_;
				delete *lch;
				*lch = nullptr;
			}
		}
		else {
			path.push_back (rch);

			auto minch = path.back ();
			for (; (*minch)->left_; minch = path.back ())
				path.push_back (&(*minch)->left_);

			(*for_del)->key_ = (*minch)->key_;

			auto tmp = (*minch)->right_;
			path.pop_back ();
			delete *minch;
			*minch = tmp;
		}

		while (!path.empty ()) {
			auto cur = path.back ();
			path.pop_back ();

			*cur = balance (*cur);
		}
	}


	AVL_tree::AVL_tree (const AVL_tree& that) {
		if (that.root_) {

			std::list<const AVL_node*> from {that.root_};

			while (!from.empty ()) {
				auto cur = from.front ();
				from.pop_front ();

				insert (cur->key_);

				if (cur->left_)
					from.push_back (cur->left_);
				if (cur->right_)
					from.push_back (cur->right_);
			}
		}
	}


	AVL_tree& AVL_tree::operator= (const AVL_tree& that) {		
		if (this != &that) {
			AVL_tree tmp (that);
			*this = std::move (tmp);
		}

		return *this;
	}


	AVL_tree::AVL_tree (AVL_tree&& that) : root_ (that.root_) {
		that.root_ = nullptr;
	}


	AVL_tree& AVL_tree::operator= (AVL_tree&& that) {
		if (root_ != that.root_)
			std::swap (root_, that.root_);
		return *this;
	}


	AVL_tree::~AVL_tree () {
		if (root_) {
			std::list<AVL_node*> nodes {root_};

			while (!nodes.empty ()) {
				auto cur = nodes.front ();
				nodes.pop_front ();

				if (cur->left_)
					nodes.push_back (cur->left_);
				if (cur->right_)
					nodes.push_back (cur->right_);

				delete cur;
			}
		}
	}


}