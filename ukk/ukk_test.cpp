#include <cstdio>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include "ukk.h"
#include "string_processing.h"

using namespace suffix_tree;
using namespace str_proc;

#define PRINT_TREE 0


int main () {
    char fname[] = "strings.txt";
    bounds::str_bounds* bounds = nullptr;
    char* text = get_text_for_ukk (fname, &bounds);
    printf ("\n\ntext is ready\n");

    vertex* suffix_tree = build (text, bounds);
    printf ("suffix tree created!\n\n");

    #if PRINT_TREE
        print (suffix_tree, text);
    #endif

    common_str::substr** cstrings = common_str::find (suffix_tree, bounds, text);
    printf ("searching results:\n");
    for (unsigned k = 1u; k < bounds::nstr (bounds); k++) {
        printf ("\tk -- %u\t", k + 1);
        for (unsigned i = common_str::fst_ch (cstrings[k]); i < common_str::lst_ch (cstrings[k]); i++)
            printf ("%c", text[i]);
        printf ("\n");
    }


    clean (suffix_tree);
    bounds::clean (bounds);
    common_str::clean (cstrings);
    delete[] text;  
    return 0;
}
