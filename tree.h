#ifndef TREE_H
#define TREE_H

namespace tree {

	struct tree_t;

	tree_t* create_tree      (tree_t* parent, int data);
	void    delete_tree      (tree_t* tree);
	void	add			     (tree_t* tree, int data);
	void	print_tree	     (const tree_t* tree);
	void	printin_dot      (const tree_t* tree);
	void	get_2addresses   (const tree_t* tree, const tree_t** fst, const tree_t** snd);
	int 	calculate_length (const tree_t* fst, const tree_t* snd);
	int 	length_bitlabel	 (tree_t* fst, tree_t* snd);
}

#endif