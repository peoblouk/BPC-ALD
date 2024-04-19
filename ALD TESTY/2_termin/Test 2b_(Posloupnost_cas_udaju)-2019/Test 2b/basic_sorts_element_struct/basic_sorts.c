/** \file basic_sorts.c
 *  \brief Implementace API přímých metod třídění
 *  \author Petyovský
 *  \version 2024
 *  $Id: basic_sorts.c 2631 2024-03-08 16:40:08Z petyovsky $
 */

#include "basic_sorts.h"

void print_element(const struct TVectorIterator* aIter)
	{
	vector_element_store_file(vector_iterator_value(aIter), stdout);
	}

void insert_sort(struct TVector* aVector) // složitost je n2
	{
	vector_for_each(vector_iterator_begin(aVector), print_element);
	putchar('\n');

	if (aVector == NULL)
		return;

	for (size_t i = 1; i < aVector->iSize; i++)
		{
		// vector_for_each(vector_iterator_begin(aVector), print_element);
		// putchar('\n');

		TVectorElement tmp = vector_value(aVector, i); // vyberu hodnotu a porovnávám jí v nesetrizene oblasti
		size_t j = i;
		for (; j > 0; --j) // Projedu nesetřízenou oblastí <- |
			{
			if (vector_compare_position_value(aVector, j - 1, tmp) <= 0)
				break;
			vector_move_positions(aVector, j, aVector, j - 1);
			}
		vector_set_value(aVector, j, tmp);
		}

	// vector_for_each(vector_iterator_begin(aVector), print_element);
	// putchar('\n');
	}

void select_sort(struct TVector* aVector) // složitost je n2
	{
	if (aVector == NULL)
		return;

	for (size_t i = 0; i < aVector->iSize - 1; i++) // i ukazuje na prvni prvek nesetridene oblasti
		{
		// vector_for_each(vector_iterator_begin(aVector), print_element);
		// putchar('\n');
		size_t minPos = i;

		for (size_t j = i + 1; j < aVector->iSize; j++) // j slouzi pro prochazeni nesetridene oblasti (od druheho prvku po poslední)
			{
			if (vector_compare_positions(aVector, j, aVector, minPos) < 0) // Najdu nejmenší hodnotu
				minPos = j;
			}
		vector_swap_positions(aVector, i, aVector, minPos); // Přehoď hodnotu min na začátek
		}
	// vector_for_each(vector_iterator_begin(aVector), print_element);
	// putchar('\n');
	}

void bubble_sort(struct TVector* aVector) // Obecně platí, že složitost je n2
	{
	if (aVector == NULL)
		return;

	for (size_t i = 0; i < (aVector->iSize - 1); i++) // i je poceft prvku setridene oblasti // setrizená oblast je vlevo
		{
		// vector_for_each(vector_iterator_begin(aVector), print_element);
		// putchar('\n');

		for (size_t j = 0; j < aVector->iSize - i - 1; j++) // Prochází nesetridene oblasti (nesetridena oblast se postupne zkracuje o 1 (-i))
			if (vector_compare_positions(aVector, j, aVector, j + 1) > 0) // Hodnota je větší
				vector_swap_positions(aVector, j, aVector, j + 1);
		}
	// vector_for_each(vector_iterator_begin(aVector), print_element);
	// putchar('\n');
	}

void bubble_sort_optimized(struct TVector* aVector) // Optimalizace - Kontrolou zda zde nedošlo k swapu
	{
	if (aVector == NULL)
		return;

	bool again = true; // Indikace jestli došlo ke swapu nebo ne

	for (size_t i = 0; i < (aVector->iSize - 1) && again == true; i++) // i je poceft prvku setridene oblasti // setrizená oblast je vlevo
		{
		// vector_for_each(vector_iterator_begin(aVector), print_element);
		// putchar('\n'); 
		again = false;

		for (size_t j = 0; j < aVector->iSize - i - 1; j++) // Prochází nesetridene oblasti (nesetridena oblast se postupne zkracuje o 1 (-i))
			if (vector_compare_positions(aVector, j, aVector, j + 1) > 0) // Hodnota je větší
				{
				vector_swap_positions(aVector, j, aVector, j + 1);
				again = true;
				}
		}
	// vector_for_each(vector_iterator_begin(aVector), print_element);
	// putchar('\n');
	}

void bubble_sort_optimized2(struct TVector* aVector) // Optimalizace zapamatováním posledního swapu
	{
	if (aVector == NULL)
		return;

	size_t maxpos = aVector->iSize - 1;

	while (maxpos > 0)
		{
		//vector_for_each(vector_iterator_begin(aVector), print_element);
		//putchar('\n');

		size_t swap_lastpos = 0;

		for (size_t j = 0; j < maxpos; j++) // Prochází nesetridene oblasti (nesetridena oblast se postupne zkracuje o 1 (-i))
			{
			if (vector_compare_positions(aVector, j, aVector, j + 1) > 0) // Hodnota je větší
				{
				vector_swap_positions(aVector, j, aVector, j + 1);
				swap_lastpos = j;
				}
			}
		maxpos = swap_lastpos;
		}
	// vector_for_each(vector_iterator_begin(aVector), print_element);
	// putchar('\n');
	}

void shaker_sort(struct TVector* aVector)
	{
	if (aVector == NULL)
		return;

	size_t maxpos = aVector->iSize - 1;
	size_t minpos = 0;
	size_t swap_lastpos = 0;

	while (minpos < maxpos) // Musím dojet do středu
		{
		// vector_for_each(vector_iterator_begin(aVector), print_element);
		// putchar('\n');

		// zleva do prava
		for (size_t j = minpos; j < maxpos; j++) // Prochází nesetridene oblasti (nesetridena oblast se postupne zkracuje o 1 (-i))
			if (vector_compare_positions(aVector, j, aVector, j + 1) > 0) // Hodnota je větší
				{
				vector_swap_positions(aVector, j, aVector, j + 1);
				swap_lastpos = j;
				}
		maxpos = swap_lastpos;

		// zprava do leva
		for (size_t j = maxpos; j > minpos; j--) // Prochází nesetridene oblasti (nesetridena oblast se postupne zkracuje o 1 (-i))
			if (vector_compare_positions(aVector, j - 1, aVector, j) > 0) // Hodnota je větší
				{
				vector_swap_positions(aVector, j - 1, aVector, j);
				swap_lastpos = j;
				}
		minpos = swap_lastpos;
		}
	//vector_for_each(vector_iterator_begin(aVector), print_element);
	//putchar('\n');
	}