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

		treap_t* lcaparent_;
		unsigned rank_;
	};


	static inline void				write_dot_file		(FILE* file, const treap_t* treap);
	static inline treap_t*			get_vert_from_idx	(treap_t* treap, unsigned idx);
	static 		  void				dfs_olca	   		(treap_t* treap, unsigned* fst, unsigned* snd, int* answers, unsigned size, int* lca);
	static 		  void 				dsf_make_set		(treap_t* vert);
	static 		  void 				dsf_union			(treap_t* v1, treap_t* v2);
	static  	  treap_t*			dsf_find			(treap_t* vert);



	treap_t* create_treap (const int* data, unsigned size) {
		assert (data);
		list::turn_off_loop_check ();

		treap_t* prev = new treap_t;
		prev->data_ = data[0];
		prev->idx_  = 0;
		prev->rank_ = 0;
		prev->parent_ 	 = nullptr;
		prev->left_   	 = nullptr;
		prev->right_  	 = nullptr;
		prev->lcaparent_ = nullptr;
		for (unsigned i = 1; i < size; i++) {
			treap_t* vertex = new treap_t;
			vertex->data_ = data[i];
			vertex->idx_  = i;
			vertex->rank_ = 0;
			vertex->left_  	   = nullptr;
			vertex->right_ 	   = nullptr;
			vertex->lcaparent_ = nullptr;

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


	void tarjan_olca (treap_t* treap, unsigned* fst, unsigned* snd, int* answers, unsigned size) {
		assert (treap);
		assert (fst);
		assert (snd);
		assert (answers);

		// todo: sort_requests (fst, snd);
		for (unsigned i = 0; i < size; i++) {
			if (fst[i] < snd[i]) {
				unsigned tmp = fst[i];
				fst[i] = snd[i];
				snd[i] = tmp;
			}
		}

		int* lca = new int[size];
		dfs_olca (treap, fst, snd, answers, size, lca);

		printin_dot (treap);

		for (unsigned i = 0; i < size; i++) {
			// printf ("idxs: %u -- %u\ttarjan olca %d\n",fst[i], snd[i], answers[i]);
			int min = find_min (treap, fst[i], snd[i]);
			// printf ("treap lca %d\n", min); 
			assert (min == answers[i]);
		}

		// todo: check and print

		delete[] lca;

	}


	void dfs_olca (treap_t* treap, unsigned* fst, unsigned* snd, int* answers, unsigned size, int* lca) {
		assert (treap);
		assert (fst);
		assert (snd);
		assert (answers);
		assert (lca);

		dsf_make_set (treap);
		lca[treap->idx_] = treap->data_;
		#define DFS_OLCA( x )	dfs_olca (x, fst, snd, answers, size, lca)
		if (treap->left_) {
			DFS_OLCA (treap->left_);
			dsf_union (treap, treap->left_);
			lca[dsf_find (treap)->idx_] = treap->data_;
		}
		if (treap->right_) {
			DFS_OLCA (treap->right_);
			dsf_union (treap, treap->right_);
			lca[dsf_find (treap)->idx_] = treap->data_;
		}
		#undef DFS_OLCA

		for (unsigned i = 0; i < size; i++)
			if (fst[i] == treap->idx_) {
				// printf ("saving answers vertex_idx - %u\tidxs %u -- %u\n\t", treap->idx_, fst[i], snd[i]);
				// printf ("fst");
				// for (unsigned i = 0; i < size; i++)
				// 	printf ("  %u", fst[i]);
				// printf ("\n\tsnd");
				// for (unsigned i = 0; i < size; i++)
				// 	printf ("  %u", snd[i]);
				// printf ("\n\tlca");
				// for (unsigned i = 0; i < size; i++)
				// 	printf ("  %d", lca[i]);
				// printf ("\n");
				treap_t* vert = get_vert_from_idx (treap, snd[i]);
				assert (vert);
				// printf ("\tvert idx %u\n\n", vert->idx_); 
				answers[i] = lca[dsf_find (vert)->idx_];

			}
	}


	static void dsf_make_set (treap_t* vert) {
		assert (vert);

		vert->lcaparent_ = vert;
		vert->rank_ = 0;
	}


	static void dsf_union (treap_t* v1, treap_t* v2) {
		assert (v1);
		assert (v2);

		treap_t* v1_root = dsf_find (v1);
		treap_t* v2_root = dsf_find (v2);

		if 		(v1_root->rank_ > v2_root->rank_)
			v2_root->lcaparent_ = v1_root;
		else if (v1_root->rank_ < v2_root->rank_)
			v1_root->lcaparent_ = v2_root;
		else {
			v2_root->lcaparent_ = v1_root;
			v1_root->rank_++;
		}
	}


	static treap_t* dsf_find (treap_t* vert) {
		assert (vert);

		if (vert->lcaparent_ != vert)
			vert->lcaparent_ = dsf_find (vert->lcaparent_);

		return vert->lcaparent_;
	}
}