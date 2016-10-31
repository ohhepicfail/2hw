#ifndef UKK_H
#define UKK_H

namespace suffix_tree {

	struct vertex;

	char* 	get_text_for_ukk  	(const char* filename);
	vertex* build_suffix_tree	(const char* text);
	void    delete_suffix_tree  (vertex* root);
	void 	print_suffix_tree	(const vertex* root, const char* str);

}

#endif