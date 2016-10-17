#include <cstdio>
#include <ctime>
#include "list.h"

using namespace list;

int main () {
	node_t* list = create_or_push_front (nullptr, 0);

	for (unsigned i = 0; i < 150000000u; i++)
		list = create_or_push_front (list, static_cast <const void*> (&i - i));

	random_loop (list);

	const node_t* lb    = nullptr;
	const node_t* lb_bl = nullptr;

	std::clock_t start;
	start = std::clock();
	unsigned loop_size    = find_loop (list, &lb);
	double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	printf ("find_loop time %lf\n", duration);

	start = std::clock ();
	unsigned loop_size_bl = find_loop_bitlabel (list, &lb_bl);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	printf ("find_loop_bitlabel time %lf\n", duration);

	printf ("%d\t%d\n", loop_size == loop_size_bl, lb == lb_bl);

	list = reverse (list);
	lb    = nullptr;
	lb_bl = nullptr;

	start = std::clock ();
	loop_size    = find_loop (list, &lb);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	printf ("find_loop time %lf\n", duration);

	start = std::clock ();
	loop_size_bl = find_loop_bitlabel (list, &lb_bl);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	printf ("find_loop_bitlabel time %lf\n", duration);

	printf ("%d\t%d\n", loop_size == loop_size_bl, lb == lb_bl);

	delete_list (list);
}