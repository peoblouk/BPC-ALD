#include <stdio.h>
#include "testbench.h"
#include "TSet.h"

const struct TTestPlan testplan[] =
	{
	#include "testplan_difference.h" // Test difference
	#include "testplan_difference2.h" // Test difference
	#include "testplan_difference3.h" // Test difference
	};

int main(int argc, char* argv[])
	{
	struct TSet set1 = { 0 };
	struct TSet set2 = { 0 };
	struct TSet set3 = { 0 };

	set_init(&set1);
	set_init(&set2);
	set_init(&set3);

	if (!testbench_run(&set1, 1, testplan))
		{
		puts("testplan 1 failed!!!");
		set_destroy(&set1);
		set_destroy(&set2);
		return 1;
		}

	if (!testbench_run(&set2, 1, testplan + 1))
		{
		puts("testplan 2 failed!!!");
		set_destroy(&set1);
		set_destroy(&set2);
		return 2;
		}

	if (!set_difference(&set3, &set1, &set2)) // Test rozdílu
		{
		set_destroy(&set1);
		set_destroy(&set2);
		set_destroy(&set3);
		return 4;
		}

	if (!testbench_run(&set3, 1, testplan + 2))
		{
		puts("testplan 3 failed!!!");
		set_destroy(&set1);
		set_destroy(&set2);
		set_destroy(&set3);
		return 3;
		}

	putchar('\n');

	printf("SET1: ");
	set_for_each(set_iterator_begin(&set1), print_value_by_iterator);
	putchar('\n');

	printf("SET2: ");
	set_for_each(set_iterator_begin(&set2), print_value_by_iterator);
	putchar('\n');

	printf("rozdil: ");
	set_for_each(set_iterator_begin(&set3), print_value_by_iterator);
	putchar('\n');


	set_destroy(&set1);
	set_destroy(&set2);
	set_destroy(&set3);

	return 0;
	}