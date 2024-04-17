/** \file TSetUnsortedFlexArray.c
 *  \brief Implementace API pro typ SetUnsortedFlexArray (Flexibilní, tzv. otevřené, pole pro realizaci množiny pomocí netříděného dynamicky alokovaného pole)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TSetUnsortedFlexArray.c 2704 2024-04-12 15:39:57Z petyovsky $
 */

#include <stdlib.h>
#include <assert.h>
#include "TSetUnsortedFlexArray.h"
#include "check.h"

/** \brief Úplná definice privátního typu SetUnsortedFlexArray
 *  \details Privátní typ SetUnsortedFlexArray (nedostupný mimo funkce z tohoto zdrojového souboru) reprezentuje typ pro tzv. flexibilní (otevřené) pole hodnot typu SetElement.
 */
struct TSetUnsortedFlexArray
	{
	size_t iCapacity;			///< Skutečně naalokovaná kapacita prvků vnitřního dynamického pole
	TSetElement iElement[];		///< Flexibilní (otevřené) pole hodnot typu SetElement (nedostupné mimo funkce ze souboru TSetUnsortedFlexArray.c)
	};

int set_flex_array_search(const struct TSetUnsortedFlexArray *aFlexArray, TSetElement aValue, size_t *aPosPtr, size_t aSize)
	{
	assert(aPosPtr);

	size_t pos = 0;
	int cmp = -2;

	if(aSize)
		{
		assert(aFlexArray);
		for(size_t i = 0; (cmp != 0) && (i < aSize); pos = i++)
			cmp = set_element_comparator(&aValue, &aFlexArray->iElement[i]);
		}

	*aPosPtr = pos;
	return cmp;
	}

bool set_flex_array_insert(struct TSetUnsortedFlexArray **aFlexArrayPtr, TSetElement aValue, size_t aSize)
	{
	assert(aFlexArrayPtr);
	struct TSetUnsortedFlexArray *act_array = *aFlexArrayPtr;

	size_t pos = 0;
	if(set_flex_array_search(act_array, aValue, &pos, aSize) == 0)
		return false;

	if( (!aSize) || (aSize >= act_array->iCapacity) )
		{
		const size_t new_capacity = (aSize) ? (2 * act_array->iCapacity) : (2);
		struct TSetUnsortedFlexArray *new_array =
			malloc(offsetof(struct TSetUnsortedFlexArray, iElement) + new_capacity * sizeof(TSetElement));
		//	malloc(sizeof(struct TSetUnsortedFlexArray) + new_capacity * sizeof(TSetElement));
		//	https://gustedt.wordpress.com/2011/03/14/flexible-array-member/
		//	https://en.cppreference.com/w/c/types/offsetof

		if(!new_array)
			return false;

		new_array->iCapacity = new_capacity;
		for(size_t i = 0; i < aSize; ++i)
			new_array->iElement[i] = act_array->iElement[i];

		*aFlexArrayPtr = new_array;
		free(act_array);
		act_array = new_array;
		}

	act_array->iElement[aSize] = aValue;
	return true;
	}

bool set_flex_array_erase(struct TSetUnsortedFlexArray **aFlexArrayPtr, TSetElement aValue, size_t aSize)
	{
	assert(aFlexArrayPtr);
	assert(aSize);

	struct TSetUnsortedFlexArray *act_array = *aFlexArrayPtr;

	size_t pos = 0;
	if(set_flex_array_search(act_array, aValue, &pos, aSize) != 0)
		return false;

	size_t new_size = aSize - 1;
	if(!new_size)
		{
		*aFlexArrayPtr = NULL;
		free(act_array);
		return true;
		}

	if(act_array->iCapacity / aSize > 4)
		{
		size_t new_capacity = act_array->iCapacity / 2;
		struct TSetUnsortedFlexArray *new_array =
			malloc(offsetof(struct TSetUnsortedFlexArray, iElement) + new_capacity * sizeof(TSetElement));
		//	malloc(sizeof(struct TSetUnsortedFlexArray) + new_capacity * sizeof(TSetElement));
		//	https://gustedt.wordpress.com/2011/03/14/flexible-array-member/
		//	https://en.cppreference.com/w/c/types/offsetof

		if(!new_array)
			return false;

		new_array->iCapacity = new_capacity;
		for(size_t i = 0; i < pos; ++i)
			new_array->iElement[i] = act_array->iElement[i];

		for(; pos < new_size; ++pos)
			new_array->iElement[pos] = act_array->iElement[pos + 1];

		*aFlexArrayPtr = new_array;
		free(act_array);

		return true;
		}

	for(; pos < new_size; ++pos)
		act_array->iElement[pos] = act_array->iElement[pos + 1];

	return true;
	}

void set_flex_array_destroy(struct TSetUnsortedFlexArray *aFlexArray)
	{
	free(aFlexArray);
	}

size_t set_flex_array_begin_pos(const struct TSetUnsortedFlexArray *aFlexArray)
	{
	return 0;
	}

bool set_flex_array_is_valid_pos(const struct TSetUnsortedFlexArray *aFlexArray, size_t aPos, size_t aSize)
	{
	if(aFlexArray)
		return (aSize <= aFlexArray->iCapacity) && (aPos < aSize);

	return false;
	}

size_t set_flex_array_next_pos(const struct TSetUnsortedFlexArray *aFlexArray, size_t aPos)
	{
	return ++aPos;
	}

TSetElement set_flex_array_value_at_pos(const struct TSetUnsortedFlexArray *aFlexArray, size_t aPos)
	{
	assert(aFlexArray);
	assert(aPos < aFlexArray->iCapacity);

	return aFlexArray->iElement[aPos];
	}
