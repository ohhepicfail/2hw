#ifndef STRING_PROCESSING_H
#define STRING_PROCESSING_H

#include "ukk.h"

namespace str_proc {
	char* get_text_for_ukk (const char* filename, suffix_tree::bounds::str_bounds** sb);
}

#endif
