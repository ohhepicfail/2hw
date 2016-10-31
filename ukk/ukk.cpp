#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cmath>
#include <cassert>
#include <climits>
#include "ukk.h"

namespace suffix_tree {

    namespace chlist {

        struct children_list {
            unsigned begin_, end_;
            vertex* child_;
            children_list* next_;
        };

        static inline children_list* push_front (children_list* chl, unsigned begin, unsigned end, vertex* vert);
        static inline void           delete_l   (children_list* chl);

    }

    struct vertex {
        chlist::children_list* child_;
    };


    static inline unsigned  get_file_size           (const char* filename);
    static inline char*     read_file               (const char* filename, unsigned fsize);
    static inline unsigned  count_numbers           (unsigned n);
    static inline char*     concatenate_strings_v1  (const char* strings, unsigned tsize);
    static inline char*     concatenate_strings_v2  (const char* strings, unsigned tsize);
    static inline vertex*   add                     (vertex* root,  const char* str, unsigned begin, unsigned end);
    static inline vertex*   choose_vert_and_add     (vertex* root,  const char* str, unsigned begin, unsigned end);
    static inline vertex*   split                   (unsigned vert1_begin, unsigned vert2_begin, unsigned vert2_end, vertex* vert2);
    static inline void      print_tree              (const vertex* vert, unsigned tabs, const char* str);


    namespace chlist {

        static inline children_list* push_front (children_list* chl, unsigned begin, unsigned end, vertex* vert) {
            children_list* new_ch = new children_list;
            new_ch->begin_ = begin;
            new_ch->end_   = end;
            new_ch->child_ = vert;
            new_ch->next_  = chl;

            return new_ch;
        }


        static inline void delete_l (children_list* chl) {
            for (children_list* tmp = nullptr; chl; chl = tmp) {
                tmp = chl->next_;
                delete_suffix_tree (chl->child_);
                delete chl;
            }
        }

    }


    static inline unsigned get_file_size (const char* filename) {
        assert (filename);
        FILE* f = fopen (filename, "r");
        assert (f);

        fseek (f, 0, SEEK_END);
        unsigned fsize = ftell (f);
        fseek (f, 0, SEEK_END);

        fclose (f);
        return fsize + 1;
    }


    static inline char* read_file (const char* filename, unsigned fsize) {
        assert (filename);
        FILE* f = fopen (filename, "r");
        assert (f);

        char* text = new char[fsize];
        fread (text, fsize - 1, sizeof (char), f);
        text[fsize - 1] = '\0';

        fclose (f);
        return text;
    }


    static inline unsigned count_numbers (unsigned n) {
        unsigned counter = 0;
        while (n > 0) {
            n /= 10;
            counter++;
        }

        return counter;
    }

    static inline char* concatenate_strings_v1 (const char* strings, unsigned tsize) {
        assert (strings);

        unsigned extra_symb = 0;
        for (unsigned i = 0; i < tsize; ++i)
            if (strings[i] == '\n')
                extra_symb++;

        unsigned extra_size = 0;
        for (unsigned i = 1; i <= extra_symb; i++)
                extra_size += count_numbers (i);

        char* text = new char[tsize + extra_size];

        for (unsigned i = 0, j = 0; i < tsize; ++i) {

            if (strings[i] == '\n') {
                text[j++] = '\\';
                strcpy (text + j, std::to_string (extra_symb).c_str ());
                j += count_numbers (extra_symb);
                extra_symb--;
            } 

            else
                text[j++] = strings[i];
        }

        return text;
    }


    static inline char* concatenate_strings_v2 (const char* strings, unsigned tsize) {
        assert (strings);

        unsigned extra_symb = 0;
        for (unsigned i = 0; i < tsize; ++i)
            if (strings[i] == '\n')
                extra_symb++;

        char* text = new char[tsize - extra_symb];
        for (unsigned i = 0, j = 0; i < tsize; i++)
            if (strings[i] != '\n')
                text[j++] = strings[i];

        return text;
    }


    char* get_text_for_ukk (const char* filename) {
        assert (filename);
        unsigned fsize = get_file_size (filename);
        char* text = read_file (filename, fsize);
        printf ("\n\nYour text:\n%s\n\n", text);
        // char* text2 = concatenate_strings_v1 (text, fsize);
        char* text2 = concatenate_strings_v2 (text, fsize);
        printf ("Your text after concatenation:\n%s\n\n", text2);

        delete[] text;
        return text2;
    }

    static inline vertex* add (vertex* root, const char* str, unsigned begin, unsigned end) {
        assert (str);

        if (!root) {
            root = new vertex;
            root->child_ = chlist::push_front (nullptr, 0, UINT_MAX, nullptr);
        }
        else
            for (; begin <= end; begin++)
                root = choose_vert_and_add (root, str, begin, end);      

        return root;
    }


    static inline vertex* choose_vert_and_add (vertex* root,  const char* str, unsigned begin, unsigned end) {
        assert (root);
        assert (str);

        chlist::children_list* cur = root->child_;
        while (cur && str[cur->begin_] != str[begin])
            cur = cur->next_;

        if (!cur) 
           root->child_ = chlist::push_front (root->child_, begin, UINT_MAX, nullptr); 
        // list
        else if (cur->end_ == UINT_MAX) {
            bool all_the_same = true;
            unsigned i = begin;
            unsigned j = cur->begin_;
            for (; i <= end; i++, j++)
                if (str[i] != str[j]) {
                    all_the_same = false;
                    break;
                }

            assert (!cur->child_);
            if (!all_the_same) {
                cur->child_ = split (i, j, UINT_MAX, nullptr);
                cur->end_ = j;
            }
        }
        // not a list
        else {
            unsigned i = begin;
            unsigned j = cur->begin_;
            bool all_the_same = true;
            for (; j < cur->end_ && i <= end; i++, j++)
                if (str[i] != str[j]) {
                    all_the_same = false;
                    break;
                }

            assert (cur->child_);
            if (!all_the_same)
                cur->child_ = split (i, j, cur->end_, cur->child_);
            else if (j == cur->end_)
                cur->child_ = choose_vert_and_add (cur->child_, str, i, end);
        }

        return root;
    }


    static inline vertex* split (unsigned vert1_begin, unsigned vert2_begin, unsigned vert2_end, vertex* vert2) {
        vertex* vert = new vertex;
        vert->child_ = chlist::push_front (nullptr, vert1_begin, UINT_MAX,  nullptr);
        vert->child_ = chlist::push_front (vert->child_, vert2_begin, vert2_end, vert2);

        return vert;
    }


    vertex* build_suffix_tree (const char* text) {
        assert (text);

        vertex* root = nullptr;
        unsigned i = 0;
        for (; text[i] != '\0'; i++) {
            root = add (root, text, 0, i);
            // printf ("\n\n%u\n", i);
            // print_suffix_tree (root, text);
        }
        // printf ("\n\nLast\n");
        root = add (root, text, 0, i); 

        return root;
    }


    void delete_suffix_tree (vertex* root) {
        if (root)
            delete_l (root->child_);
        delete root;
    }


    void print_suffix_tree (const vertex* root, const char* str) {
        assert (root);

        print_tree (root, 0, str);
    }


    static inline void print_tree (const vertex* vert, unsigned tabs, const char* str) {
        assert (vert);

        chlist::children_list* cur = vert->child_;

        while (cur) {
            for (unsigned i = 0; i < tabs; i++)
                printf ("\t");

            printf ("[%u -- %u)\t", cur->begin_, cur->end_);
            for (unsigned i = cur->begin_; (cur->end_ < UINT_MAX) ? i < cur->end_ : str[i] != '\0'; i++)
                printf ("%c", str[i]);
            printf ("\n");
            if (cur->child_)
                print_tree (cur->child_, tabs + 1, str);

            cur = cur->next_;
        }
    }

}
