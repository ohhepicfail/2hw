#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "avl.h"


int main () {
	avl::AVL_tree avl (5);

	srand (time (0));
	auto n = 50;
	for (int i = 0; i < n; i++)
		avl.insert (rand () % n);

	avl.print ("avl.dot");
	for (int i = 0; i < n; i++)
		avl.remove (rand () % n);
	avl.print ("avl1.dot");

	return 0;
}