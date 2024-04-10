/** \file main.c
 *  \brief Hlavní program testující API výkonnějších metod třídění
 *  \author Petyovský
 *  \version 2024
 *  $Id: main.c 2691 2024-04-05 14:58:43Z petyovsky $
 */

#include <stdio.h>
#include <time.h>
#include "basic_sorts.h"
#include "advanced_sorts.h"
#include "powerful_sorts.h"
#include "main_support.h"
#include "check.h"

/** \defgroup MainProgram 8. Hlavní program
 *  \brief Definice funkcí hlavního programu
 *  \{
 */

/** \brief Testování třídicí metody
 *  \details Testovací funkce pro třídicí metodu, funkce změří výpočetní čas, kterou třídicí metoda potřebovala k dokončení setřídění hodnot vektoru. Dále funkce ověří správnost setřídění hodnot a vypíše statistiky o počtu operací použitých pro setřídění.
 *  \param[in] aSortNameStr String reprezentující název třídicí metody
 *  \param[in] aSortFunc Ukazatel na funkci reprezentující třídicí metodu
 *  \param[in] aOrigVector Ukazatel na vstupní vektor
 *  \return \c true pokud testování třídicí metody pro daný vstupní vektor dopadlo úspěšně
 */
bool sort_testing(const char *aSortNameStr, void (*aSortFunc)(struct TVector *), const struct TVector *aOrigVector)
	{
	const bool print_elements_before = false;		// constexpr since C23
	const bool print_elements_after = false;		// constexpr since C23

	struct TVector vector = { 0 };
	if(!vector_clone(aOrigVector, &vector))
		return false;
	
	printf("\n------ %s ------\n", aSortNameStr);

	if(print_elements_before)
		{
		//vector_for_each(vector_iterator_begin(&vector), print_element);
		//putchar('\n');

		//auto it = vector_for_n(vector_iterator_begin(&vector), 20, print_element);		// auto since C23
		struct TVectorIterator it = vector_for_n(vector_iterator_begin(&vector), 20, print_element);
		puts(vector_iterator_is_valid(&it) ? "..." : "");
		}

	struct timespec start_timespec = { 0, }, end_timespec = { 0, };
	if(!timespec_get(&start_timespec, TIME_UTC))
		printf("Error on start timespec_get\n");

	(*aSortFunc)(&vector);
	if(!timespec_get(&end_timespec, TIME_UTC))
		printf("Error on end timespec_get\n");

	if(print_elements_after)
		{
		//vector_for_each(vector_iterator_begin(&vector), print_element);
		//putchar('\n');

		//auto it = vector_for_n(vector_iterator_begin(&vector), 20, print_element);		// auto since C23
		struct TVectorIterator it = vector_for_n(vector_iterator_begin(&vector), 20, print_element);
		puts(vector_iterator_is_valid(&it) ? "..." : "");
		}

	const bool is_sorted = vector_is_ascendantly_sorted(aOrigVector, &vector);
	printf("\nVector is %s\n", is_sorted ? "sorted. OK :-)" : "not sorted!!! FAIL :-(");
	printf("\nSorting time: %lf seconds.\n", difftimespec(end_timespec, start_timespec));

	vector_print_sortstats(&vector);
	vector_destroy(&vector);
	return is_sorted;
	}

int main(int argc, char *argv[])
	{
	srand((unsigned int)time(NULL));

	struct TVector vector_orig = { 0 };
	FILE *fsrc = fopen("reversed_data.txt", "r");
	//FILE *fsrc = fopen("select_sort_data.txt", "r");
	//FILE *fsrc = fopen("sorted_data.txt", "r");

	//FILE *fsrc = fopen("quick_sort_data1.txt", "r");
	//FILE *fsrc = fopen("quick_sort_data2.txt", "r");
	//FILE *fsrc = fopen("quick_sort_data3.txt", "r");
	//FILE *fsrc = fopen("quick_sort_data4.txt", "r");

	if(!fsrc)
		return 1;

	//bool initialized = vector_init_random(&vector_orig, 3);
	//bool initialized = vector_init_random(&vector_orig, 50000);
	bool initialized = vector_init_file(&vector_orig, fsrc);

	fclose(fsrc);
	if(!initialized)
		return 2;

	sort_testing("Insert Sort", insert_sort, &vector_orig);
	//sort_testing("Select Sort", select_sort, &vector_orig);
	//sort_testing("Bubble Sort", bubble_sort, &vector_orig);
	//sort_testing("Bubble Sort Optimized", bubble_sort_optimized, &vector_orig);
	//sort_testing("Shaker Sort", shaker_sort, &vector_orig);

	sort_testing("Shell Sort", shell_sort, &vector_orig);
	sort_testing("Quick Sort", quick_sort, &vector_orig);

	sort_testing("Heap Sort (iterators, top_down)", heap_sort_with_iterators, &vector_orig);
	sort_testing("Heap Sort (no iterators, top_down)", heap_sort, &vector_orig);

	sort_testing("Merge Sort", merge_sort, &vector_orig);
	sort_testing("Merge Sort (with span)", merge_sort_with_span, &vector_orig);

	vector_destroy(&vector_orig);
	return 0;
	}

/** \example cv10_reports_reversed_data.lst
 * Příklad výstupu programu pro vstupní soubor: [`reversed_data.txt`](pages.html).
 */
/** \example cv10_reports_select_sort_data.lst
 * Příklad výstupu programu pro vstupní soubor: [`select_sort_data.txt`](pages.html).
 */
/** \example cv10_reports_sorted_data.lst
 * Příklad výstupu programu pro vstupní soubor: [`sorted_data.txt`](pages.html).
 */

/** \example cv10_reports_quick_sort_data1.lst
 * Příklad výstupu programu pro vstupní soubor: [`quick_sort_data1.txt`](pages.html).
 */
/** \example cv10_reports_quick_sort_data2.lst
 * Příklad výstupu programu pro vstupní soubor: [`quick_sort_data2.txt`](pages.html).
 */
/** \example cv10_reports_quick_sort_data3.lst
 * Příklad výstupu programu pro vstupní soubor: [`quick_sort_data3.txt`](pages.html).
 */
/** \example cv10_reports_quick_sort_data4.lst
 * Příklad výstupu programu pro vstupní soubor: [`quick_sort_data4.txt`](pages.html).
 */

/** \} MainProgram */
