#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "treap.h"

#define PRINT 0

using namespace treap;

int main () {
	const unsigned n = 1000000;
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
	unsigned* fst = new unsigned[n];
	unsigned* snd = new unsigned[n];
	for (unsigned i = 0; i < n; i++) {
		fst[i] = rand () % n;
		snd[i] = rand () % n;
	}
	int* ans = new int[n] ();

	tarjan_olca (treap, fst, snd, ans, n);

	printf ("end\n");
	delete[] data;
	delete[] fst;
	delete[] snd;
	delete[] ans;
	delete_treap (treap);

	return 0;
}
