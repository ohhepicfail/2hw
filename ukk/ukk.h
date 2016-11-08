#ifndef UKK_H
#define UKK_H

namespace suffix_tree {

	struct vertex;

	vertex* build (const char* text);
	void    clean (vertex* root);
	void 	print (const vertex* root, const char* str);
	bool	test  (const vertex* root, const char* str, unsigned* chb, unsigned* che);

}

#endif