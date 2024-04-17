/** \file main.c
 *  \brief Hlavní program pro testování ADT TSet
 *  \author Petyovský
 *  \version 2024
 *  $Id: main.c 2720 2024-04-16 07:54:08Z petyovsky $
 */

#include <stdio.h>
#include "testbench.h"
#include "TSet.h"

/** \defgroup MainProgram 5. Hlavní program
 *  \brief Definice funkcí hlavního programu
 *  \{
 */

const struct TTestPlan testplan[] =
	{
	//#include "testplan_set1.h"
	#include "testplan_set2.h"
	//#include "testplan_set3.h"
	};

void testset2_hardcoded(void)
	{
	struct TSet set2 = { 0 };
	set_init(&set2);
	gSetElementComparisons = 0;

	printf("set1 is empty: %s\n", set_is_empty(&set2) ? "true" : "false");
	set_insert(&set2, 222);
	printf("set1 is empty: %s\n", set_is_empty(&set2) ? "true" : "false");

	for(struct TSetIterator it = set_iterator_begin(&set2); set_iterator_is_valid(&it); set_iterator_to_next(&it))
		printf("%d ", set_iterator_value(&it));
	putchar('\n');

	set_insert(&set2, 42);
	set_insert(&set2, 43);
	set_insert(&set2, 40);
	set_insert(&set2, 41);
	set_insert(&set2, 333);

	set_for_each(set_iterator_begin(&set2), print_value_by_iterator);
	putchar('\n');

	set_erase(&set2, 222);
	set_erase(&set2, 40);

	set_for_each(set_iterator_begin(&set2), print_value_by_iterator);
	putchar('\n');

	bool is_element_of = set_is_element_of(&set2, 42);
	printf("set1 find 42: %s\n", (is_element_of) ? "true" : "false");

	set_destroy(&set2);

	printf("\nTSetElement comparisons: %zu\n", gSetElementComparisons);
	}

int main(int argc, char *argv[])
	{
	struct TSet set1 = { 0 };
	set_init(&set1);

	if(!testbench_run(&set1, ITEMS_OF(testplan), testplan))
		{
		puts("testplan failed!!!");
		set_destroy(&set1);
		return 1;
		}
	set_destroy(&set1);

	testset2_hardcoded();
	return 0;
	}

/** \example cv11_reports_TSet_sorted_dynamic_array_set1_linear_search_optimized.lst
 * Příklad výstupu programu pro soubor `testplan_set1.h` při `FLEX_ARRAY_SEARCH_VARIANT == LINEAR_SEARCH_OPTIMIZED`.
 */
/** \example cv11_reports_TSet_sorted_dynamic_array_set1_binary_search.lst
 * Příklad výstupu programu pro soubor `testplan_set1.h` při `FLEX_ARRAY_SEARCH_VARIANT == BINARY_SEARCH`.
 */

/** \example cv11_reports_TSet_sorted_dynamic_array_set2_linear_search_optimized.lst
 * Příklad výstupu programu pro soubor `testplan_set2.h` při `FLEX_ARRAY_SEARCH_VARIANT == LINEAR_SEARCH_OPTIMIZED`.
 */
/** \example cv11_reports_TSet_sorted_dynamic_array_set2_binary_search.lst
 * Příklad výstupu programu pro soubor `testplan_set2.h` při `FLEX_ARRAY_SEARCH_VARIANT == BINARY_SEARCH`.
 */

/** \example cv11_reports_TSet_sorted_dynamic_array_set3_linear_search_optimized.lst
 * Příklad výstupu programu pro soubor `testplan_set3.h` při `FLEX_ARRAY_SEARCH_VARIANT == LINEAR_SEARCH_OPTIMIZED`.
 */
/** \example cv11_reports_TSet_sorted_dynamic_array_set3_binary_search.lst
 * Příklad výstupu programu pro soubor `testplan_set3.h` při `FLEX_ARRAY_SEARCH_VARIANT == BINARY_SEARCH`.
 */

/** \} MainProgram */
