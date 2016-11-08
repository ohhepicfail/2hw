#include <cstdio>
#include "ukk.h"
#include "string_processing.h"

using namespace suffix_tree;
using namespace str_proc;


int main () {
    char fname[] = "strings.txt";
    char* text = get_text_for_ukk (fname);

    vertex* suffix_tree = build (text);
    print (suffix_tree, text);
    unsigned chb,che;
    bool is_ok = test (suffix_tree, text, &chb, &che);
    if (!is_ok)
    	printf ("something go wrong! chb %u %c  che %u %c\n", chb, text[chb], che, text[che]);

    clean (suffix_tree);
    delete[] text;    
    return 0;
}