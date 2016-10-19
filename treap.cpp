#include <cassert>
#include <cstdio>
#include <cstdint>
#include "treap.h"
#include "list.h"


namespace treap {

	struct treap_t {
		treap_t* parent_;
		treap_t* left_;
		treap_t* right_;

		int data_;
		unsigned idx_;
	};


	static inline void		write_dot_file		(FILE* file, const treap_t* treap);
	static inline treap_t*	get_vert_from_idx	(treap_t* treap, unsigned idx);


	treap_t* create_treap (const int* data, unsigned size) {
		assert (data);

		treap_t* prev = new treap_t;
		prev->data_ = data[0];
		prev->idx_  = 0;
		prev->parent_ = nullptr;
		prev->left_   = nullptr;
		prev->right_  = nullptr;
		for (unsigned i = 1; i < size; i++) {
			treap_t* vertex = new treap_t;
			vertex->data_ = data[i];
			vertex->idx_  = i;
			vertex->left_  = nullptr;
			vertex->right_ = nullptr;

			if (data[i] >= prev->data_) {
				prev->right_ = vertex;
				vertex->parent_ = prev;
			}
			else {
				while (prev->parent_ && data[i] < prev->parent_->data_) 
					prev = prev->parent_;

				vertex->parent_ = prev->parent_;
				vertex->left_   = prev;
				if (prev->parent_)
					prev->parent_->right_ = vertex;
				prev->parent_ = vertex;
			}

			prev = vertex;
		}

		while (prev->parent_)
			prev = prev->parent_;

		return prev;
	}


	void printin_dot (const treap_t* treap) {
		if (!treap)
			printf ("\n\n printin_dot (), treap is null\n\n");

		FILE * dot_file = fopen ("dump_treap.dot", "wb");
		assert (dot_file);

		fprintf (dot_file, "digraph tree\n{\n");
			write_dot_file (dot_file, treap);

		fprintf (dot_file, "}\n");

    	fclose (dot_file);
	}


	static inline void write_dot_file (FILE* file, const treap_t* treap) {
		assert (file);
		assert (treap);

		static long long int null_counter = -1;

		if (treap->left_) {
			fprintf (file, "\t%lu->%lu\n", reinterpret_cast<unsigned long> (treap), reinterpret_cast<unsigned long> (treap->left_));
			write_dot_file (file, treap->left_);
		} else {
			fprintf (file, "\t%lu->%lld\n", reinterpret_cast<unsigned long> (treap), null_counter);
			fprintf (file, "\t%lld [label = \"null\"]\n", null_counter--);
		}

		if (treap->right_) {
			fprintf (file, "\t%lu->%lu\n", reinterpret_cast<unsigned long> (treap), reinterpret_cast<unsigned long> (treap->right_));
			write_dot_file (file, treap->right_);
		} else {
			fprintf (file, "\t%lu->%lld\n", reinterpret_cast<unsigned long> (treap), null_counter);
			fprintf (file, "\t%lld [label = \"null\"]\n", null_counter--);
		}

		fprintf (file, "\t%lu [label = \"data = %d\nidx = %u\"]\n", reinterpret_cast<unsigned long> (treap), treap->data_, treap->idx_);
	}


	void delete_treap (treap_t* treap) {
		assert (treap);

		using namespace list;

		node_t* in  = create_or_push_front (nullptr, treap);
		node_t* out = nullptr;

		while ( ! is_empty (in) || ! is_empty (out)) {
			if (is_empty (out)) {
				out = copy (in);
				delete_list (in);
				in = nullptr;
			}

			const treap_t* cur = static_cast<const treap_t*> (pop_front (&out));

			if (cur->left_)
				in = create_or_push_front (in, cur->left_);
			if (cur->right_)
				in = create_or_push_front (in, cur->right_);

			delete cur;
		}	
	}


	int find_min (treap_t* treap, unsigned fst, unsigned snd) {
		assert (treap);

		treap_t* tfst = get_vert_from_idx (treap, fst);
		treap_t* tsnd = get_vert_from_idx (treap, snd);

		treap_t* cur = tfst;
		while (cur) {
			treap_t* next = cur->parent_;
			cur->parent_ = reinterpret_cast<treap_t*> (reinterpret_cast<intptr_t> (cur->parent_) | 1);
			cur = next;
		}

		const treap_t* common_vertex = tsnd;
		while (common_vertex && ! (reinterpret_cast<intptr_t> (common_vertex->parent_) & 1)) 
			common_vertex = common_vertex->parent_;

		cur = tfst;
		while (cur && (reinterpret_cast<intptr_t> (cur->parent_) & 1)) {
			cur->parent_ = reinterpret_cast<treap_t*> (reinterpret_cast<intptr_t> (cur->parent_) & ~1);
			cur = cur->parent_;
		}

		return common_vertex->data_;
	}


	static inline treap_t* get_vert_from_idx (treap_t* treap, unsigned idx) {
		assert (treap);

		while (treap->parent_)
			treap = treap->parent_;

		while (treap->idx_ != idx) {
			if (treap->idx_ >= idx)
				treap = treap->left_;
			else
				treap = treap->right_;
		}

		return treap;
	}
}