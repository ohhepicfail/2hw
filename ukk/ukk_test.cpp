#include <cstdio>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include "ukk.h"
#include "string_processing.h"

using namespace suffix_tree;
using namespace str_proc;


void get_rand_str (char* str, unsigned length) {
	assert (str);

	for (unsigned i = 0; i < length - 1; i++)
		str[i] = rand () % 21 + 'a';

	str[length - 1] = '\0';
}

#define FIND_STR  0

int main () {
    vertex* suffix_tree = nullptr;

    #if FIND_STR
    srand (time (nullptr));
    unsigned length = 1000;
    char* str = new char[length];
    for (unsigned i = 0; i < 100000; i++)
    {
    	get_rand_str (str, length);
    	// printf ("%s\n", str);
    	suffix_tree = build (str);
    	unsigned chb,che;
    	bool is_ok = test (suffix_tree, str, &chb, &che);
    	clean (suffix_tree);
    	if (!is_ok) {
    		printf ("%s\n", str);
    		return -1;
    	}

    }
    #else
	    char fname[] = "strings.txt";
	    char* text = get_text_for_ukk (fname);
	    suffix_tree = build (text);
	    // print (suffix_tree, text);
	    unsigned chb,che;
	    bool is_ok = test (suffix_tree, text, &chb, &che);
	    if (!is_ok)
	    	printf ("something go wrong! chb %u %c  che %u %c\n", chb, text[chb], che, text[che]);
	    clean (suffix_tree);
	    delete[] text;  
    #endif  
    return 0;
}