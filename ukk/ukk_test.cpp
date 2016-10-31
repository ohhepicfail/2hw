#include <cassert>
#include "ukk.h"

using namespace suffix_tree;


int main () {
    char fname[] = "strings.txt";
    char* text = get_text_for_ukk (fname);

    vertex* suffix_tree = build_suffix_tree (text);
    // print_suffix_tree (suffix_tree, text);
    assert (suffix_tree);

    delete_suffix_tree (suffix_tree);
    delete[] text;    
    return 0;
}