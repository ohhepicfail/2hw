#include <cassert>
#include <cstdio>
#include <cstdint>
#include <ctime>
#include <cstdlib>
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


	namespace query {
		struct query_t {
			unsigned fst_;
			unsigned snd_;
			int ans_;
		};
	}


	namespace dsf {
		static void 	make_set 	  (treap_t* vert);
		static void 	union_by_rank (treap_t* v1, treap_t* v2);
		static treap_t* find 	 	  (treap_t* vert);
	}


	static inline void	   write_dot_file		(FILE* file, const treap_t* treap);
	static inline treap_t* get_vert_from_idx	(treap_t* treap, unsigned idx);
	static 		  void	   dfs_olca	   			(treap_t* treap, query::query_t* q, unsigned size, int* lca);



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


	int find_min (treap_t* treap, const query::query_t& q) {
		assert (treap);

		treap_t* tfst = get_vert_from_idx (treap, q.fst_);
		treap_t* tsnd = get_vert_from_idx (treap, q.snd_);

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


	void tarjan_olca (treap_t* treap, query::query_t* q, unsigned size) {
		assert (treap);
		assert (q);

		std::clock_t start;
		start = std::clock();

		query::sort (q, size);

		double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		printf ("Sort time %lf\n", duration);
		
		start = std::clock ();
		int* lca = new int[size];
		dfs_olca (treap, q, size, lca);

		duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		printf ("Tarjan time %lf\n", duration);
		

		start = std::clock();
		printf ("test answers\n");
		for (unsigned i = 0; i < size; i++) {
			int min = find_min (treap, q[i]);
			if (min != q[i].ans_) {
				printf ("%u\t%d\t%d\n", i, min, q[i].ans_);
				assert (min == q[i].ans_);
			}
		}
		duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		printf ("Treap time %lf\n", duration);
		printf ("successful\n");

		delete[] lca;
	}


	void dfs_olca (treap_t* treap, query::query_t* q, unsigned size, int* lca) {
		assert (treap);
		assert (q);
		assert (lca);

		using namespace dsf;

		make_set (treap);
		lca[treap->idx_] = treap->data_;
		#define DFS_OLCA( x )	dfs_olca (x, q, size, lca)
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

		long cur = query::binary_search (q, treap->idx_, size);
		for (;cur >= 0 && cur < size && q[cur].fst_ == treap->idx_; cur++)
			q[cur].ans_ = lca[find (get_vert_from_idx (treap, q[cur].snd_))->idx_];
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


	namespace query {

		query_t* create (unsigned size) {
			query_t* q = new query_t[size];

			return q;
		}


		void set (query_t* q, unsigned idx, unsigned value1, unsigned value2) {
			assert (q);
			if (value1 < value2) {
				unsigned tmp = value1;
				value1 = value2;
				value2 = tmp;
			}

			q[idx].fst_ = value1;
			q[idx].snd_ = value2;
		}


		int get_answer (query_t* q, unsigned idx) {
			assert (q);

			return q[idx].ans_;
		}


		void print (query_t* q, unsigned size) {
			for (unsigned i = 0; i < size; i++)
				printf ("(%u,%u)\t", q[i].fst_, q[i].snd_);
			printf ("\n");
		}


		void delete_query (query_t* q) {
			delete[] q;
		}


		int sort_cmp (const void * x1, const void * x2) {
			const query_t* q1 = static_cast<const query_t*> (x1);
			const query_t* q2 = static_cast<const query_t*> (x2);
			if (q1->fst_ == q2->fst_) {
				if (q1->snd_ == q2->snd_)
					return 0;
				if (q1->snd_ < q2->snd_)
					return -1;
				return 1;

			}
			if (q1->fst_ < q2->fst_)
				return -1;
			return 1;
		}


		void sort (query_t* q, unsigned size) {
			assert (q);

			qsort (q, size, sizeof (query_t), sort_cmp);
		}

		int bsearch_cmp (const void * x1, const void * x2) {
			const query_t* q1 = static_cast<const query_t*> (x1);
			const query_t* q2 = static_cast<const query_t*> (x2);
			if (q1->fst_ == q2->fst_)
				return 0;
			if (q1->fst_ < q2->fst_)
				return -1;
			return 1;
		}


		long binary_search (query_t* q, unsigned val, unsigned size) {
			assert (q);

			query_t qval = {val, 0, 0};
			query_t* item = static_cast<query_t*> (bsearch (&qval, q, size, sizeof (query_t), bsearch_cmp));
			if (!item)
				return -1;
			long idx = (item - q);
			while (idx > 0 && q[idx].fst_ == q[idx - 1].fst_)
				idx--;
			return idx;
		}
	}
}