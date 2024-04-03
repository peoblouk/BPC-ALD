/** \file TVector.c
 *  \brief Implementace API typu Sortable Vector with shareable sortstats
 *  \author Petyovský
 *  \version 2024
 *  $Id: TVector.c 2669 2024-03-25 13:23:11Z petyovsky $
 */

#include <stdlib.h>
#include <assert.h>
#include "TVector.h"
#include "check.h"

bool vector_attach_sortstats(const struct TVector *aFromVector, struct TVector *aToVector)
	{
	if(!aToVector || !aFromVector)
		return false;

	if(aToVector == aFromVector)
		return true;

	if(!aFromVector->iStats)
		return false;

	if(aToVector->iValues && aToVector->iValues == aFromVector->iValues)
		return false;	// Dangling iValues pointers to same valid memory!!

	if(!aToVector->iStatsAttached)
		free(aToVector->iStats);

	aToVector->iStatsAttached = true;
	aToVector->iStats = aFromVector->iStats;
	return true;
	}

bool vector_init(struct TVector *aVector, size_t aSize)
	{
	if(!aVector)
		return false;

	const bool stats_attached = aVector->iStatsAttached;
	struct TVectorSortStats *stats = aVector->iStats;
	if(!stats_attached)
		{
		stats = calloc(1, sizeof(struct TVectorSortStats));
		if(!stats)
			return false;
		}
	else
		if(!stats)
			return false;

	TVectorElement *new_values = NULL;
	if(aSize)
		{
		new_values = calloc(aSize, sizeof(TVectorElement));
		if(!new_values)
			{
			if(!stats_attached)
				free(stats);
			return false;
			}
		}

	*aVector = (struct TVector) { .iValues = new_values, .iSize = aSize, .iStats = stats, .iStatsAttached = stats_attached };
	return true;
	}

bool vector_init_random(struct TVector *aVector, size_t aSize)
	{
	if(!aVector)
		return false;

	const bool stats_attached = aVector->iStatsAttached;
	struct TVectorSortStats *stats = aVector->iStats;
	if(!stats_attached)
		{
		stats = calloc(1, sizeof(struct TVectorSortStats));
		if(!stats)
			return false;
		}
	else
		if(!stats)
			return false;

	TVectorElement *new_values = NULL;
	if(aSize)
		{
		new_values = malloc(aSize * sizeof(TVectorElement));
		if(!new_values)
			{
			if(!stats_attached)
				free(stats);
			return false;
			}

		for(size_t i = 0; i < aSize; ++i)
			new_values[i] = vector_element_random_value();
		}

	*aVector = (struct TVector) { .iValues = new_values, .iSize = aSize, .iStats = stats, .iStatsAttached = stats_attached };
	return true;
	}

bool vector_init_file(struct TVector *aVector, FILE *aInputFile)
	{
	if(!aVector || !aInputFile)
		return false;

	size_t new_size = 0;
	if(fscanf(aInputFile, "%zu", &new_size) != 1)
		return false;

	const bool stats_attached = aVector->iStatsAttached;
	struct TVectorSortStats *stats = aVector->iStats;
	if(!stats_attached)
		{
		stats = calloc(1, sizeof(struct TVectorSortStats));
		if(!stats)
			return false;
		}
	else
		if(!stats)
			return false;

	TVectorElement *new_values = NULL;
	if(new_size)
		{
		new_values = malloc(new_size * sizeof(TVectorElement));
		if(!new_values)
			{
			if(!stats_attached)
				free(stats);
			return false;
			}

		for(size_t i = 0; i < new_size; ++i)
			if(!vector_element_load_file(&new_values[i], aInputFile))
				{
				if(!stats_attached)
					free(stats);
				free(new_values);
				return false;
				}
		}

	*aVector = (struct TVector) { .iValues = new_values, .iSize = new_size, .iStats = stats, .iStatsAttached = stats_attached };
	return true;
	}

bool vector_clone(const struct TVector *aVector, struct TVector *aVectorClone)
	{
	if(!aVector || !aVectorClone)
		return false;

	if(aVector == aVectorClone)
		return true;

	assert((!aVector->iValues && !aVector->iSize) || (aVector->iValues && aVector->iSize));

	if(aVector->iValues && aVector->iValues == aVectorClone->iValues)
		return false;	// Dangling iValues pointers to the same valid memory!!

	const bool stats_attached = aVectorClone->iStatsAttached;
	struct TVectorSortStats *stats = aVectorClone->iStats;
	if(!stats_attached)
		{
		if(!stats)
			{
			if(!aVector->iStats)
				stats = calloc(1, sizeof(struct TVectorSortStats));
			else
				stats = malloc(sizeof(struct TVectorSortStats));

			if(!stats)
				return false;
			}

		if(aVector->iStats)
			*stats = *aVector->iStats;
		}
	else
		if(!stats)
			return false;

	TVectorElement *new_values = NULL;
	if(aVector->iSize)
		{
		new_values = malloc(aVector->iSize * sizeof(TVectorElement));
		if(!new_values)
			{
			if(!stats_attached)
				free(stats);
			return false;
			}

		for(size_t i = 0; i < aVector->iSize; ++i)
			new_values[i] = aVector->iValues[i];
		}

	free(aVectorClone->iValues);
	*aVectorClone = (struct TVector) { .iValues = new_values, .iSize = aVector->iSize, .iStats = stats, .iStatsAttached = stats_attached };
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

static inline TVectorElement vector_fast_value(const struct TVector *aVector, size_t aPos)
	{
	return aVector->iValues[aPos];
	}

TVectorElement vector_value(const struct TVector *aVector, size_t aPos)
	{
	assert(aVector);
	assert(aVector->iValues);
	assert(aVector->iSize > aPos);

	++aVector->iStats->iByPosition.iGets;
	return vector_fast_value(aVector, aPos);
	}

static inline void vector_fast_set_value(struct TVector *aVector, size_t aPos, TVectorElement aValue)
	{
	aVector->iValues[aPos] = aValue;
	}

void vector_set_value(struct TVector *aVector, size_t aPos, TVectorElement aValue)
	{
	assert(aVector);
	assert(aVector->iValues);
	assert(aVector->iSize > aPos);

	++aVector->iStats->iByPosition.iSets;
	vector_fast_set_value(aVector, aPos, aValue);
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

static inline void vector_fast_move_positions(struct TVector *aToVector, size_t aToPos, const struct TVector *aFromVector, size_t aFromPos)
	{
	aToVector->iValues[aToPos] = aFromVector->iValues[aFromPos];
	}

void vector_move_positions(struct TVector *aToVector, size_t aToPos, const struct TVector *aFromVector, size_t aFromPos)
	{
	assert(aToVector);
	assert(aToVector->iSize > aToPos);
	assert(aFromVector);
	assert(aFromVector->iSize > aFromPos);

	++aToVector->iStats->iByPosition.iMoves;
	if(aToVector->iStats != aFromVector->iStats)
		++aFromVector->iStats->iByPosition.iMoves;

	vector_fast_move_positions(aToVector, aToPos, aFromVector, aFromPos);
	}

static inline int vector_fast_compare_positions(const struct TVector *aLeftVector, size_t aLeftPos, const struct TVector *aRightVector, size_t aRightPos)
	{
	return vector_element_comparator(&aLeftVector->iValues[aLeftPos], &aRightVector->iValues[aRightPos]);
	}

int vector_compare_positions(const struct TVector *aLeftVector, size_t aLeftPos, const struct TVector *aRightVector, size_t aRightPos)
	{
	assert(aLeftVector);
	assert(aLeftVector->iSize > aLeftPos);
	assert(aRightVector);
	assert(aRightVector->iSize > aRightPos);

	++aLeftVector->iStats->iByPosition.iComparisons;
	if(aLeftVector->iStats != aRightVector->iStats)
		++aRightVector->iStats->iByPosition.iComparisons;

	return vector_fast_compare_positions(aLeftVector, aLeftPos, aRightVector, aRightPos);
	}

static inline int vector_fast_compare_position_value(const struct TVector *aVector, size_t aPos, TVectorElement aValue)
	{
	return vector_element_comparator(&aVector->iValues[aPos], &aValue);
	}

int vector_compare_position_value(const struct TVector *aVector, size_t aPos, TVectorElement aValue)
	{
	assert(aVector);
	assert(aVector->iSize > aPos);
	++aVector->iStats->iByPosition.iComparisons;

	return vector_fast_compare_position_value(aVector, aPos, aValue);
	}

static inline void vector_fast_swap_positions(struct TVector *aLeftVector, size_t aLeftPos, struct TVector *aRightVector, size_t aRightPos)
	{
	TVectorElement tmp = aLeftVector->iValues[aLeftPos];
	aLeftVector->iValues[aLeftPos] = aRightVector->iValues[aRightPos];
	aRightVector->iValues[aRightPos] = tmp;
	}

void vector_swap_positions(struct TVector *aLeftVector, size_t aLeftPos, struct TVector *aRightVector, size_t aRightPos)
	{
	assert(aLeftVector);
	assert(aLeftVector->iSize > aLeftPos);
	assert(aRightVector);
	assert(aRightVector->iSize > aRightPos);

	vector_fast_swap_positions(aLeftVector, aLeftPos, aRightVector, aRightPos);

	++aLeftVector->iStats->iByPosition.iSwaps;
	if(aLeftVector->iStats != aRightVector->iStats)
		++aRightVector->iStats->iByPosition.iSwaps;
	}

static inline void vector_fast_swap_position_value(struct TVector *aVector, size_t aPos, TVectorElement *aValue)
	{
	TVectorElement tmp = aVector->iValues[aPos];
	aVector->iValues[aPos] = *aValue;
	*aValue = tmp;
	}

void vector_swap_position_value(struct TVector *aVector, size_t aPos, TVectorElement *aValue)
	{
	assert(aVector);
	assert(aVector->iSize > aPos);
	assert(aValue);

	vector_fast_swap_position_value(aVector, aPos, aValue);

	++aVector->iStats->iByPosition.iSwaps;
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

static inline void vector_fast_print_sortstats_item(const struct TVectorSortStatsItem *aSortStatsItem)
	{
	printf(
			" Comparisons: %zu\n"
			" Swaps: %zu\n"
			" Moves: %zu\n"
			" Value Gets: %zu\n"
			" Value Sets: %zu\n",

			aSortStatsItem->iComparisons,
			aSortStatsItem->iSwaps,
			aSortStatsItem->iMoves,
			aSortStatsItem->iGets,
			aSortStatsItem->iSets
		);
	}

bool vector_print_sortstats(const struct TVector *aVector)
	{
	if(!aVector)
		return false;

	if(!aVector->iStats)
		return false;

	printf("\nOperations using positions:\n");
	vector_fast_print_sortstats_item(&aVector->iStats->iByPosition);

	printf("\nOperations using iterators:\n");
	vector_fast_print_sortstats_item(&aVector->iStats->iByIterator);

	return true;
	}

void vector_destroy(struct TVector *aVector)
	{
	if(aVector)
		{
		free(aVector->iValues);
		if(!aVector->iStatsAttached)
			free(aVector->iStats);
		*aVector = (struct TVector) { .iValues = NULL, .iSize = 0, .iStats = NULL, .iStatsAttached = false };
		}
	}

struct TVectorIterator vector_iterator_begin(const struct TVector *aVector)
	{
	if(aVector && aVector->iSize)
		return (struct TVectorIterator) { .iVector = (struct TVector *) aVector, .iPos = 0 };

	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

struct TVectorIterator vector_iterator_pos(const struct TVector *aVector, size_t aPos)
	{
	if(aVector && aVector->iSize > aPos)
		return (struct TVectorIterator) { .iVector = (struct TVector *) aVector, .iPos = aPos };

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

	const struct TVector *vector = aIter->iVector;
	++vector->iStats->iByIterator.iGets;

	return vector_fast_value(vector, aIter->iPos);
	}

bool vector_iterator_set_value(const struct TVectorIterator *aIter, TVectorElement aValue)
	{
	if(!vector_iterator_is_valid(aIter))
		return false;

	struct TVector * const vector = aIter->iVector;
	++vector->iStats->iByIterator.iSets;

	vector_fast_set_value(vector, aIter->iPos, aValue);
	return true;
	}

void vector_move_with_iterators(const struct TVectorIterator *aToIter, const struct TVectorIterator *aFromIter)
	{
	assert(vector_iterator_is_valid(aToIter));
	assert(vector_iterator_is_valid(aFromIter));

	struct TVector * const to_vector = aToIter->iVector;
	struct TVector * const from_vector = aFromIter->iVector;

	++to_vector->iStats->iByIterator.iMoves;
	if(to_vector->iStats != from_vector->iStats)
		++from_vector->iStats->iByIterator.iMoves;

	vector_fast_move_positions(to_vector, aToIter->iPos, from_vector, aFromIter->iPos);
	}

int vector_compare_with_iterators(const struct TVectorIterator *aLeftIter, const struct TVectorIterator *aRightIter)
	{
	assert(vector_iterator_is_valid(aLeftIter));
	assert(vector_iterator_is_valid(aRightIter));

	const struct TVector *left_vector = aLeftIter->iVector;
	const struct TVector *right_vector = aRightIter->iVector;

	++left_vector->iStats->iByIterator.iComparisons;
	if(left_vector->iStats != right_vector->iStats)
		++right_vector->iStats->iByIterator.iComparisons;

	return vector_fast_compare_positions(left_vector, aLeftIter->iPos, right_vector, aRightIter->iPos);
	}

int vector_compare_with_iterator_value(const struct TVectorIterator *aIter, TVectorElement aValue)
	{
	assert(vector_iterator_is_valid(aIter));

	const struct TVector *vector = aIter->iVector;
	++vector->iStats->iByIterator.iComparisons;

	return vector_fast_compare_position_value(vector, aIter->iPos, aValue);
	}

void vector_swap_with_iterators(const struct TVectorIterator *aLeftIter, const struct TVectorIterator *aRightIter)
	{
	assert(vector_iterator_is_valid(aLeftIter));
	assert(vector_iterator_is_valid(aRightIter));

	struct TVector * const left_vector = aLeftIter->iVector;
	struct TVector * const right_vector = aRightIter->iVector;
	vector_fast_swap_positions(left_vector, aLeftIter->iPos, right_vector, aRightIter->iPos);

	++left_vector->iStats->iByIterator.iSwaps;
	if(left_vector->iStats != right_vector->iStats)
		++right_vector->iStats->iByIterator.iSwaps;
	}

void vector_swap_with_iterator_value(const struct TVectorIterator *aIter, TVectorElement *aValue)
	{
	assert(vector_iterator_is_valid(aIter));
	assert(aValue);

	struct TVector * const vector = aIter->iVector;
	vector_fast_swap_position_value(vector, aIter->iPos, aValue);

	++vector->iStats->iByIterator.iSwaps;
	}

bool vector_iterators_are_same(const struct TVectorIterator *aLeftIter, const struct TVectorIterator *aRightIter)
	{
	bool left_iter_is_valid = vector_iterator_is_valid(aLeftIter);
	bool right_iter_is_valid = vector_iterator_is_valid(aRightIter);

	if(left_iter_is_valid != right_iter_is_valid)
		return false;

	if(!left_iter_is_valid && !right_iter_is_valid)
		return true;

	return (aLeftIter->iVector == aRightIter->iVector) && (aLeftIter->iPos == aRightIter->iPos);
	}
