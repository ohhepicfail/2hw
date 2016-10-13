#include <cstdio>
#include "list.h"

using namespace list;

int main () {
	node_t* list = create_list (0);

	for (unsigned i = 0; i < 6u; i++)
		push_back (list, static_cast <const void*> (&i - i));

	print_list (list);
	random_loop (list);

	unsigned loop_size = find_loop (list, nullptr);
	printf ("loop size %u\n", loop_size);
	list = reverse (list);
	printf ("after reversing:\n");
	print_list (list);

	delete_list (list);
}