#include <cassert>
#include <climits>
#include <unordered_map>
#include <list>
#include "ukk.h"

namespace suffix_tree {

    namespace chld {
        struct child {
            unsigned begin_;
            unsigned end_;
            vertex* to_;
        };
    }


    struct vertex {
        std::unordered_map<char, chld::child*> childs_;
        vertex* suffix_link_;
    };

    namespace ap {
        struct active_point {
            unsigned ch_idx_;
            vertex* vert_;
            unsigned depth_;
        };

        enum depth_key {
            VERTEX = 0,
            LIST = 1
        };
    }


    namespace chld {
        static inline void add (char ch, vertex* vert, unsigned begin, unsigned end, vertex* chld_vert); 
        static inline child* find (char ch, vertex* vert);
        static inline child* create (unsigned begin, unsigned end, vertex* child);
    }


    namespace ap {
        static inline void set (active_point* ap, unsigned symb_idx, vertex* v, unsigned depth);
        static inline void inc_depth (active_point* ap, unsigned symb_idx);
    }


    static inline vertex* init_tree ();
    static inline void canonize (ap::active_point* ap, const char* str); 
    static inline void update (ap::active_point* ap, const char* str);
    static inline vertex* add (ap::active_point* ap, bool* end_point, const char* str);


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

            vert->childs_.insert (std::make_pair (ch, create (begin, end, chld_vert)));
        }


        static inline child* find (char ch, vertex* vert) {
            assert (vert);

            auto res = vert->childs_.find (ch);
            if (res == vert->childs_.end ())
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


        static inline void inc_depth (active_point* ap, unsigned symb_idx) {
            assert (ap);

            // if (ap->depth_ == VERTEX)
            //     ap->ch_idx_ = symb_idx;
            ap->depth_++;
        }
    }


    vertex* build (const char* text) {
        assert (text);

        vertex* root  = init_tree ();

        ap::active_point* act_p = new ap::active_point;
        ap::set (act_p, 0, root, ap::VERTEX);
        unsigned i = 0;
        for (; text[i] != '\0'; i++) { 
            ap::inc_depth (act_p, i);
            canonize (act_p, text);
            update   (act_p, text);
        }
        print (root, text);
        printf ("depth %u  |  ich %u %c |  root %d  |  vert %u\n", act_p->depth_, act_p->ch_idx_, text[act_p->ch_idx_], root, act_p->vert_);
        ap::inc_depth (act_p, i);
        canonize (act_p, text);
        printf ("ok\n");
        printf ("depth %u  |  ich %u %c |  root %d  |  vert %u\n", act_p->depth_, act_p->ch_idx_, text[act_p->ch_idx_], root, act_p->vert_);
        update   (act_p, text);

        delete act_p;
        return root;
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


    static inline void canonize (ap::active_point* ap, const char* str) {
        assert (ap);
        assert (str);

        unsigned& depth = ap->depth_;

        // if (depth == ap::LIST && ap->vert_ && ap->vert_->childs_.size () == 255) {          //freak!!!!!!!!!!!!!!!!!
        //     ap->depth_ = ap::VERTEX;
        //     ap->vert_ = ap->vert_->childs_.find ('a')->second->to_;

        // }
        while (depth != ap::VERTEX /*&& depth != ap::LIST*/) {
            unsigned ich = ap->ch_idx_;
            chld::child* chld = chld::find (str[ich], ap->vert_);

            /*if (!chld) {
                ap->ch_idx_++;
                ap->depth_--;
                return;
            }
            else*/ if (chld && chld->end_ <= chld->begin_ + depth) {
                for (unsigned i = 1; i < chld->end_ - chld->begin_; i++)
                    if (str[i + chld->begin_] != str[i + ich])
                        return;
                depth = depth - (chld->end_ - chld->begin_);
                ap->vert_ = chld->to_;
                ap->ch_idx_ = ich + chld->end_ - chld->begin_;
            } 
            else
                return;
        }
    }


    static inline void update (ap::active_point* ap, const char* str) {
        assert (ap);
        assert (str);

        vertex* prev_v = nullptr;
        bool end = false;

        while (1) {
            vertex* new_v = add (ap, &end, str);

            printf ("\ndepth %u  |  ich %u %c |  vert %u\n", ap->depth_, ap->ch_idx_, str[ap->ch_idx_], ap->vert_);
            if (new_v)
                printf ("vertex created\n");
             if (!new_v)
                printf ("vertex did't create\n");
            if (prev_v) {
                if (!new_v /*&& prev_v != ap->vert_*/)                  /// ??????????????????//
                    new_v = ap->vert_;
                prev_v->suffix_link_ = new_v;
            }

            prev_v = new_v;

            if (end)
                break;

            ap->vert_ = ap->vert_->suffix_link_;
            printf ("\tsuffix link %u\n", ap->vert_);

            // if (prev_v)
            //     prev_v->suffix_link_ = ap->vert_;           ///// ???????????????????????????
            canonize (ap, str);
        }
    }


    static inline vertex* add (ap::active_point* ap, bool* end_point, const char* str) {
        assert (ap);
        assert (end_point);
        assert (str);

        if (!ap->depth_) {
            // ap->depth_++;
            *end_point = true;
            return ap->vert_;
        }

        chld::child* chld = chld::find (str[ap->ch_idx_], ap->vert_);
        if (!chld) {
            printf ("depth %u\tich %u\n", ap->depth_, ap->ch_idx_);
            // assert (ap->depth_ == ap::LIST);
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
            *end_point = true;
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

            for (const auto &pair : cur.vert_->childs_) {
                counter++;
                chld::child* chld = pair.second;
                printf ("%c\t", pair.first);
                printf ("\t%u->%u [label = \"[%u...%u)\"]\t%lu\t", cur.n_, counter, chld->begin_, chld->end_, chld->to_);
                if (chld->to_)
                    printf ("suffix_link %u\n", chld->to_->suffix_link_);
                else
                    printf ("\n");
                fprintf (dot_file, "\t%u->%u [label =\"", cur.n_, counter);
                for (auto i = chld->begin_; i < chld->end_ && str[i] != '\0'; i++)
                     fprintf (dot_file, "%c", str[i]);
                  fprintf (dot_file, "\n[%u..%u)\"]\n", chld->begin_, chld->end_);
                // fprintf (dot_file, "\t%u->%u [label = \"[%u...%u)\"]\n", cur.n_, counter, chld->begin_, chld->end_);
                verts.push_back ({counter, chld->to_});
            } 
        }

        fprintf (dot_file, "}\n");
        fclose (dot_file);
    }


    void trysuff () {
        vertex* v1 = new vertex;
        vertex* v2 = new vertex;
        chld::child* ch1 = chld::create (1, 9, v2);
        chld::child* ch2 = chld::create (2, 11, nullptr);
        chld::child* ch3 = chld::create (0, 3, nullptr);
        v1->childs_.insert (std::make_pair ('a', ch1));
        v1->childs_.insert (std::make_pair ('s', ch2));
        ch1->to_->childs_.insert (std::make_pair ('d', ch3));

        print (v1, "hello");
    }
}

