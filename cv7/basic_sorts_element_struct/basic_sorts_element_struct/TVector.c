/** \file TVector.c
 *  \brief Implementace Sortable Vector API
 *  \author Petyovský
 *  \version 2024
 *  $Id: TVector.c 2621 2024-03-08 15:10:53Z petyovsky $
 */

#include <stdlib.h>
#include <assert.h>
#include "TVector.h"
#include "check.h"

bool vector_init(struct TVector *aVector, size_t aSize)
	{
	if(!aVector)
		return false;

	TVectorElement *new_values = NULL;
	if(aSize)
		{
		new_values = calloc(aSize, sizeof(TVectorElement));
		if(!new_values)
			return false;
		}

	*aVector = (struct TVector) { .iValues = new_values, .iSize = aSize };
	return true;
	}

bool vector_init_random(struct TVector *aVector, size_t aSize)
	{
	if(!aVector)
		return false;

	TVectorElement *new_values = NULL;
	if(aSize)
		{
		new_values = malloc(aSize * sizeof(TVectorElement));
		if(!new_values)
			return false;

		for(size_t i = 0; i < aSize; ++i)
			new_values[i] = vector_element_random_value();
		}

	*aVector = (struct TVector) { .iValues = new_values, .iSize = aSize };
	return true;
	}

bool vector_init_file(struct TVector *aVector, FILE *aInputFile)
	{
	if(!aVector || !aInputFile)
		return false;

	size_t new_size = 0;
	if(fscanf(aInputFile, "%zu", &new_size) != 1)
		return false;

	TVectorElement *new_values = NULL;
	if(new_size)
		{
		new_values = malloc(new_size * sizeof(TVectorElement));
		if(!new_values)
			return false;

		for(size_t i = 0; i < new_size; ++i)
			if(!vector_element_load_file(&new_values[i], aInputFile))
				{
				free(new_values);
				return false;
				}
		}

	*aVector = (struct TVector) { .iValues = new_values, .iSize = new_size };
	return true;
	}

bool vector_clone(const struct TVector *aVector, struct TVector *aVectorClone)
	{
	if(!aVector || !aVectorClone)
		return false;

	if(aVector == aVectorClone)
		return true;

	assert((!aVector->iValues && !aVector->iSize) || (aVector->iValues && aVector->iSize));

	TVectorElement *new_values = NULL;
	if(aVector->iSize)
		{
		new_values = malloc(aVector->iSize * sizeof(TVectorElement));
		if(!new_values)
			return false;

		for(size_t i = 0; i < aVector->iSize; ++i)
			new_values[i] = aVector->iValues[i];
		}

	free(aVectorClone->iValues);
	*aVectorClone = (struct TVector) { .iValues = new_values, .iSize = aVector->iSize, .iStats = aVector->iStats };
	return true;
	}

bool vector_store_file(const struct TVector *aVector, FILE *aOutputFile)
	{
	if(!aVector || !aOutputFile)
		return false;

	assert((!aVector->iValues && !aVector->iSize) || (aVector->iValues && aVector->iSize));

	if(fprintf(aOutputFile, "%zu ", aVector->iSize) < 2)
		return false;

	if(aVector->iValues)
		for(size_t i = 0; i < aVector->iSize; ++i)
			if(!vector_element_store_file(aVector->iValues[i], aOutputFile))
				return false;

	return true;
	}

TVectorElement vector_value(const struct TVector *aVector, size_t aPos)
	{
	assert(aVector);
	assert(aVector->iValues);
	assert(aVector->iSize > aPos);

	++((struct TVector *) aVector)->iStats.iGets;
	return aVector->iValues[aPos];
	}

void vector_set_value(struct TVector *aVector, size_t aPos, TVectorElement aValue)
	{
	assert(aVector);
	assert(aVector->iValues);
	assert(aVector->iSize > aPos);

	++aVector->iStats.iSets;
	aVector->iValues[aPos] = aValue;
	}

size_t vector_size(const struct TVector *aVector)
	{
	if(aVector)
		if(aVector->iValues)
			return aVector->iSize;
	return 0;
	}

bool vector_resize(struct TVector *aVector, size_t aNewSize)
	{
	return false;
	}

void vector_move_positions(struct TVector *aToVector, size_t aToPos, const struct TVector *aFromVector, size_t aFromPos)
	{
	assert(aToVector);
	assert(aToVector->iSize > aToPos);
	assert(aFromVector);
	assert(aFromVector->iSize > aFromPos);

	++aToVector->iStats.iMoves;
	if(aToVector != aFromVector)
		++((struct TVector *) aFromVector)->iStats.iMoves;

	aToVector->iValues[aToPos] = aFromVector->iValues[aFromPos];
	}

int vector_compare_positions(const struct TVector *aLeftVector, size_t aLeftPos, const struct TVector *aRightVector, size_t aRightPos)
	{
	assert(aLeftVector);
	assert(aLeftVector->iSize > aLeftPos);
	assert(aRightVector);
	assert(aRightVector->iSize > aRightPos);

	++((struct TVector *) aLeftVector)->iStats.iComparisons;
	if(aLeftVector != aRightVector)
		++((struct TVector *) aRightVector)->iStats.iComparisons;

	return vector_element_comparator(&aLeftVector->iValues[aLeftPos], &aRightVector->iValues[aRightPos]);
	}

int vector_compare_position_value(const struct TVector *aVector, size_t aPos, TVectorElement aValue)
	{
	assert(aVector);
	assert(aVector->iSize > aPos);
	++((struct TVector *) aVector)->iStats.iComparisons;

	return vector_element_comparator(&aVector->iValues[aPos], &aValue);
	}

void vector_swap_positions(struct TVector *aLeftVector, size_t aLeftPos, struct TVector *aRightVector, size_t aRightPos)
	{
	assert(aLeftVector);
	assert(aLeftVector->iSize > aLeftPos);
	assert(aRightVector);
	assert(aRightVector->iSize > aRightPos);

	TVectorElement tmp = aLeftVector->iValues[aLeftPos];
	aLeftVector->iValues[aLeftPos] = aRightVector->iValues[aRightPos];
	aRightVector->iValues[aRightPos] = tmp;

	++aLeftVector->iStats.iSwaps;
	if(aLeftVector != aRightVector)
		++aRightVector->iStats.iSwaps;
	}

void vector_swap_position_value(struct TVector *aVector, size_t aPos, TVectorElement *aValue)
	{
	assert(aVector);
	assert(aVector->iSize > aPos);
	assert(aValue);

	TVectorElement tmp = aVector->iValues[aPos];
	aVector->iValues[aPos] = *aValue;
	*aValue = tmp;

	++aVector->iStats.iSwaps;
	}

bool vector_is_ascendantly_sorted(const struct TVector *aOriginalVector, const struct TVector *aSortedVector)
	{
	if(!aOriginalVector && !aSortedVector)
		return true;

	if((!aOriginalVector && aSortedVector) || (aOriginalVector && !aSortedVector))
		return false;

	if((!aOriginalVector->iValues && aSortedVector->iValues) || (aOriginalVector->iValues && !aSortedVector->iValues))
		return false;

	if(aOriginalVector->iSize != aSortedVector->iSize)
		return false;

	if(!aOriginalVector->iValues && !aSortedVector->iValues)
		return true;

	struct TVector orig_clone = { 0 };
	if(!vector_clone(aOriginalVector, &orig_clone))
		return false;

	if(!orig_clone.iValues)
		return false;

	qsort(orig_clone.iValues, orig_clone.iSize, sizeof(TVectorElement), (int(*)(const void*, const void*))vector_element_comparator);

	bool is_sorted = true;
	for(size_t i = 0; i < aSortedVector->iSize; ++i)
		if(vector_element_comparator(&orig_clone.iValues[i], &aSortedVector->iValues[i]))
			{
			is_sorted = false;
			break;
			}

	vector_destroy(&orig_clone);
	return is_sorted;
	}

bool vector_print_sortstats(const struct TVector *aVector)
	{
	if(!aVector)
		return false;

	printf("\nComparisons: %zu\n", aVector->iStats.iComparisons);
	printf("Swaps: %zu\n", aVector->iStats.iSwaps);
	printf("Moves: %zu\n", aVector->iStats.iMoves);
	printf("Value Gets: %zu\n", aVector->iStats.iGets);
	printf("Value Sets: %zu\n", aVector->iStats.iSets);
	return true;
	}

void vector_destroy(struct TVector *aVector)
	{
	if(aVector)
		{
		free(aVector->iValues);
		*aVector = (struct TVector) { .iValues = NULL, .iSize = 0 };
		}
	}

struct TVectorIterator vector_iterator_begin(const struct TVector *aVector)
	{
	if(aVector && aVector->iSize)
		return (struct TVectorIterator) { .iVector = (struct TVector*) aVector, .iPos = 0 };

	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

struct TVectorIterator vector_iterator_pos(const struct TVector *aVector, size_t aPos)
	{
	if(aVector && aVector->iSize > aPos)
		return (struct TVectorIterator) { .iVector = (struct TVector*) aVector, .iPos = aPos };

	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

struct TVectorIterator vector_iterator_last(const struct TVector *aVector)
	{
	if(aVector && aVector->iSize)
		return (struct TVectorIterator) { .iVector = (struct TVector *) aVector, .iPos = aVector->iSize - 1 };

	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

bool vector_iterator_is_valid(const struct TVectorIterator *aIter)
	{
	if(aIter)
		if(aIter->iVector)
				if(aIter->iPos < aIter->iVector->iSize)
					return true;
	return false;
	}

bool vector_iterator_to_next(struct TVectorIterator *aIter)
	{
	if(!vector_iterator_is_valid(aIter))
		return false;

	++aIter->iPos;
	if(aIter->iPos >= aIter->iVector->iSize)
		{
		*aIter = (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
		return false;
		}

	return true;
	}

bool vector_iterator_to_prev(struct TVectorIterator *aIter)
	{
	if(!vector_iterator_is_valid(aIter))
		return false;

	if(aIter->iPos == 0)
		{
		*aIter = (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
		return false;
		}

	--aIter->iPos;
	return true;
	}

TVectorElement vector_iterator_value(const struct TVectorIterator *aIter)
	{
	if(!vector_iterator_is_valid(aIter))
		return (TVectorElement) { 0 };

	return aIter->iVector->iValues[aIter->iPos];
	}

bool vector_iterator_set_value(const struct TVectorIterator *aIter, TVectorElement aValue)
	{
	if(!vector_iterator_is_valid(aIter))
		return false;

	aIter->iVector->iValues[aIter->iPos] = aValue;
	return true;
	}

void vector_move_with_iterators(const struct TVectorIterator *aToIter, const struct TVectorIterator *aFromIter)
	{
	assert(vector_iterator_is_valid(aToIter));
	assert(vector_iterator_is_valid(aFromIter));
	vector_move_positions(aToIter->iVector, aToIter->iPos, aFromIter->iVector, aFromIter->iPos);
	}

int vector_compare_with_iterators(const struct TVectorIterator *aLeftIter, const struct TVectorIterator *aRightIter)
	{
	assert(vector_iterator_is_valid(aLeftIter));
	assert(vector_iterator_is_valid(aRightIter));
	return vector_compare_positions(aLeftIter->iVector, aLeftIter->iPos, aRightIter->iVector, aRightIter->iPos);
	}

int vector_compare_with_iterator_value(const struct TVectorIterator *aIter, TVectorElement aValue)
	{
	assert(vector_iterator_is_valid(aIter));
	return vector_compare_position_value(aIter->iVector, aIter->iPos, aValue);
	}

void vector_swap_with_iterators(const struct TVectorIterator *aLeftIter, const struct TVectorIterator *aRightIter)
	{
	assert(vector_iterator_is_valid(aLeftIter));
	assert(vector_iterator_is_valid(aRightIter));
	vector_swap_positions(aLeftIter->iVector, aLeftIter->iPos, aRightIter->iVector, aRightIter->iPos);
	}

void vector_swap_with_iterator_value(const struct TVectorIterator *aIter, TVectorElement *aValue)
	{
	assert(vector_iterator_is_valid(aIter));
	vector_swap_position_value(aIter->iVector, aIter->iPos, aValue);
	}
