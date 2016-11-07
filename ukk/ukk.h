#ifndef UKK_H
#define UKK_H

namespace suffix_tree {

	struct vertex;

	vertex* build (const char* text);
	// void    delete_suffix_tree  (vertex* root);
	void print (const vertex* root, const char* str);

}

#endif