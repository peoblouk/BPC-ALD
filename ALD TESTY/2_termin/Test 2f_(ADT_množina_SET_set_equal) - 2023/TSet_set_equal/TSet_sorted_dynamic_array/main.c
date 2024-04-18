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
	#include "testplan_equal.h" // Test equal
	#include "testplan_equal2.h" // Test equal
	#include "testplan_equal3.h" // Test equal
	};

int main(int argc, char *argv[])
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
		return 1;
		}

	if (!testbench_run(&set2, 1, testplan + 1))
		{
		puts("testplan 2 failed!!!");
		set_destroy(&set2);
		return 2;
		}

	if (!testbench_run(&set3, 1, testplan + 2))
		{
		puts("testplan 3 failed!!!");
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

	printf("SET3: ");
	set_for_each(set_iterator_begin(&set3), print_value_by_iterator);
	putchar('\n');

	putchar('\n');
	printf("Mnoziny c. 1 a 2 %s stejne.", (set_is_equal(&set1, &set2)) ? ("jsou") : ("nejsou"));
	putchar('\n');
	printf("Mnoziny c. 1 a 3 %s stejne.", (set_is_equal(&set1, &set3)) ? ("jsou") : ("nejsou"));
	putchar('\n');
	printf("Mnoziny c. 1 a 1 %s stejne.", (set_is_equal(&set1, &set1)) ? ("jsou") : ("nejsou"));
	putchar('\n');
	printf("Mnoziny c. 2 a 1 %s stejne.", (set_is_equal(&set2, &set1)) ? ("jsou") : ("nejsou"));

	set_destroy(&set1);
	set_destroy(&set2);
	set_destroy(&set3);

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



/* TEST
	set_init(&set2);

	set_insert(&set1, "c");
	set_insert(&set2, "c");

	set_insert(&set1, "b");
	set_insert(&set2, "b");

	if (set_is_equal(&set1, &set2))
		{
		printf("ok!");
		return 1;
		}

	set_destroy(&set1);
	set_destroy(&set2);
*/