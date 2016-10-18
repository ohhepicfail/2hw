#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "tree.h"

#define PRINT_TREE 0

using namespace tree;

int main () {


	tree_t* tree1 = create_tree (nullptr, 5);
	srand (time (nullptr));
	int tree_size = 1501;
	for (int i = 1; i < tree_size; i++) {
		add (tree1, rand () % tree_size);
	}

	const tree_t* fst1 = nullptr;
	const tree_t* snd1 = nullptr;
	get_2addresses (tree1, &fst1, &snd1);

	#if PRINT_TREE
		printin_dot (tree1);
		print_tree (fst1);
		printf ("\n\n\n");
		print_tree (snd1);
		printf ("\n\n\n");
	#endif

	std::clock_t start;
	start = std::clock();
	printf ("1) way lengh  %d\n", calculate_length (fst1, snd1));
	double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	printf ("calculate_length time %lf\n", duration);

	start = std::clock();
	printf ("2) way lengh  %d\n", length_bitlabel (const_cast<tree_t*> (fst1), const_cast<tree_t*> (snd1)));
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	printf ("length_bitlabel  time %lf\n", duration);

	tree_t* tree2 = create_tree (nullptr, 5);
	for (int i = 1; i < tree_size; i++) {
		add (tree2, rand () % tree_size);
	}
	const tree_t* fst2 = nullptr;
	const tree_t* snd2 = nullptr;
	get_2addresses (tree2, &fst2, &snd2);

	start = std::clock();
	printf ("1) way lengh  %d\n", calculate_length (fst1, snd2));
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	printf ("calculate_length time %lf\n", duration);

	start = std::clock();
	printf ("2) way lengh  %d\n", length_bitlabel (const_cast<tree_t*> (fst1), const_cast<tree_t*> (snd2)));
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	printf ("length_bitlabel  time %lf\n", duration);

	delete_tree (tree1);
	delete_tree (tree2);

	return 0;
}