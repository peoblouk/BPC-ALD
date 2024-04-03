/** \file advanced_sorts.c
 *  \brief Implementace API účinnějších metod třídění
 *  \author Petyovský
 *  \version 2024
 *  $Id: advanced_sorts.c 2680 2024-04-02 10:28:36Z petyovsky $
 */

#include "advanced_sorts.h"
/** \brief Formátovaný tisk hodnoty elementu za předpokladu platné hodnoty
 *  \details Vytiskne formátovaně hodnotu elementu pokud je parametr \p aCondition platný, jinak se vytisknou zástupné znaky.
 *  \param[in] aCondition pokud je \c true bude hodnota vytisknuta, jinak se tisknou zástupné znaky
 *  \param[in] aValue Hodnota elementu k vytisknutí
 */
//[[maybe_unused]]  // Since C23
static inline void print_element_if(bool aCondition, TVectorElement aValue)
	{
	if(aCondition)
		vector_element_store_file(aValue, stdout);
	else
		fprintf(stdout, ". ");
	}

void shell_sort(struct TVector *aVector)
	{
	size_t h = 1;


	for (;;) // Generuj h dokud není h_max <= (počet prvků) / 2
		{
		size_t h_new = 2 * h + 1;
		if (h_new > (aVector->iSize / 2)) 
			break;
		else
			h = h_new;
		}
		printf("h: %zu\n", h);



	//for(size_t x = 0; x < aVector->iSize; ++x)
	//	vector_element_store_file(aVector->iValues[x], stdout);
	//putchar('\n');

	//for(size_t x = 0; x < aVector->iSize; ++x)
	//	print_element_if( (x % h == k) && (x <= i), aVector->iValues[x]);
	//putchar('\n');
	//putchar('\n');

		for (; h > 0; h = (h-1) / 2) // h určuje krok - vzdálenost mezipodsloupnosti
			{
			for (size_t k = 0; k < h; k++) // Projedu posloupnosti s posunem o k
				{
				for (size_t i = k + h; i < aVector->iSize; i += h) // i je index prvniho prvku nesetrizene casti podposloupnosti
					{
					TVectorElement tmp = vector_value(aVector, i); // vyberu hodnotu a porovnávám jí v nesetrizene oblasti
					size_t j = i;
					
					for (; j > k; j -= h) // Projedu nesetřízenou oblastí <- |
						{
						if (vector_compare_position_value(aVector, j - h , tmp) <= 0)
							break;
						vector_move_positions(aVector, j, aVector, j - h);
						}
					vector_set_value(aVector, j, tmp);
					}
				}
			}
	}

/** \brief Vnitřní a lokální funkce realizující rekurzivní funkci pro metodu (Quick Sort)
 *  \details Realizuje rekurzivní funkci pro třídicí metodu (Quick Sort) pro elementy vektoru v rozsahu pozic `<k,l>`.
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 *  \param[in] k Index levého elementu z rozsahu `<k,l>`
 *  \param[in] l Index pravého elementu z rozsahu `<k,l>`
 */
static void quick_sort_worker(struct TVector * /*restrict*/ aVector, size_t k, size_t l) // Rozdělím na dvě části s = (k + l) / 2
	{
	size_t s = (k + l) / 2; // Index pivotu (volím jako střed)
	TVectorElement tmp = vector_value(aVector, s); // vyberu hodnotu a porovnávám jí v nesetrizene oblasti
	size_t i = k; // Průběžné indexy
	size_t j = l; // Průběžné indexy

	do {
		for (; vector_compare_position_value(aVector, i, tmp) < 0; ++i); // Procházím dozadu, pokud najdu číslo větší jak prostřední pivot, tak ukončím
		for (; vector_compare_position_value(aVector, j, tmp) > 0; --j); // Procházím zezadu
		if (i > j)
			break;
		vector_swap_positions(aVector, i, aVector, j); // SWAP
		++i;

		if (j > k)
			--j;
		} while (i <= j);
	
		if (j > k)
			quick_sort_worker(aVector, k, j);
		if (i < l)
			quick_sort_worker(aVector, i, l);
	}

void quick_sort(struct TVector *aVector)
	{
	if(vector_size(aVector) < 2) // Výraz v podmínce vhodně upravte :-) (nemám co třídit, pokud nemám víc jak jeden prvek)
		return;

	quick_sort_worker(aVector, 0, aVector->iSize - 1);
	}
