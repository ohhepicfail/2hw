#include <cassert>
#include <climits>
#include <unordered_map>
#include <list>
#include "ukk.h"

namespace suffix_tree {

    /**
     * @brief      Cointains data about child and functions to work with it
     */
    namespace chld {

        /**
         * @brief      Cointains data about child
         */
        struct child {
            unsigned begin_;
            unsigned end_;
            vertex* to_;
        };
    }


    /**
     * @brief      Contains map with its children and suffix link to another vertex
     * 
     * @see        chld
     */
    struct vertex {
        std::unordered_map<char, chld::child*> children_;
        vertex* suffix_link_;
    };

    /**
     * @brief     Contains information about active point
     * 
     * @see Ukkonen's algorithm
     */
    namespace ap {

        /**
         * @brief      Main figure in Ukkonen's algorithm
         */
        struct active_point {
            unsigned ch_idx_;   ///< Key in map
            vertex* vert_;      ///< Current vertex
            unsigned depth_;    ///< Current symbol index is depth_ + ch_idx_
        };

        enum depth_key {
            VERTEX = 0,
            LIST = 1
        };
    }


    namespace bounds {

        /**
         * @brief      Contains array of the strings bounds and number of the strings
         */
        struct str_bounds {
            unsigned* bounds_;
            unsigned  nstr_;
        };
    }


    namespace common_str {

        /**
         * @brief      Contains data about substring
         */
        struct substr {
            unsigned begin_;    ///< First symbol index of common substring
            unsigned end_;      ///< Symbol index following the last of common substring
            unsigned rank_;     ///< Number of strings where this substring were found
        };
    }


    namespace chld {

        /**
         * @brief      Adds new child
         *
         * @param[in]  ch         Key in map
         * @param[out] vert       Vertex to which child will be added
         * @param[in]  begin      First child index
         * @param[in]  end        Last child index
         * @param      chld_vert  Vertex that child is pointing
         */
        static inline void   add    (char ch, vertex* vert, unsigned begin, unsigned end, vertex* chld_vert); 

        /**
         * @brief      Finds child in vertex 
         *
         * @param[in]  ch    Key 
         * @param[in]  vert  Vertex
         *
         * @return     child or nullptr
         */
        static inline child* find   (char ch, const vertex* vert);

        /**
         * @brief      Creates new child
         *
         * @param[in]  begin  First child index
         * @param[in]  end    Last child index
         * @param[in]  child  Vertex that child is pointing
         *
         * @return     New child
         */
        static inline child* create (unsigned begin, unsigned end, vertex* child);
    }


    namespace ap {

        /**
         * @brief      Sets data in active point
         *
         * @param[out] ap        Current active point
         * @param[in]  symb_idx  ch_idx_ in ap
         * @param[in]  v         vert_ in ap
         * @param[in]  depth     depth_ in ap
         */
        static inline void set       (active_point* ap, unsigned symb_idx, vertex* v, unsigned depth);

        /**
         * @brief      Increases depth in active point
         *
         * @param      ap    active point
         */
        static inline void inc_depth (active_point* ap);
    }


    namespace common_str {

        /**
         * @brief      Searches string number where this symbol were located
         *
         * @param[in]  sb    Strings bounds
         * @param[in]  ich   Symbol index
         *
         * @return     Number of the string
         */
        static inline unsigned  bsearch_str     (const bounds::str_bounds* sb, unsigned ich);
        
        /**
         * @brief      Uses dfs to find common substrings
         *
         * @param[in]  vert        Vertex for dfs
         * @param[in]  sb          Strings bounds
         * @param      cstrs       Common strings
         * @param[in]  text        Text on which tree was built
         * @param[in]  cur_length  Current length or dfs deep   
         * @param      contained   Array that contains data about lists in this vertex
         *
         * @return     Array that contains data about lists in this vertex -- contained
         */
        static inline unsigned* common_str_dfs  (const vertex* vert, const bounds::str_bounds* sb, substr** cstrs, const char* text, unsigned cur_length = 0, unsigned* contained = nullptr);
    }


    /**
     * @brief      Creates root and dummy
     *
     * @return     Root
     */
    static inline vertex* init_tree ();

    /**
     * @brief      Adds a string to the suffix tree
     *
     * @param[in]  root    Suffix tree root     
     * @param[in]  fst_ch  First index of the string
     * @param[in]  str     String
     */
    static inline void    add_str   (vertex* root, unsigned fst_ch, const char* str);

    /**
     * @brief      Tries to canonize active point
     *
     * @param[in]  ap    Currrent active point  
     * @param[in]  str   Current string on which tree is being built    
     *
     * @return     Suffix link for previous vertex
     */
    static inline vertex* canonize  (ap::active_point* ap, const char* str); 

    /**
     * @brief      Walks on suffix links and tries to add vertices
     *
     * @param[in]  ap    Current active point
     * @param[in]  str   Current string on which tree is being built
     */
    static inline void    update    (ap::active_point* ap, const char* str);

    /**
     * @brief      Adds new vertex, list or nothing. Decides when to stop current update
     *
     * @param[in]  ap         Current active point
     * @param[out] end_point  True if update must be completed
     * @param[in]  str        Current string on which tree is being built
     *
     * @return     New vertex if function created it, old vertex if function found it or nullptr 
     */
    static inline vertex* add       (ap::active_point* ap, bool& end_point, const char* str);


    namespace chld {

        static inline child* create (unsigned begin, unsigned end, vertex* chld) {
            child* ch  = new child;
            ch->to_    = chld;
            ch->begin_ = begin;
            ch->end_   = end;

            return ch;
        }


        static inline void add (char ch, vertex* vert, unsigned begin, unsigned end, vertex* chld_vert) {
            assert (vert);

            vert->children_.insert (std::make_pair (ch, create (begin, end, chld_vert)));
        }


        static inline child* find (char ch, const vertex* vert) {
            assert (vert);

            auto res = vert->children_.find (ch);
            if (res == vert->children_.end ())
                return nullptr;
            return res->second;
        }


    }


    namespace ap {

        static inline void set (active_point* ap, unsigned symb_idx, vertex* v, unsigned depth) {
            assert (v);
            assert (ap);

            ap->ch_idx_ = symb_idx;
            ap->vert_   = v;
            ap->depth_  = depth;
        }


        static inline void inc_depth (active_point* ap) {
            assert (ap);
            ap->depth_++;
        }
    }


    vertex* build (const char* text, const bounds::str_bounds* sb) {
        assert (text);

        vertex* root  = init_tree ();

		const unsigned* bounds = bounds::get_sb (sb);
        unsigned nstr = bounds::nstr (sb);

        for (unsigned i = 0; i < nstr; i++)
            add_str (root, bounds[i], text);

        return root;
    }


	static inline void add_str (vertex* root, unsigned fst_ch, const char* str) {
        ap::active_point* act_p = new ap::active_point;
        ap::set (act_p, fst_ch, root, ap::VERTEX);

        for (unsigned i = fst_ch; str[i] != '/'; i++) { 
            ap::inc_depth (act_p);
            canonize (act_p, str);
            update   (act_p, str);
        }
        ap::inc_depth (act_p);
        canonize (act_p, str);
        update   (act_p, str);

        delete act_p;
    }


    static inline vertex* init_tree () {
        vertex* dummy = new vertex;
        vertex* root  = new vertex;

        root->suffix_link_ = dummy;
        dummy->suffix_link_   = dummy;

        for (char ch = SCHAR_MIN; ch < SCHAR_MAX; ch++)
            chld::add (ch, dummy, 0, 1, root);

        return root;
    }


    static inline vertex* canonize (ap::active_point* ap, const char* str) {
        assert (ap);
        assert (str);

        unsigned& depth = ap->depth_;

        vertex* vert_for_suff_link = nullptr;
        while (depth != ap::VERTEX) {
            unsigned ich = ap->ch_idx_;
            chld::child* chld = chld::find (str[ich], ap->vert_);

            if (depth == ap::LIST)
                vert_for_suff_link = ap->vert_;
            if (chld && chld->end_ <= chld->begin_ + depth) {
                for (unsigned i = 1; i < chld->end_ - chld->begin_; i++)
                    if (str[i + chld->begin_] != str[i + ich])
                        return vert_for_suff_link;
                depth = depth - (chld->end_ - chld->begin_);
                ap->vert_ = chld->to_;
                ap->ch_idx_ = ich + chld->end_ - chld->begin_;
            } 
            else
                return vert_for_suff_link;
        }
        return vert_for_suff_link;
    }


    static inline void update (ap::active_point* ap, const char* str) {
        assert (ap);
        assert (str);

        vertex* prev_v = nullptr;
        bool end = false;

        while (1) {
            vertex* new_v = add (ap, end, str);

            if (prev_v && new_v)
                if (prev_v != ap->vert_ && ap->depth_ != 0)
                    prev_v->suffix_link_ = new_v;

            prev_v = new_v;

            if (end)
                break;

            ap->vert_ = ap->vert_->suffix_link_;
            vertex* sf_link = canonize (ap, str);
            if (sf_link && prev_v)
                prev_v->suffix_link_ = sf_link;
        }
    }


    static inline vertex* add (ap::active_point* ap, bool& end_point, const char* str) {
        assert (ap);
        assert (str);

        if (!ap->depth_) {
            end_point = true;
            return ap->vert_;
        }

        chld::child* chld = chld::find (str[ap->ch_idx_], ap->vert_);
        if (!chld) {
            if (ap->depth_ != ap::LIST) {
                ap->depth_--;
                ap->ch_idx_++;
            }
            chld::add (str[ap->ch_idx_], ap->vert_, ap->ch_idx_, UINT_MAX, nullptr);
            return nullptr;
        }
        
        unsigned depth = ap->depth_;
        assert (depth);
        depth--;
        if (str[chld->begin_ + depth] == str[ap->ch_idx_ + depth]) {
            end_point = true;
            return nullptr;
        }

        vertex* new_v = new vertex;
        new_v->suffix_link_ = nullptr;
        chld::add (str[chld->begin_ + depth], new_v, chld->begin_ + depth, chld->end_, chld->to_);
        chld::add (str[ap->ch_idx_ + depth], new_v, ap->ch_idx_ + depth, UINT_MAX, nullptr);
        chld->end_ = chld->begin_ + depth;
        chld->to_ = new_v;

        return  new_v;
    }


    void print (const vertex* root, const char* str) {
        assert (root);
        assert (str);

        FILE * dot_file = fopen ("dump_suffix_tree.dot", "wb");
        assert (dot_file);

        fprintf (dot_file, "digraph tree\n{\n");

        struct nvert {
            unsigned n_;
            const vertex* vert_;
        };

        unsigned counter = 0;
        std::list<nvert> verts {{counter, root}};

        while (!verts.empty ()) {
            nvert cur = verts.front ();
            verts.pop_front ();

            if (!cur.vert_) {
                fprintf (dot_file, "\t%u [label = \"\"]\n", cur.n_);
                fprintf (dot_file, "\t%u [shape = plaintext]\n", cur.n_);
                continue;
            }
            else 
                fprintf (dot_file, "\t%u [label = \"\"]\n", cur.n_);

            for (const auto &pair : cur.vert_->children_) {
                counter++;
                chld::child* chld = pair.second;
                fprintf (dot_file, "\t%u->%u [label =\"", cur.n_, counter);
                auto i = chld->begin_;
                for (; i < chld->end_ && str[i] != '/'; i++)
                    fprintf (dot_file, "%c", str[i]);
                if (i < chld->end_ && str[i] == '/')
                    fprintf (dot_file, "%c", str[i]);
                fprintf (dot_file, "\n[%u..%u)\"]\n", chld->begin_, chld->end_);
                verts.push_back ({counter, chld->to_});
            } 
        }

        fprintf (dot_file, "}\n");
        fclose (dot_file);
    }


    void clean (vertex* root) {
        if (!root)
            return;

        vertex* dummy = root->suffix_link_;
        for (const auto &pair : dummy->children_)
            delete pair.second;
        delete dummy;


        std::list<vertex*> verts {root};

        while (!verts.empty ()) {
            vertex* cur = verts.front ();
            verts.pop_front ();

            for (const auto &pair : cur->children_) {
                chld::child* chld = pair.second;
                if (chld->to_)
                    verts.push_back (chld->to_); 
                delete chld;
            }  

            delete cur;         
        }
    }


    bool test (const vertex* root, const char* str) {
        assert (root);
        assert (str);

        for (unsigned ich = 0; str[ich] != '\0'; ich++) {
            const vertex* vert = root;
            unsigned cur = ich;
            bool substr_done = false;
            while (!substr_done) {
                chld::child* chld = chld::find (str[cur], vert);
                if (!chld)
                    return false;

                for (auto i = chld->begin_; i < chld->end_; i++) {

                    if (str[cur] != str[i])
                        return false;

                    if (str[cur] == '\0') {
                        substr_done = true;
                        break;
                    }

                    cur++;
                }

                if (!substr_done)
                    vert = chld->to_;
            }
        }

        return true;
    }


    namespace bounds {
        str_bounds* set (unsigned* bounds, unsigned nstrings) {
            assert (bounds);

            str_bounds* sb = new str_bounds;
            sb->bounds_ = bounds;
            sb->nstr_ = nstrings;

            return sb;
        }


        void clean (str_bounds* sb) {
            assert (sb);

            delete[] sb->bounds_;
            delete sb;
        }


        unsigned nstr (const str_bounds* sb) {
            assert (sb);

            return sb->nstr_;
        }


        const unsigned* get_sb (const str_bounds* sb) {
            assert (sb);

            return sb->bounds_;
        }
    }


    namespace common_str {

        substr** find (const vertex* root, const bounds::str_bounds* sb, const char* text) {
            assert (root);
            assert (sb);
            assert (text);

            unsigned nstrings = bounds::nstr (sb);
            substr** sub_strings = new substr*[nstrings + 1];
            for (unsigned i = 0; i < nstrings + 1; i++) {
                unsigned def = 0;
                if (i == nstrings)
                    def = UINT_MAX;
                sub_strings[i] = new substr;
                sub_strings[i]->begin_ = def;
                sub_strings[i]->end_   = def;
                sub_strings[i]->rank_  = def;
            }

            delete[] common_str_dfs (root, sb, sub_strings, text);

            return sub_strings;
        }


        void clean (substr** common) {
            assert (common);

            unsigned i = 0;
            for (; common[i]->rank_ != UINT_MAX; i++)
                delete common[i];
            delete common[i];

            delete[] common;
        }


        static inline unsigned* common_str_dfs (const vertex* vert, const bounds::str_bounds* sb, substr** cstrs, const char* text, unsigned cur_length, unsigned* contained) {
            assert (vert);
            assert (sb);
            assert (cstrs);
            assert (text);

            unsigned nstr = bounds::nstr (sb);
            if (!contained)
                contained = new unsigned[nstr] ();

            chld::child* child_for_idx = nullptr;
            for (const auto &pair : vert->children_) {

                chld::child* chld = pair.second;
                child_for_idx = chld;

                if (chld->to_) {
                    unsigned* chld_contained = new unsigned[nstr] ();
                    chld_contained = common_str_dfs (chld->to_, sb, cstrs, text, cur_length + chld->end_ - chld->begin_, chld_contained);

                    for (unsigned i = 0; i < nstr; i++) 
                        contained[i] = contained[i] ? 1 : chld_contained[i];

                    delete[] chld_contained;
                }
                else {
                    unsigned edge_idx = bsearch_str (sb, chld->begin_);
                    contained[edge_idx] = 1;
                }
            }

            unsigned nstr_cont = 0;
            for (unsigned i = 0; i  < nstr; i++)
                if (contained[i])
                    nstr_cont++;

            for (unsigned i = 0; i < nstr && nstr_cont > i; i++) {
                unsigned commonstr_length = cstrs[i]->end_ - cstrs[i]->begin_;
                unsigned ich = child_for_idx->begin_;
                while (ich && (text[ich - 1] == '$' || text[ich - 1] == '/' || isdigit (text[ich - 1])))
                    ich--;
                if (cur_length < child_for_idx->begin_ - ich)
                    continue;
                unsigned substr_length = cur_length - (child_for_idx->begin_ - ich);
                if (commonstr_length < substr_length || (cstrs[i]->rank_ < nstr_cont && commonstr_length == substr_length)) {
                    cstrs[i]->rank_  = nstr_cont;
                    cstrs[i]->begin_ = ich - substr_length;
                    cstrs[i]->end_   = ich;
                }
            }

            return contained;
        }


        static inline unsigned bsearch_str (const bounds::str_bounds* sb, unsigned ich) {
            assert (sb);

            unsigned nstr = bounds::nstr (sb);
            const unsigned* bounds = bounds::get_sb (sb);

            if (ich == UINT_MAX)
                return nstr - 1;

            unsigned fst = 0u;
            unsigned lst = nstr - 1;
            unsigned mid = 0u;

            while (1) {
                mid = (lst + fst) / 2;

                if (bounds[mid] == ich)
                    return mid;

                if (bounds[mid] < ich) {
                    if (mid >= nstr - 1 || bounds[mid + 1] > ich)
                        return mid;

                    fst = mid + 1;
                }
                else {
                    if (bounds[mid - 1] < ich)
                        return mid - 1;

                    lst = mid - 1;
                }
            }

            return UINT_MAX;
        }


        unsigned fst_ch (substr* common) {
            assert (common);

            return common->begin_;
        }


        unsigned lst_ch (substr* common) {
            assert (common);

            return common->end_;
        }

    }

}

