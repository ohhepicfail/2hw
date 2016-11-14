#ifndef UKK_H
#define UKK_H

namespace suffix_tree {

	namespace bounds {
		struct str_bounds;

		str_bounds* 	set    (unsigned* bounds, unsigned nstrings);
		void			clean  (str_bounds* sb);
		unsigned		nstr   (const str_bounds* sb);
		const unsigned*	get_sb (const str_bounds* sb);
	}

	// namespace common_str {
		// struct substr;
		
	// 	substr** find   (const vertex* root, const bounds::str_bounds* sb);
	// 	unsigned fst_ch (substr** common, unsigned common_idx);
	// 	unsigned lst_ch (substr** common, unsigned common_idx);
	// 	void  	 clean  (substr** common, unsigned size);
	// }

	struct vertex;

	vertex* build (const char* text, const bounds::str_bounds* sb);
	void    clean (vertex* root);
	void 	print (const vertex* root, const char* str);
	bool	test  (const vertex* root, const char* str);
}

#endif
