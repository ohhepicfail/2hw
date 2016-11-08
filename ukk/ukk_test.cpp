#include <cstdio>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include "ukk.h"
#include "string_processing.h"

using namespace suffix_tree;
using namespace str_proc;


int main () {
    char fname[] = "strings.txt";
    char* text = get_text_for_ukk (fname);
    printf ("\n\ntext is ready\n\n");

    vertex* suffix_tree = build (text);
    printf ("suffix tree created!\n\n");

    print (suffix_tree, text);
    bool is_ok = test (suffix_tree, text);
    if (!is_ok)
    	printf ("something go wrong!\n");
    else
    	printf ("all is ok\n\n");

    clean (suffix_tree);
    delete[] text;  
    return 0;
}