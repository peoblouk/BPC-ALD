#include <stdio.h>
#include "testbench.h"
#include "TSet.h"

const struct TTestPlan testplan[] =
	{
	#include "testplan_difference.h" // Test equal
	#include "testplan_difference2.h" // Test equal
	};

int main(int argc, char* argv[])
	{
	struct TSet set1 = { 0 };
	struct TSet set2 = { 0 };

	set_init(&set1);
	set_init(&set2);

	if (!testbench_run(&set1, 1, testplan))
		{
		puts("testplan 1 failed!!!");
		set_destroy(&set1);
		return 1;
		}

	if (!testbench_run(&set2, 1, testplan + 1))
		{
		puts("testplan 2 failed!!!");
		set_destroy(&set2);
		return 2;
		}

	putchar('\n');

	printf("SET1: ");
	set_for_each(set_iterator_begin(&set1), print_value_by_iterator);
	putchar('\n');

	printf("SET2: ");
	set_for_each(set_iterator_begin(&set2), print_value_by_iterator);
	putchar('\n');


	set_destroy(&set1);
	set_destroy(&set2);

	return 0;
	}