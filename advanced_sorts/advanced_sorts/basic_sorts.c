/** \file basic_sorts.c
 *  \brief Implementace API přímých metod třídění
 *  \author Petyovský
 *  \version 2024
 *  $Id: basic_sorts.c 2631 2024-03-08 16:40:08Z petyovsky $
 */

#include "basic_sorts.h"

void print_element(const struct TVectorIterator *aIter)
	{
	vector_element_store_file(vector_iterator_value(aIter), stdout);
	}

void insert_sort(struct TVector *aVector)
	{
	//vector_for_each(vector_iterator_begin(aVector), print_element);
	//putchar('\n');
	}

void select_sort(struct TVector *aVector)
	{
	//vector_for_each(vector_iterator_begin(aVector), print_element);
	//putchar('\n');
	}

void bubble_sort(struct TVector *aVector)
	{
	//vector_for_each(vector_iterator_begin(aVector), print_element);
	//putchar('\n');
	}

void bubble_sort_optimized(struct TVector *aVector)
	{
	//vector_for_each(vector_iterator_begin(aVector), print_element);
	//putchar('\n');
	}

void bubble_sort_optimized2(struct TVector *aVector)
	{
	//vector_for_each(vector_iterator_begin(aVector), print_element);
	//putchar('\n');
	}

void shaker_sort(struct TVector *aVector)
	{
	//vector_for_each(vector_iterator_begin(aVector), print_element);
	//putchar('\n');
	}
