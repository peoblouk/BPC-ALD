/** \file main.c
 *  \brief Hlavní program testující API přímých metod třídění
 *  \author Petyovský
 *  \version 2024
 *  $Id: main.c 2631 2024-03-08 16:40:08Z petyovsky $
 */

#include <stdio.h>
#include <time.h>
#include "basic_sorts.h"
#include "main_support.h"
#include "check.h"

/** \defgroup MainProgram 6. Hlavní program
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
	struct TVector vector = { 0 };
	if(!vector_clone(aOrigVector, &vector))
		return false;
	
	printf("\n------ %s ------\n", aSortNameStr);
	//vector_for_each(vector_iterator_begin(&vector), print_element);
	//vector_for_n(vector_iterator_begin(&vector), 20, print_element);
	//putchar('\n');

	struct timespec start_timespec = { 0, }, end_timespec = { 0, };
	if(!timespec_get(&start_timespec, TIME_UTC))
		printf("Error on start timespec_get\n");

	(*aSortFunc)(&vector);
	if(!timespec_get(&end_timespec, TIME_UTC))
		printf("Error on end timespec_get\n");

	//vector_for_each(vector_iterator_begin(&vector), print_element);
	//vector_for_n(vector_iterator_begin(&vector), 20, print_element);
	//putchar('\n');

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
	// FILE *fsrc = fopen("reversed_data.txt", "r");
	// FILE *fsrc = fopen("select_sort_data.txt", "r");
	// FILE *fsrc = fopen("bubble_sort_optimized_data.txt", "r");
	FILE *fsrc = fopen("shaker_sort_data.txt", "r");
	// FILE *fsrc = fopen("sorted_data.txt", "r");

	if(!fsrc)
		return 1;

	// bool initialized = vector_init_random(&vector_orig, 50000);
	bool initialized = vector_init_file(&vector_orig, fsrc);

	fclose(fsrc);
	if(!initialized)
		return 2;

	sort_testing("Insert Sort", insert_sort, &vector_orig);
	sort_testing("Select Sort", select_sort, &vector_orig);
	sort_testing("Bubble Sort", bubble_sort, &vector_orig);
	sort_testing("Bubble Sort Optimized", bubble_sort_optimized, &vector_orig);
	sort_testing("Bubble Sort Optimized2", bubble_sort_optimized2, &vector_orig);
	sort_testing("Shaker Sort", shaker_sort, &vector_orig);

	vector_destroy(&vector_orig);
	return 0;
	}

/** \} MainProgram */
