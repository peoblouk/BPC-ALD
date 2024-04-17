/** \file TSet.c
 *  \brief Implementace API pro typ Set (realizace množiny pomocí nesetříděného flexibilního dynamicky alokovaného pole)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TSet.c 2704 2024-04-12 15:39:57Z petyovsky $
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
