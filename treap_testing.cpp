#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "treap.h"

#define PRINT 0

using namespace treap;

int main () {
	const unsigned n = 10000;
	int* data = new int[n];

	srand (time (nullptr));
	for (unsigned i = 0; i < n; i++)
		data[i] = rand () % n;

	treap_t* treap = create_treap (data, n);

	#if PRINT
		for (unsigned i = 0; i < n; i++)
			printf ("%d\t", data[i]);
		printin_dot (treap);
	#endif

	printf ("begin\n");
	unsigned nrequest = n*1000;
	query::query_t* q = query::create (nrequest);
	for (unsigned i = 0; i < nrequest; i++)
		query::set (q, i, rand () % n, rand () % n);

	tarjan_olca (treap, q, nrequest);

	printf ("end\n");
	delete[] data;
	query::delete_query (q);
	delete_treap (treap);

	return 0;
}
