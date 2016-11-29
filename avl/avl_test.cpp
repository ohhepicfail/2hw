#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "avl.h"


int main () {
	avl::AVL_tree avl (5);

	srand (time (0));
	auto n = 20;
	for (int i = 0; i < n; i++)
		avl.insert (i);


	avl.print ("avl.dot");

	printf ("number of the elements that is less than\n");
	for (auto i = -1; i < n + 2; i++)
		printf ("%d\t%u\n", i, avl.n_elem_lessthan (i));

	printf ("\nn min elem\n");
	for (auto i = 1; i < n + 1; i++)
		printf ("%u\t%u\n", i, avl.find_nmin (i));

	return 0;
}