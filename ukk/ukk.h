#ifndef UKK_H
#define UKK_H

/**
 * @brief 	Contains functions and structure for build suffix tree and work with it. 
 */
namespace suffix_tree {

	/**
	 * @brief      Contains functions and structure for work with strings bounds
	 */
	namespace bounds {
		struct str_bounds;

		/**
		 * @brief      Sets strings bounds
		 *
		 * @param[in]  bounds    Array with the strings beginnings
		 * @param[in]  nstrings  Number of the strings
		 * 
		 * @warning    Function uses simple copy, not deep. So, don't free bounds memory! 
		 *
		 * @return     str_bounds structure
		 */
		str_bounds* 	set    (unsigned* bounds, unsigned nstrings);

		/**
		 * @brief      Delete allocated memory 
		 *
		 * @param[in]  sb    str_bounds
		 */
		void			clean  (str_bounds* sb);

		/**
		 * @brief      Returns strings number
		 *
		 * @param[in]  sb  str_bounds  
		 *
		 * @return     Number of the strings in sb
		 */
		unsigned		nstr   (const str_bounds* sb);

		/**
		 * @brief      Return array of the strings bounds
		 *
		 * @param[in]  sb    str_bounds 
		 *
		 * @return     Array of the strings bounds
		 */
		const unsigned*	get_sb (const str_bounds* sb);
	}

	struct vertex;

	/**
	 * @brief      Build suffix tree using Ukkonen's algorithm
	 *
	 * @param[in]  text  Text that prepared for building tree
	 * @param[in]  sb    Structure with infornation about text
	 *
	 * @see        bounds
	 * @see        str_proc
	 *
	 * @return     Suffix tree
	 */
	vertex* build (const char* text, const bounds::str_bounds* sb);

	/**
	 * @brief      Deletes suffix tree
	 *
	 * @param      root  Suffix tree root
	 */
	void    clean (vertex* root);

	/**
	 * @brief      Prints suffix tree in .dot file
	 *
	 * @param[in]  root  Suffix tree root
	 * @param[in]  str   Original text that builds a suffix tree
	 */
	void 	print (const vertex* root, const char* str);

	/**
	 * @brief      Tries to find all suffixes of str in suffix tree
	 *
	 * @param[in]  root  Suffix tree root
	 * @param[in]  str   String
	 *
	 * @return     true if all suffixes have been found. Otherwise false
	 */
	bool	test  (const vertex* root, const char* str);

	namespace common_str {
		struct substr;
		
		substr** find   (const vertex* root, const bounds::str_bounds* sb, const char* text);
		unsigned fst_ch (substr* common);
		unsigned lst_ch (substr* common);
		void  	 clean  (substr** common);
	}
}

#endif
