#include <cassert>
#include "ukk.h"
#include "string_processing.h"

using namespace suffix_tree;
using namespace str_proc;


int main () {
    char fname[] = "strings.txt";
    char* text = get_text_for_ukk (fname);

    vertex* suffix_tree = build (text);
    print (suffix_tree, text);
    // trysuff ();
    assert (suffix_tree);

    // delete_suffix_tree (suffix_tree);
    delete[] text;    
    return 0;
}