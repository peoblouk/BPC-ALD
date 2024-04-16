/** \file merge_sort_with_span.c
 *  \brief Implementace API výkonnější metody třídění slučováním (Merge sort), využívající rekurzi a datový typ Span
 *  \author Petyovský
 *  \version 2023
 *  $Id: merge_sort_with_span.c 2323 2023-04-11 12:58:49Z petyovsky $
 */

#include "merge_sort_with_span.h"
#include "TSpan.h"

/** \brief Vnitřní a lokální funkce realizující sloučení dvou již setříděných rozsahů (Spanů)
 *  \details Realizuje sloučení dvou již setříděných rozsahů \p aFrom1 \p aFrom2 do výsledného rozsahu \p aTo.
 *  \param[in] aFrom1 První rozsah obsahující již setříděné vstupních elementy
 *  \param[in] aFrom2 Druhý rozsah obsahující již setříděné vstupních elementy
 *  \param[out] aTo Výstupní rozsah pro elementy z obou sloučených / zatříděných rozsahů \p aFrom1 a \p aFrom2
 */
static void mergify(struct TSpan aFrom1, struct TSpan aFrom2, struct TSpan aTo)
	{
	//	puts("Mergify:");
	//	for(size_t i = 0; i < span_size(&aFrom1); ++i)
	//		vector_element_store_file(aFrom1.iVector.iValues[i], stdout);
	//	fprintf(stdout, "| ");
	//	for(size_t i = 0; i < span_size(&aFrom2); ++i)
	//		vector_element_store_file(aFrom2.iVector.iValues[i], stdout);
	//	putchar('\n');

	size_t f1 = 0, f2 = 0, t = 0;
	const size_t from1_size = span_size(&aFrom1);
	const size_t from2_size = span_size(&aFrom2);

	while(f1 < from1_size && f2 < from2_size)
		{
		if(span_compare_positions(&aFrom1, f1, &aFrom2, f2) <= 0)
			span_move_positions(&aTo, t++, &aFrom1, f1++);
		else
			span_move_positions(&aTo, t++, &aFrom2, f2++);
		}

	while(f1 < from1_size)
		span_move_positions(&aTo, t++, &aFrom1, f1++);

	while(f2 < from2_size)
		span_move_positions(&aTo, t++, &aFrom2, f2++);

//	for(size_t i = 0; i < span_size(&aTo); ++i)
//		vector_element_store_file(aTo.iVector.iValues[i], stdout);
//	putchar('\n');
	}

/** \brief Vnitřní a lokální funkce realizující rekurzivní funkci pro metodu (Merge Sort)
 *  \details Realizuje rekurzivní funkci pro třídicí metodu (Merge Sort), setřídí elementy rozsahu \p aFrom a ukládá je do rozsahu \p aTo.
 *  \param[in] aFrom Vstupní rozsah říděných vstupních elementů
 *  \param[out] aTo Výstupní rozsah elementů pro setříděné elementy
 */
static void merge_sort_worker(struct TSpan aFrom, struct TSpan aTo)
	{
	if(span_size(&aFrom) < 2)
		return;

//	puts("aFrom:");
//	for(size_t i = 0; i < span_size(&aFrom); ++i)
//		vector_element_store_file(aFrom.iVector.iValues[i], stdout);
//	putchar('\n');

	const size_t middle = span_size(&aTo) / 2;

//	{
//	puts("Split:");
//	struct TSpan left = span_create_left_subspan(aTo, middle);
//	struct TSpan right = span_create_right_subspan(aTo, middle);
//	for(size_t i = 0; i < span_size(&left); ++i)
//		vector_element_store_file(left.iVector.iValues[i], stdout);
//	fprintf(stdout, "| ");
//	for(size_t i = 0; i < span_size(&right); ++i)
//		vector_element_store_file(right.iVector.iValues[i], stdout);
//	putchar('\n');
//	}

	merge_sort_worker(span_create_left_subspan(aTo, middle), span_create_left_subspan(aFrom, middle));
	merge_sort_worker(span_create_right_subspan(aTo, middle), span_create_right_subspan(aFrom, middle));

	mergify(span_create_left_subspan(aFrom, middle), span_create_right_subspan(aFrom, middle), aTo);
	}

void merge_sort_with_span(struct TVector *aVector)
	{
	if(vector_size(aVector) < 2)
		return;

	struct TVector work_vector = { 0 };
	if(!vector_attach_sortstats(aVector, &work_vector))	// Create sortstats relationship between `aVector` and `work_vector`
		return;

	if(!vector_clone(aVector, &work_vector))			// Copy all element values from `aVector` to the `work_vector`
		return;

	merge_sort_worker(span_create(work_vector), span_create(*aVector));

	vector_destroy(&work_vector);
	}
