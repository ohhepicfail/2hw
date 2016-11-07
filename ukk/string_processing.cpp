#include <cstdio>
#include <cassert>
#include <string>
#include <cstring>
#include "string_processing.h"

namespace str_proc {

	static inline unsigned  get_file_size           (const char* filename);
    static inline char*     read_file               (const char* filename, unsigned fsize);
    static inline unsigned  count_numbers           (unsigned n);
    static inline char*     concatenate_strings_v1  (const char* strings, unsigned tsize);
    static inline char*     concatenate_strings_v2  (const char* strings, unsigned tsize);


    static inline unsigned get_file_size (const char* filename) {
        assert (filename);
        FILE* f = fopen (filename, "r");
        assert (f);

        fseek (f, 0, SEEK_END);
        unsigned fsize = ftell (f);
        fseek (f, 0, SEEK_SET);

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

        char* text2 = concatenate_strings_v2 (text, fsize);
        printf ("Your text after concatenation:\n%s\n\n", text2);

        delete[] text;
        return text2;
    }

}