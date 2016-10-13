#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "tree.h"

using namespace tree;

int main () {


	tree_t* tree1 = create_tree (nullptr, 5);

	srand (time (nullptr));
	int tree_size = 50;
	for (int i = 1; i < tree_size; i++) {
		add (tree1, rand () % tree_size);
	}

	printin_dot (tree1);

	const tree_t* fst1 = nullptr;
	const tree_t* snd1 = nullptr;
	get_2addresses (tree1, &fst1, &snd1);

	print_tree (fst1);
	printf ("\n\n\n");
	print_tree (snd1);
	printf ("\n\n\n");

	printf ("way lengh %d\n", calculate_lengh (fst1, snd1));


	tree_t* tree2 = create_tree (nullptr, 5);
	for (int i = 1; i < tree_size; i++) {
		add (tree2, rand () % tree_size);
	}
	const tree_t* fst2 = nullptr;
	const tree_t* snd2 = nullptr;
	get_2addresses (tree2, &fst2, &snd2);

	printf ("way lengh %d\n", calculate_lengh (fst1, snd2));

	delete_tree (tree1);
	delete_tree (tree2);

	return 0;
}