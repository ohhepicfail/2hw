#include "avl.h"
#include <cstdio>
#include <list>
#include <cassert>
#include <climits>
#include <ctime>
#include <cstdlib>


static inline auto height (const AVL_node* node) -> decltype (node->height_);
static inline auto balance_factor (const AVL_node* node);
static inline void fix_height (AVL_node* node);
static inline AVL_node* rotate_right (AVL_node* node);
static inline AVL_node* rotate_left (AVL_node* node);
static inline AVL_node* balance (AVL_node* node);


static inline auto height (const AVL_node* node) -> decltype (node->height_) {
	if (!node)
		return 0;
	else
		return node->height_;
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
}


static inline AVL_node* rotate_right (AVL_node* node)
{
	auto* tmp   = node->left_;
	node->left_ = tmp->right_;
	tmp->right_ = node;

	fix_height (node);
	fix_height (tmp);

	return tmp;
}


static inline AVL_node* rotate_left (AVL_node* node)
{
	auto* tmp = node->right_;

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

	assert (root_);
	std::list<const AVL_node*> nodes {root_};

	fprintf (dot_file, "\t%u [label = \"root\"]\n", UINT_MAX);
	fprintf (dot_file, "\t%u -> %d\n", UINT_MAX, root_->key_); 

	while (!nodes.empty ()) {
		auto* cur = nodes.front ();
		nodes.pop_front ();

		fprintf (dot_file, "\t%d [label = \"key_ = %d\\n height_ = %u\"]\n", cur->key_, cur->key_, cur->height_);

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
	assert (root_);
	std::list<AVL_node**> path {&root_};

	for (;;) {
		auto* cur = path.back ();

		if ((*cur)->key_ == key)
			return;

		else if (key < (*cur)->key_) {
			if ((*cur)->left_)
				path.push_back (&(*cur)->left_);
			else {
				(*cur)->left_ = new AVL_node;
				(*cur)->left_->key_ = key;
				break;
			}
		}
		else if (key > (*cur)->key_) {
			if ((*cur)->right_)
				path.push_back (&(*cur)->right_);
			else {
				(*cur)->right_ = new AVL_node;
				(*cur)->right_->key_ = key;
				break;
			}
		}

	}

	while (!path.empty ()) {
		auto* cur = path.back ();
		path.pop_back ();

		*cur = balance (*cur);
	}
}


AVL_tree::~AVL_tree () {
	std::list<AVL_node*> nodes {root_};

	while (!nodes.empty ()) {
		auto* cur = nodes.front ();
		nodes.pop_front ();

		if (cur->left_)
			nodes.push_back (cur->left_);
		if (cur->right_)
			nodes.push_back (cur->right_);

		delete cur;
	}
}



int main () {
	AVL_tree avl (5);

	srand (time (0));
	auto n = 1000u;
	for (unsigned i = 0; i < n; i++)
		avl.insert (n / 2 - rand () % n);


	avl.print ("avl.dot");
}