/** \file heap_sort.c
 *  \brief Implementace API výkonnější metody třídění (Heap sort), využívající poziční API pro přístup k uzlům haldy
 *  \author Petyovský
 *  \version 2024
 *  $Id: heap_sort.c 2691 2024-04-05 14:58:43Z petyovsky $
 */

#include "heap_sort.h"
#include "THeap.h"

/** \brief Vnitřní a lokální funkce zajišťující vložení hodnoty do zvoleného uzlu haldy, při zachování podmínek haldy
 *  \details Lokální funkce zajišťuje vložení hodnoty do zvoleného uzlu haldy, při zachování podmínek haldy (prosívání hodnoty směrem k listům).
 *  \param[in] aValue Hodnota vkládáná do haldy
 *  \param[in,out] aHeap Ukazatel na vytvářenou haldu
 *  \param[in] aParentPos Index uzlu určeného pro vložení hodnoty \p aValue (hodnota v tomto uzlu bude přepsána).
 */
static inline void sift_down(THeapElement aValue, struct THeap *aHeap, size_t aParentPos)
	{
	const size_t heapsize = heap_size(aHeap);

	for (size_t pos = aParentPos; (pos = heap_pos_to_left(pos)) < heapsize; aParentPos = pos)
		{
		const size_t right_pos = heap_pos_to_right(aParentPos);
		// Pokud existuje i pravy potomek, zjisti se, v kterem z potomku je vetsi hodnota
		if ((right_pos < heapsize) && heap_compare_positions(aHeap, pos, aHeap, right_pos) < 0) // Neexistuje další následník pravý
			pos = right_pos;

		if (heap_compare_position_value(aHeap, pos, aValue) > 0) // Hodnota ve vetsim z potomku je vetsi nez vkladana hodnota
			heap_move_positions(aHeap, aParentPos, aHeap, pos);
		else
			// Vkladana hodnota je vetsi nez hodnota v potomcich, je splnena podminka haldy
			break; // Koncime
		}
	heap_set_value(aHeap, aParentPos, aValue);
	}

/** \brief Vnitřní a lokální funkce realizující uspořádání elementů vektoru do haldy
 *  \details Lokální funkce zajišťuje uspořádání elementů vektoru do haldy, pomocí volání funkce sift_down().
 *  \param[in,out] aHeap Ukazatel na vytvářenou haldu
 */
static void heapify(struct THeap *aHeap)
	{
	size_t pos = heap_size(aHeap);
	if (pos < 2)
		return;

	pos = heap_pos_to_parent(pos - 1);

	do {
		sift_down(heap_value(aHeap, pos), aHeap, pos);
		} while (pos--); // Dokud

	}

void heap_sort(struct TVector *aVector)
	{
//	for(size_t i = 0; i < vector_size(aVector); ++i)
//		vector_element_store_file(aVector->iValues[i], stdout);
//	putchar('\n');

	if(vector_size(aVector) < 2)
		return;

	//auto heap = heap_create(*aVector);		// auto since C23
	struct THeap heap = heap_create(*aVector);

//	for(size_t i = 0; i < heap_size(&heap); ++i)
//		vector_element_store_file(heap.iVector.iValues[i], stdout);
//	putchar('\n');

	heapify(&heap);

//	for(size_t i = 0; i < heap_size(&heap); ++i)
//		vector_element_store_file(heap.iVector.iValues[i], stdout);
//	putchar('\n');

//	puts("Heapify stats:");
//	vector_print_sortstats(aVector);

	size_t heapsize = heap_size(&heap);
	while(heapsize > 1)
		{
		const size_t root_pos = 0;
		const size_t last_pos = heapsize - 1;

		const THeapElement value = heap_value(&heap, last_pos);
		heap_move_positions(&heap, last_pos, &heap, root_pos);

		heapsize = heap_decrement_size(&heap);
//		for(size_t i = 0; i < heapsize; ++i)
//			vector_element_store_file(heap.iVector.iValues[i], stdout);
//		putchar('\n');

		sift_down(value, &heap, root_pos);

//		for(size_t i = 0; i < heapsize; ++i)
//			vector_element_store_file(heap.iVector.iValues[i], stdout);
//		puts("\n-------");
		}
	}
