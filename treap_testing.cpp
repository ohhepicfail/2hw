#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "treap.h"

#define PRINT 0

using namespace treap;

int main () {
	const unsigned n = 3000000;
	int* data = new int[n];

	srand (time (nullptr));
	for (unsigned i = 0; i < n; i++)
		data[i] = rand () % n + 1;

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

// int main () {
// 	const unsigned n = 1000000;
// 	int* data = new int[n];

// 	srand (time (nullptr));
// 	for (unsigned i = 0; i < n; i++)
// 		data[i] = rand ();

// 	treap_t* treap = create_treap (data, n);

// 	#if PRINT
// 		for (unsigned i = 0; i < n; i++)
// 			printf ("%d\t", data[i]);
// 		printin_dot (treap);
// 	#endif

// 	printf ("begin\n");
// 	for (unsigned i = 0; i < 100000; i++) {
// 		unsigned fst = rand () % n;
// 		unsigned snd = rand () % n;
// 		if (fst > snd) {
// 			unsigned tmp = fst;
// 			fst = snd;
// 			snd = tmp;
// 		}

// 		#if PRINT
// 			printf ("\nfst = %u\t snd = %u\n%d\t", fst, snd, find_min (treap, fst, snd));
// 		#endif

// 		int min = find_min (treap, fst, snd);
// 		for (; i % 1000 == 0 && fst <= snd; fst++)
// 			assert (data[fst] >= min);
// 	}

// 	printf ("end\n");
// 	delete[] data;
// 	delete_treap (treap);

// 	return 0;
// }