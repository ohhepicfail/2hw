#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "list.h"
#include "tree.h"

namespace tree {

	struct tree_t {
		tree_t* parent_;
		tree_t* left_;
		tree_t* right_;

		int data_;
	};


	static inline void write_dot_file (FILE* file, const tree_t* tree);



	tree_t* create_tree (tree_t* parent, int data) {
		tree_t* new_tree = new tree_t;

		if (parent)
			new_tree->parent_ = parent;
		else
			new_tree->parent_ = nullptr;
		new_tree->left_   = nullptr;
		new_tree->right_  = nullptr;
		new_tree->data_   = data;

		return new_tree;
	}


	void delete_tree (tree_t* tree) {
		assert (tree);

		if (tree->left_)
			delete_tree (tree->left_);
		if (tree->right_)
			delete_tree (tree->right_);

		tree->left_  = nullptr;
		tree->right_ = nullptr;

		delete tree;		
	}


	void add (tree_t* tree, int data) {
		assert (tree);

		if (tree->data_ >= data) {
			if (tree->left_)
				add (tree->left_, data);
			else
				tree->left_ = create_tree (tree, data);
		} else {
			if (tree->right_)
				add (tree->right_, data);
			else
				tree->right_ = create_tree (tree, data);

		}
	}


	void print_tree (const tree_t* tree) {
		if ( ! tree) {
			printf (" ( nullptr )");
			return;
		}

		printf (" ( %d ", tree->data_);
		print_tree (tree->left_);
		print_tree (tree->right_);
		printf (")");

		if (tree->parent_ == nullptr)
			printf ("\n");

	}


	void printin_dot (const tree_t* tree) {
		if ( !  tree)
			printf ("\n\n printin_dot (), tree is null\n\n");

		FILE * dot_file = fopen ("dump.dot", "wb");
		assert (dot_file);

		fprintf (dot_file, "digraph tree\n{\n");
			write_dot_file (dot_file, tree);

		fprintf (dot_file, "}\n");

    	fclose (dot_file);
	}


	static inline void write_dot_file (FILE* file, const tree_t* tree) {
		assert (file);
		assert (tree);

		static long long int null_counter = -1;

		if (tree->left_) {
			fprintf (file, "\t%u->%u\n", tree, tree->left_);
			write_dot_file (file, tree->left_);
		} else {
			fprintf (file, "\t%u->%lld\n", tree, null_counter);
			fprintf (file, "\t%lld [label = \"null\"]\n", null_counter--);
		}

		if (tree->right_) {
			fprintf (file, "\t%u->%u\n", tree, tree->right_);
			write_dot_file (file, tree->right_);
		} else {
			fprintf (file, "\t%u->%lld\n", tree, null_counter);
			fprintf (file, "\t%lld [label = \"null\"]\n", null_counter--);
		}

		fprintf (file, "\t%u [label = \"%d\"]\n", tree, tree->data_);
	}


	void get_2addresses (const tree_t* tree, const tree_t** fst, const tree_t** snd) {
		assert (tree);
		assert (fst);
		assert (snd);

		srand (time (nullptr));

		const tree_t* cur = tree;
		for (*fst = nullptr; ! *fst;) {
			int way = rand () % 20;

			if (way == 0)
				*fst = cur;
			else if (way % 2 == 0 && cur->left_)
				cur = cur->left_;
			else if (way % 2 == 1 && cur->right_)
				cur = cur->right_;
			else
				*fst = cur;
		}
		cur = tree;
		for (*snd = nullptr; ! *snd;) {
			int way = rand () % 20;

			if (way == 0)
				*snd = cur;
			else if (way % 2 == 0 && cur->left_)
				cur = cur->left_;
			else if (way % 2 == 1 && cur->right_)
				cur = cur->right_;
			else
				*snd = cur;
		}
	}


	int calculate_lengh (const tree_t* fst, const tree_t* snd) {
		assert (fst);
		assert (snd);

		if (fst == snd)
			return 0;

		using namespace list;

		node_t* flist = create_list (static_cast <const void*> (fst));
		node_t* slist = create_list (static_cast <const void*> (snd));

		const tree_t* cur = fst;
		while (cur) {
			push_front (&flist, cur);
			cur = cur->parent_;
		}
		cur = snd;
		while (cur) {
			push_front (&slist, cur);
			cur = cur->parent_;
		}

		unsigned counter = 0;
		while (!is_empty (flist) && !is_empty (slist) && pop_front (&flist) == pop_front (&slist))
			counter++;

		if (counter == 0)
			return -1;

		counter = 0;
		if (is_empty (flist) || is_empty (slist))
			counter++;

		while (!is_empty (flist)) {
			pop_front (&flist);
			counter++;
		}
		while (!is_empty (slist)) {
			pop_front (&slist);
			counter++;
		}

		return counter;
	}
}