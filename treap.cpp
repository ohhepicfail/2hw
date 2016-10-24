#include <cassert>
#include <cstdio>
#include <cstdint>
#include "treap.h"
#include "queue.h"


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


	namespace dsf {
	static void 	make_set 	  (treap_t* vert);
	static void 	union_by_rank (treap_t* v1, treap_t* v2);
	static treap_t* find 	 	  (treap_t* vert);
	}


	static inline void	   write_dot_file		(FILE* file, const treap_t* treap);
	static inline treap_t* get_vert_from_idx	(treap_t* treap, unsigned idx);
	static 		  void	   dfs_olca	   			(treap_t* treap, unsigned* fst, unsigned* snd, int* answers, unsigned size, int* lca);
	static 		  void 	   sort_requests		(unsigned* fst, unsigned* snd, unsigned size);
	static 		  void 	   quick_sort			(unsigned* a, unsigned* b, unsigned n);
	static 		  long 	   binarysearch 		(unsigned val, unsigned* array, unsigned n);



	treap_t* create_treap (const int* data, unsigned size) {
		assert (data);

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

		using namespace queue;

		queue_t* verts = create_or_push (nullptr, treap);

		while (!is_empty (verts)) {
			const treap_t* cur = static_cast<const treap_t*> (pop (verts));
			if (cur->left_)
				verts = create_or_push (verts, cur->left_);
			if (cur->right_)
				verts = create_or_push (verts, cur->right_);

			delete cur; 
		}

		delete_queue (verts);
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


	static void quick_sort (unsigned* a, unsigned* b, unsigned n) {
		assert (a);
		assert (b);
	 
	    long i = 0;
	    long j = n;
	 
	    unsigned p = a[n >> 1];

	    do {
	        while (i < n && a[i] < p) i++;
	        while (j > 0 && a[j] > p) j--;
	 
	        if (i <= j) {
	            unsigned tmp = a[i]; a[i] = a[j]; a[j] = tmp;
	            		 tmp = b[i]; b[i] = b[j]; b[j] = tmp;
	            i++; j--;
	        }
	    } while (i <= j);
	 
	    if (j > 0) 
	    	quick_sort (a, b, j);
	    if (n > i) 
	    	quick_sort (a + i, b + i, n - i);
	}


	static long binarysearch (unsigned val, unsigned* array, unsigned n) {
		assert (array);

	    long low = 0;
	    long high = static_cast<long> (n - 1);
	    long middle = 0;

	    while (low <= high) {
	        middle = (low + high) / 2;
	        if (val < array[middle])
	            high = middle - 1;
	        else if (val > array[middle])
	            low = middle + 1;
	        else {
	        	while (middle > 0 && val == array[middle-1])
	        		middle--;
	            return static_cast<unsigned> (middle);
	        }
	    }
	    return -1;
	}

	static void sort_requests (unsigned* fst, unsigned* snd, unsigned size) {
		assert (fst);
		assert (snd);

		for (unsigned i = 0; i < size; i++) {
			if (fst[i] < snd[i]) {
				unsigned tmp = fst[i];
				fst[i] = snd[i];
				snd[i] = tmp;
			}
		}

		quick_sort (fst, snd, size - 1);
	}


	void tarjan_olca (treap_t* treap, unsigned* fst, unsigned* snd, int* answers, unsigned size) {
		assert (treap);
		assert (fst);
		assert (snd);
		assert (answers);

		sort_requests (fst, snd, size);
		
		int* lca = new int[size];
		dfs_olca (treap, fst, snd, answers, size, lca);

		printf ("test answers\n");
		for (unsigned i = 0; i < size; i++) {
			int min = find_min (treap, fst[i], snd[i]);
			if (min != answers[i]) {
				printf ("%u\t%d\t%d\n", i, min, answers[i]);
				assert (min == answers[i]);
			}
		}
		printf ("successful\n");

		delete[] lca;
	}


	void dfs_olca (treap_t* treap, unsigned* fst, unsigned* snd, int* answers, unsigned size, int* lca) {
		assert (treap);
		assert (fst);
		assert (snd);
		assert (answers);
		assert (lca);

		using namespace dsf;

		make_set (treap);
		lca[treap->idx_] = treap->data_;
		#define DFS_OLCA( x )	dfs_olca (x, fst, snd, answers, size, lca)
		if (treap->left_) {
			DFS_OLCA (treap->left_);
			union_by_rank (treap, treap->left_);
			lca[find (treap)->idx_] = treap->data_;
		}
		if (treap->right_) {
			DFS_OLCA (treap->right_);
			union_by_rank (treap, treap->right_);
			lca[find (treap)->idx_] = treap->data_;
		}
		#undef DFS_OLCA

		long cur = binarysearch (treap->idx_, fst, size);
		for (;cur >= 0 && cur < size && fst[cur] == treap->idx_; cur++)
			answers[cur] = lca[find (get_vert_from_idx (treap, snd[cur]))->idx_];
	}


	namespace dsf {

		static void make_set (treap_t* vert) {
			assert (vert);

			vert->lcaparent_ = vert;
			vert->rank_ = 0;
		}


		static void union_by_rank (treap_t* v1, treap_t* v2) {
			assert (v1);
			assert (v2);

			treap_t* v1_root = find (v1);
			treap_t* v2_root = find (v2);

			if 		(v1_root->rank_ > v2_root->rank_)
				v2_root->lcaparent_ = v1_root;
			else if (v1_root->rank_ < v2_root->rank_)
				v1_root->lcaparent_ = v2_root;
			else {
				v2_root->lcaparent_ = v1_root;
				v1_root->rank_++;
			}
		}


		static treap_t* find (treap_t* vert) {
			assert (vert);

			if (vert->lcaparent_ != vert)
				vert->lcaparent_ = find (vert->lcaparent_);

			return vert->lcaparent_;
		}
	}
}