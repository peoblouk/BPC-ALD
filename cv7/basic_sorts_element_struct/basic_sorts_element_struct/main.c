/** \file main.c
 *  \brief Hlavní program testující API přímých metod třídění
 *  \author Petyovský
 *  \version 2024
 *  $Id: main.c 2641 2024-03-18 12:12:21Z petyovsky $
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
	//putchar('\n');

	//auto it = vector_for_n(vector_iterator_begin(&vector), 20, print_element);					// Since C23
	//struct TVectorIterator it = vector_for_n(vector_iterator_begin(&vector), 20, print_element);
	//puts(vector_iterator_is_valid(&it) ? "..." : "");

	struct timespec start_timespec = { 0, }, end_timespec = { 0, };
	if(!timespec_get(&start_timespec, TIME_UTC))
		printf("Error on start timespec_get\n");

	(*aSortFunc)(&vector);
	if(!timespec_get(&end_timespec, TIME_UTC))
		printf("Error on end timespec_get\n");

	//vector_for_each(vector_iterator_begin(&vector), print_element);
	//putchar('\n');

	//struct TVectorIterator it = vector_for_n(vector_iterator_begin(&vector), 20, print_element);
	//puts(vector_iterator_is_valid(&it) ? "..." : "");

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
	FILE *fsrc = fopen("reversed_data_student.txt", "r");
	if(!fsrc)
		return 1;

	//bool initialized = vector_init_random(&vector_orig, 5/*0000*/);
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

	puts("\n------ Test stability vicenasobneho trideni s metodou select_sort:");
	vector_element_set_comparator(EByLastName);
	/// \bug Zvolte vhodnout třídicí metodu, která splňuje podmínku stability. (\fileinfo{filename}:\lineinfo)
	select_sort(&vector_orig);

	vector_element_set_comparator(EByFirstName);
	/// \bug Zvolte správné pořadí stabilních a nestabilních metod třídění dle jednotlivých klíčů. (\fileinfo{filename}:\lineinfo)
	select_sort(&vector_orig);

	//vector_for_each(vector_iterator_begin(&vector_orig), print_element);
	vector_for_n(vector_iterator_begin(&vector_orig), 20, print_element);
	putchar('\n');

	vector_destroy(&vector_orig);
	return 0;
	}

/** \} MainProgram */
