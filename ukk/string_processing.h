#ifndef STRING_PROCESSING_H
#define STRING_PROCESSING_H

#include "ukk.h"

/**
 * @brief 	Prepare text for building suffix tree
 */
namespace str_proc {

	/**
	 * @brief      Get the text and prepare it for building generalized suffix tree
	 *
	 * @param[in]  filename  The filename
	 * @param[out] sb   	 As a result it contains number of strings and the beginnings of the strings     
	 * 
	 * @see  	   suffix_tree::bounds::str_bounds
	 *
	 * @return     New version of the text
	 */
	char* get_text_for_ukk (const char* filename, suffix_tree::bounds::str_bounds** sb);
}

#endif
