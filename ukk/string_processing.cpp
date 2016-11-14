#include <cstdio>
#include <cassert>
#include <string>
#include <cstring>
#include "string_processing.h"

namespace str_proc {


    /**
     * @brief      Counts size of the file
     *
     * @param[in]  filename  Name of the file
     *
     * @return     Size of the file
     */
    static inline unsigned  get_file_size           (const char* filename);

    /**
     * @brief      Reads a all file by fread     
     *
     * @param[in]  filename  Name of the file
     * @param[in]  fsize     Size of the file
     *
     * @return     Text from the file
     */
    static inline char*     read_file               (const char* filename, unsigned fsize);
    
    /**
     * @brief      Counts numbers in the number
     *
     * @param[in]  n     number
     *
     * @return     Number of numbers =)
     */
    static inline unsigned  count_numbers           (unsigned n);

    /**
     * @brief      Concatenates text. All '\n' -> "$ + 'reversed-string-number' + /" 
     *
     * @param[in]  strings  Some strings
     * @param[in]  size     Summary length of the strings
     * @param[in]  sb       As a result it contains number of strings and the beginnings of the strings
     * 
     * @see        suffix_tree::bounds::str_bounds
     * @see        concatenate_strings_v2
     *
     * @return     New text
     */
    static inline char*     concatenate_strings_v1  (const char* strings, unsigned size, suffix_tree::bounds::str_bounds** sb);

    /**
     * @brief      Concatenates text. It deletes all '\n'. It's all
     *
     * @param[in]  strings  The strings
     * @param[in]  size     The size
     * @param[in]  sb       As a result it contains number of strings and the beginnings of the strings
     * 
     * @see        suffix_tree::bounds::str_bounds
     * @see        concatenate_strings_v1
     *
     * @return     New text
     */
    static inline char*     concatenate_strings_v2  (const char* strings, unsigned size, suffix_tree::bounds::str_bounds** sb);


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
        if (n == 0)
            return 1u;
        while (n > 0) {
            n /= 10;
            counter++;
        }

        return counter;
    }

    static inline char* concatenate_strings_v1 (const char* strings, unsigned size, suffix_tree::bounds::str_bounds** sb) {
        assert (strings);

        unsigned extra_symb = 1u;       // $0 at the end
        for (unsigned i = 0; i < size; ++i)
            if (strings[i] == '\n')
                extra_symb++;

        unsigned extra_size = 0;       
        for (unsigned i = 0; i <= extra_symb; i++)
                extra_size += count_numbers (i);

        char* text = new char[size + extra_size + extra_symb];
        unsigned* strings_bounds = new unsigned[extra_symb];
        unsigned nstr = extra_symb; 
        strings_bounds[0] = 0;
        for (unsigned i = 0, j = 0, k = 1; i < size; ++i) {

            if (strings[i] == '\n' || strings[i] == '\0') {

                text[j++] = '$';
                strcpy (text + j, std::to_string (extra_symb).c_str ());
                j += count_numbers (extra_symb);
                extra_symb--;
                text[j++] = '/';

                if (strings[i] == '\0')
                    text[j++] = strings[i];
                else
                    strings_bounds[k++] = j;
            } 
            else
                text[j++] = strings[i];
        }

        *sb = suffix_tree::bounds::set (strings_bounds, nstr);

        return text;
    }


    static inline char* concatenate_strings_v2 (const char* strings, unsigned size, suffix_tree::bounds::str_bounds** sb) {
        assert (strings);

        unsigned extra_symb = 0;
        for (unsigned i = 0; i < size; ++i)
            if (strings[i] == '\n' && strings[i + 1] != '\0')
                extra_symb++;

        char* text = new char[size - extra_symb];
        unsigned* strings_bounds = new unsigned[extra_symb + 1]; 
        for (unsigned i = 0, j = 0, k = 0; i < size; i++) {
            if (strings[i] != '\n')
                text[j++] = strings[i];
            else
                strings_bounds[k++] = j;
        }
        strings_bounds[extra_symb] = 0;

        *sb = suffix_tree::bounds::set (strings_bounds, extra_symb + 1);

        return text;
    }


    char* get_text_for_ukk (const char* filename, suffix_tree::bounds::str_bounds** sb) {
        assert (filename);
        unsigned fsize = get_file_size (filename);
        char* text = read_file (filename, fsize);

        char* text2 = concatenate_strings_v1 (text, fsize, sb);

        delete[] text;
        return text2;
    }

}
