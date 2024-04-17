/** \file TSetSortedFlexArray.c
 *  \brief Implementace API pro typ SetSortedFlexArray (Flexibilní, tzv. otevřené, pole pro realizaci množiny pomocí setříděného dynamicky alokovaného pole)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TSetSortedFlexArray.c 2703 2024-04-12 15:38:04Z petyovsky $
 */

#define LINEAR_SEARCH_OPTIMIZED	1	///< Volba pro vyhledávání s optimalizací dřívějšího ukončení
#define BINARY_SEARCH			2	///< Volba pro binární vyhledávání

#define FLEX_ARRAY_SEARCH_VARIANT	LINEAR_SEARCH_OPTIMIZED	///< Varianta využívající lineární vyhledávání s optimalizací ve funkci: `set_flex_array_search()`
//#define FLEX_ARRAY_SEARCH_VARIANT	BINARY_SEARCH			///< Varianta využívající binární vyhledávání ve funkci: `set_flex_array_search()`

#include <stdlib.h>
#include <assert.h>
#include "TSetSortedFlexArray.h"
#include "check.h"

/** \brief Úplná definice privátního typu SetSortedFlexArray
 *  \details Privátní typ SetSortedFlexArray (nedostupný mimo funkce z tohoto zdrojového souboru) reprezentuje typ pro tzv. flexibilní (otevřené) pole hodnot typu SetElement.
 */
struct TSetSortedFlexArray
	{
	size_t iCapacity;			///< Skutečně naalokovaná kapacita prvků vnitřního dynamického pole
	TSetElement iElement[];		///< Flexibilní (otevřené) pole hodnot typu SetElement (nedostupné mimo funkce ze souboru TSetSortedFlexArray.c)
	};

int set_flex_array_search(const struct TSetSortedFlexArray *aFlexArray, TSetElement aValue, size_t *aPosPtr, size_t aSize)
	{
	assert(aPosPtr);

	if(!aSize)
		{
		*aPosPtr = 0;
		return -2;
		}

	assert(aFlexArray);

#if FLEX_ARRAY_SEARCH_VARIANT == LINEAR_SEARCH_OPTIMIZED

	/* Zde implementujte lineární vyhledávání s optimalizací dřívějšího ukončení */
	return -2;

#elif FLEX_ARRAY_SEARCH_VARIANT == BINARY_SEARCH

	/* Zde implementujte binární vyhledávání */
	return -2;

#else
	#error Missing correct FLEX_ARRAY_SEARCH_VARIANT symbol definition
#endif
	}

bool set_flex_array_insert(struct TSetSortedFlexArray **aFlexArrayPtr, TSetElement aValue, size_t aSize)
	{
	assert(aFlexArrayPtr);
	struct TSetSortedFlexArray *act_array = *aFlexArrayPtr;

	/* Zde implementujte amortizované vložení prvku do flexibilního dynamicky alokovaného pole na správnou pozici. */
	/* Využijte servisní funkci: set_flex_array_search() */

	return false;
	}

bool set_flex_array_erase(struct TSetSortedFlexArray **aFlexArrayPtr, TSetElement aValue, size_t aSize)
	{
	assert(aFlexArrayPtr);
	assert(aSize);

	struct TSetSortedFlexArray *act_array = *aFlexArrayPtr;

	/* Zde implementujte amortizované odebrání prvku ze správného místa ve flexibilním dynamicky alokovaném poli. */
	/* Využijte servisní funkci: set_flex_array_search() */

	return false;
	}

void set_flex_array_destroy(struct TSetSortedFlexArray *aFlexArray)
	{
	free(aFlexArray);
	}

size_t set_flex_array_begin_pos(const struct TSetSortedFlexArray *aFlexArray)
	{
	return 0;
	}

bool set_flex_array_is_valid_pos(const struct TSetSortedFlexArray *aFlexArray, size_t aPos, size_t aSize)
	{
	if(aFlexArray)
		return (aSize <= aFlexArray->iCapacity) && (aPos < aSize);

	return false;
	}

size_t set_flex_array_next_pos(const struct TSetSortedFlexArray *aFlexArray, size_t aPos)
	{
	return ++aPos;
	}

TSetElement set_flex_array_value_at_pos(const struct TSetSortedFlexArray *aFlexArray, size_t aPos)
	{
	assert(aFlexArray);
	assert(aPos < aFlexArray->iCapacity);

	return aFlexArray->iElement[aPos];
	}
