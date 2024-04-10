/** \file merge_sort.c
 *  \brief Implementace API výkonnější metody třídění slučováním (Merge sort), využívající rekurzi
 *  \author Petyovský
 *  \version 2024
 *  $Id: merge_sort.c 2691 2024-04-05 14:58:43Z petyovsky $
 */

#include "merge_sort.h"

/** \brief Vnitřní a lokální funkce realizující sloučení dvou již setříděných rozsahů z vektoru `aFromVector` do vektoru `aToVector`
 *  \details Realizuje sloučení dvou již setříděných rozsahů `<aBegin,aMiddle)` a `<aMiddle,aEnd)` z vektoru `aFromVector` do vektoru `aToVector`.
 *  \param[in] aFromVector Ukazatel na vstupní vektor obsahující již dva setříděné rozsahy elementů
 *  \param[in] aBegin Index prvního rozsahu ve vektoru \p aFromVector
 *  \param[in] aMiddle Index prvního a druhého rozsahu ve vektoru \p aFromVector
 *  \param[in] aEnd Index druhého rozsahu ve vektoru \p aFromVector
 *  \param[out] aToVector Ukazatel na sloučený (zatříděný) vektor elementů
 */
static void mergify(struct TVector* aFromVector, size_t aBegin, size_t aMiddle, size_t aEnd, struct TVector* aToVector)
	{
	//	puts("Mergify:");
	//	for(size_t i = aBegin; i < aMiddle; ++i)
	//		vector_element_store_file(aFromVector->iValues[i], stdout);
	//	fprintf(stdout, "| ");
	//	for(size_t i = aMiddle; i < aEnd; ++i)
	//		vector_element_store_file(aFromVector->iValues[i], stdout);
	//	putchar('\n');

	size_t i = aBegin;
	size_t i1 = aBegin;
	size_t i2 = aMiddle;

	for (; (i1 < aMiddle) && (i2 < aEnd); ++i) // Pokud jsou prvky v obou
		{
		if (vector_compare_positions(aFromVector, i1, aFromVector, i2) <= 0)
			vector_move_positions(aToVector, i, aFromVector, i1++);
		else
			vector_move_positions(aToVector, i, aFromVector, i2++);
		}

	// Prvky jsou v levé straně
	for (; i1 < aMiddle; ++i) 
		vector_move_positions(aToVector, i, aFromVector, i1++);
	
	// Prvky jsou na pravé straně
	for (; i2 < aEnd; ++i)
		vector_move_positions(aToVector, i, aFromVector, i2++);



//	for(size_t i = aBegin; i < aEnd; ++i)
//		vector_element_store_file(aToVector->iValues[i], stdout);
//	putchar('\n');
	}

/** \brief Vnitřní a lokální funkce realizující rekurzivní funkci pro metodu (Merge Sort)
 *  \details Realizuje rekurzivní funkci pro třídicí metodu (Merge Sort), setřídí elementy vektoru \p aFromVector v rozsahu pozic `<aBegin,aEnd)` a ukládá je do vektoru \p aToVector.
 *  \param[in] aFromVector Ukazatel na tříděný vektor hodnot
 *  \param[in] aBegin Index levého elementu v rozsahu vektoru \p aFromVector
 *  \param[in] aEnd Index pravého elementu v rozsahu vektoru \p aFromVector
 *  \param[out] aToVector Ukazatel na tříděný vektor hodnot
 */
static void merge_sort_worker(struct TVector *aFromVector, size_t aBegin, size_t aEnd, struct TVector *aToVector)
	{
	if((aBegin >= aEnd) || (aEnd - aBegin < 2)) // Podmínka ukončení rekurze
		return;

	const size_t middle = aBegin + (aEnd - aBegin) / 2;


	merge_sort_worker(aToVector, aBegin, middle, aFromVector);
	merge_sort_worker(aToVector, middle, aEnd, aFromVector);

	mergify(aFromVector, aBegin, middle, aEnd, aToVector); // Zatřizujeme z aFromVector do aToVector, tím
	}

void merge_sort(struct TVector *aVector)
	{
	if(vector_size(aVector) < 2)
		return;

	struct TVector work_vector = { 0 };
	if(!vector_attach_sortstats(aVector, &work_vector))	// Create sortstats relationship between `aVector` and `work_vector`
		return;

	if(!vector_clone(aVector, &work_vector))			// Copy all element values from `aVector` to the `work_vector`
		return;

	merge_sort_worker(&work_vector, 0, vector_size(aVector), aVector);

	vector_destroy(&work_vector);
	}
