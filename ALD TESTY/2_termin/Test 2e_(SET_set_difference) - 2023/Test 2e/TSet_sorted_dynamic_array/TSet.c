/** \file TSet.c
 *  \brief Implementace API pro typ Set (realizace množiny pomocí setříděného flexibilního dynamicky alokovaného pole)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TSet.c 2703 2024-04-12 15:38:04Z petyovsky $
 */

#include "TSet.h"
#include "check.h"

void set_init(struct TSet *aSet)
	{
	if(aSet)
		*aSet = (struct TSet) { .iSize = 0, .iFlexArray = NULL };
	}

size_t set_size(const struct TSet *aSet)
	{
	if(aSet)
		return aSet->iSize;
	return 0;
	}

bool set_insert(struct TSet *aSet, TSetElement aValue)
	{
	if(!aSet)
		return false;

	if(!set_flex_array_insert(&aSet->iFlexArray, aValue, aSet->iSize))
		return false;

	++aSet->iSize;
	return true;
	}

bool set_erase(struct TSet *aSet, TSetElement aValue)
	{
	if(!set_size(aSet))
		return false;

	if(!set_flex_array_erase(&aSet->iFlexArray, aValue, aSet->iSize))
		return false;

	--aSet->iSize;
	return true;
	}

bool set_is_element_of(const struct TSet *aSet, TSetElement aValue)
	{
	if(!set_size(aSet))
		return false;

	size_t pos = 0;
	return set_flex_array_search(aSet->iFlexArray, aValue, &pos, aSet->iSize) == 0;
	}

struct TSetIterator set_find(const struct TSet *aSet, TSetElement aValue)
	{
	if(set_size(aSet))
		{
		size_t pos = 0;
		if(set_flex_array_search(aSet->iFlexArray, aValue, &pos, aSet->iSize) == 0)
			return (struct TSetIterator) { .iSet = aSet, .iPos = pos };	// success
		}

	return (struct TSetIterator) { .iSet = NULL, .iPos = 0 };	// fail
	}

bool set_difference(struct TSet* aNewSet, const struct TSet* aLeftSet, const struct TSet* aRightSet)
	{
	if (!aNewSet && !aLeftSet && !aRightSet)
		return true;
	else if (!aNewSet || !aLeftSet || !aRightSet || aNewSet->iSize)
		return false;

	size_t n = 0, l = 0, r = 0;

	while (l < aLeftSet->iSize && r < aRightSet->iSize)
		{
		TSetElement l_value = set_flex_array_value_at_pos(aLeftSet->iFlexArray, l);
		TSetElement r_value = set_flex_array_value_at_pos(aRightSet->iFlexArray, r);
		int cmp = set_element_comparator(&l_value, &r_value);

		if (cmp < 0)
			{
			if (!set_insert(aNewSet, l_value))
				return false;
			++l;
			}
		else if (cmp > 0)
			++r;
		else
			{
			++l;
			++r;
			}
		}

	while (l < aLeftSet->iSize)
		if (!set_insert(aNewSet, set_flex_array_value_at_pos(aLeftSet->iFlexArray, l++)))
			return false;

	return true;
	}

void set_destroy(struct TSet *aSet)
	{
	if(!aSet)
		return;

	set_flex_array_destroy(aSet->iFlexArray);
	*aSet = (struct TSet) { .iSize = 0, .iFlexArray = NULL };
	}

struct TSetIterator set_iterator_begin(const struct TSet *aSet)
	{
	if(aSet)
		return (struct TSetIterator) { .iSet = aSet, .iPos = set_flex_array_begin_pos(aSet->iFlexArray) };

	return (struct TSetIterator) { .iSet = NULL, .iPos = 0 };
	}

bool set_iterator_is_valid(const struct TSetIterator *aIter)
	{
	if(aIter)
		if(aIter->iSet)
			return set_flex_array_is_valid_pos(aIter->iSet->iFlexArray, aIter->iPos, aIter->iSet->iSize);

	return false;
	}

bool set_iterator_to_next(struct TSetIterator *aIter)
	{
	bool valid = set_iterator_is_valid(aIter);
	if(valid)
		{
		aIter->iPos = set_flex_array_next_pos(aIter->iSet->iFlexArray, aIter->iPos);
		valid = set_flex_array_is_valid_pos(aIter->iSet->iFlexArray, aIter->iPos, aIter->iSet->iSize);
		}

	return valid;
	}

TSetElement set_iterator_value(const struct TSetIterator *aIter)
	{
	if(set_iterator_is_valid(aIter))
		return set_flex_array_value_at_pos(aIter->iSet->iFlexArray, aIter->iPos);

	return (TSetElement) { 0 };
	}