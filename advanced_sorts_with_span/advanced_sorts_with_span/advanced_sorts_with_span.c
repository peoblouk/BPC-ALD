/** \file advanced_sorts_with_span.c
 *  \brief Implementace API účinnějších metod třídění využívající typ Span
 *  \author Petyovský
 *  \version 2024
 *  $Id: advanced_sorts_with_span.c 2680 2024-04-02 10:28:36Z petyovsky $
 */

#include "advanced_sorts_with_span.h"
#include "TSpan.h"

struct TVectorIterator gVector_begin_it = { 0 };		///< Bohužel z důvodů tisku hodnot elementů, je zde třeba globalní proměnná :-(

/** \brief Formátovaný tisk hodnoty elementu za předpokladu platné hodnoty
 *  \details Vytiskne formátovaně hodnotu elementu pokud je parametr \p aCondition platný, jinak se vytisknou zástupné znaky.
 *  \param[in] aCondition pokud je \c true bude hodnota vytisknuta, jinak se tisknou zástupné znaky
 *  \param[in] aIter Ukazatel na iterátor určující element k vytisknutí
 */
//[[maybe_unused]]  // Since C23
static inline void print_element_if(bool aCondition, const struct TVectorIterator *aIter)
	{
	if(aCondition)
		vector_element_store_file(vector_iterator_value(aIter), stdout);
	else
		fprintf(stdout, ". ");
	}

/** \brief Vnitřní a lokální funkce realizující rekurzivní funkci pro metodu (Quick Sort)
 *  \details Realizuje rekurzivní funkci pro třídicí metodu (Quick Sort) pro omezený rozsah elementů vektoru.
 *  \param[in,out] aSpan Omezený rozsah elementů vektoru
 */
static void quick_sort_with_span_worker(struct TSpan aSpan)
	{
	//struct TVectorIterator it = gVector_begin_it;
	//for(bool valid = vector_iterator_is_valid(&it); valid; valid = vector_iterator_to_next(&it))
	//	print_element_if(vector_iterator_is_inside_span(&aSpan, &it), &it);
	//putchar('\n');
	}

void quick_sort_with_span(struct TVector *aVector)
	{
	if(vector_size(aVector) < 42) // Výraz v podmínce vhodně upravte :-)
		return;

	gVector_begin_it = vector_iterator_begin(aVector);

	quick_sort_with_span_worker(span_create(*aVector));
	}
