#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "treap.h"

#define PRINT 0

using namespace treap;

int main () {
	const unsigned n = 10000000;
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
	unsigned nrequest = n*10;
	unsigned* fst = new unsigned[nrequest];
	unsigned* snd = new unsigned[nrequest];
	for (unsigned i = 0; i < nrequest; i++) {
		fst[i] = rand () % 256;
		snd[i] = rand () % 256;
	}
	int* ans = new int[nrequest] ();

	tarjan_olca (treap, fst, snd, ans, nrequest);

	printf ("end\n");
	delete[] data;
	delete[] fst;
	delete[] snd;
	delete[] ans;
	delete_treap (treap);

	return 0;
}
