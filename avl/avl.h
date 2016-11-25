#ifndef AVL_H
#define AVL_H


struct AVL_node {
	int key_;
	unsigned char height_ = 1;
	AVL_node *left_ = nullptr, *right_ = nullptr;
};


class AVL_tree {
private:
	AVL_node* root_ = nullptr;

public:
	//
	//
	//
	//	all ctors
	//	
	//	
	//	

	explicit AVL_tree (int key) { root_ = new AVL_node; root_->key_ = key; }
	~AVL_tree ();

	void insert (int key);
	void print (const char* filename) const;

};

#endif