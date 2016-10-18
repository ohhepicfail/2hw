#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include "list.h"
#include "tree.h"

namespace tree {

	struct tree_t {
		tree_t* parent_;
		tree_t* left_;
		tree_t* right_;

		int data_;
	};


	static inline void		write_dot_file (FILE* file, const tree_t* tree);
	static inline unsigned	length_to_root (const tree_t* ptr);



	tree_t* create_tree (tree_t* parent, int data) {
		tree_t* new_tree = new tree_t;

		new_tree->parent_ = parent;
		new_tree->left_   = nullptr;
		new_tree->right_  = nullptr;
		new_tree->data_   = data;

		return new_tree;
	}


	void delete_tree (tree_t* tree) {
		assert (tree);

		using namespace list;

		node_t* in  = create_or_push_front (nullptr, tree);
		node_t* out = nullptr;

		while ( ! is_empty (in) || ! is_empty (out)) {
			if (is_empty (out)) {
				out = copy (in);
				delete_list (in);
				in = nullptr;
			}

			const tree_t* cur = static_cast<const tree_t*> (pop_front (&out));

			if (cur->left_)
				in = create_or_push_front (in, cur->left_);
			if (cur->right_)
				in = create_or_push_front (in, cur->right_);

			delete cur;
		}	
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
			fprintf (file, "\t%lu->%lu\n", reinterpret_cast<unsigned long> (tree), reinterpret_cast<unsigned long> (tree->left_));
			write_dot_file (file, tree->left_);
		} else {
			fprintf (file, "\t%lu->%lld\n", reinterpret_cast<unsigned long> (tree), null_counter);
			fprintf (file, "\t%lld [label = \"null\"]\n", null_counter--);
		}

		if (tree->right_) {
			fprintf (file, "\t%lu->%lu\n", reinterpret_cast<unsigned long> (tree), reinterpret_cast<unsigned long> (tree->right_));
			write_dot_file (file, tree->right_);
		} else {
			fprintf (file, "\t%lu->%lld\n", reinterpret_cast<unsigned long> (tree), null_counter);
			fprintf (file, "\t%lld [label = \"null\"]\n", null_counter--);
		}

		fprintf (file, "\t%lu [label = \"%d\"]\n", reinterpret_cast<unsigned long> (tree), tree->data_);
	}


	void get_2addresses (const tree_t* tree, const tree_t** fst, const tree_t** snd) {
		assert (tree);
		assert (fst);
		assert (snd);

		srand (time (nullptr));

		const tree_t* cur = tree;
		for (*fst = nullptr; ! *fst;) {
			int way = rand () % 400;

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
			int way = rand () % 400;

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


	int calculate_length (const tree_t* fst, const tree_t* snd) {
		assert (fst);
		assert (snd);

		if (fst == snd)
			return 0;

		unsigned l1 = length_to_root (fst);
		unsigned l2 = length_to_root (snd);

		int length = 0;
		while (l1 > l2) {
			length++;
			l1--;
			fst = fst->parent_;
		}
		while (l1 < l2) {
			length++;
			l2--;
			snd = snd->parent_;
		}

		while (l1 > 0 && fst != snd) {
			length += 2;
			l1--;
			fst = fst->parent_;
			snd = snd->parent_;
		}

		if (fst != snd)
			return -1;
		return length;
	}


	static inline unsigned	length_to_root (const tree_t* ptr) {
		assert (ptr);

		unsigned length = 0;
		while (ptr->parent_) {
			ptr = ptr->parent_;
			length++;
		}

		return length;
	}


	int length_bitlabel (tree_t* fst, tree_t* snd) {
		assert (fst);
		assert (snd);

		tree_t* cur = fst;
		while (cur) {
			tree_t* next = cur->parent_;
			cur->parent_ = reinterpret_cast<tree_t*> (reinterpret_cast<intptr_t> (cur->parent_) | 1);
			cur = next;
		}

		int length = 0;
		const tree_t* common_vertex = snd;
		for (; common_vertex && ! (reinterpret_cast<intptr_t> (common_vertex->parent_) & 1); common_vertex = common_vertex->parent_)
			length++; 

		cur = fst;
		bool flag = false;
		while (cur && (reinterpret_cast<intptr_t> (cur->parent_) & 1)) {
			if (cur == common_vertex)
				flag = true;
			if (!flag)
				length++;
			cur->parent_ = reinterpret_cast<tree_t*> (reinterpret_cast<intptr_t> (cur->parent_) & ~1);
			cur = cur->parent_;
		}

		if (!flag)
			return -1;

		return length;
	}
}