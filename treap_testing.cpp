#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "treap.h"

#define PRINT 0

using namespace treap;

int main () {
	const unsigned n = 1000000;
	int* data = new int[n];

	srand (time (nullptr));
	for (unsigned i = 0; i < n; i++)
		data[i] = rand ();

	treap_t* treap = create_treap (data, n);

	#if PRINT
		for (unsigned i = 0; i < n; i++)
			printf ("%d\t", data[i]);
		printin_dot (treap);
	#endif

	printf ("begin\n");
	for (unsigned i = 0; i < 100000; i++) {
		unsigned fst = rand () % n;
		unsigned snd = rand () % n;
		if (fst > snd) {
			unsigned tmp = fst;
			fst = snd;
			snd = tmp;
		}
		#if PRINT
			printf ("\nfst = %u\t snd = %u\n%d\t", fst, snd, find_min (treap, fst, snd));
		#endif
		find_min (treap, fst, snd);
	}

	printf ("end\n");
	delete[] data;
	delete_treap (treap);
}