/** \file TVector.c
 *  \brief Implementace Basic Vector API
 *  \author Petyovský
 *  \version 2024
 *  $Id: TVector.c 2584 2024-02-05 14:33:25Z petyovsky $
 */

#include <stdlib.h>
#include <assert.h>
#include "TVector.h"
#include "check.h"

bool vector_init(struct TVector *aVector, size_t aSize)
	{
	return false;
	}

bool vector_init_random(struct TVector *aVector, size_t aSize)
	{
	return false;
	}

bool vector_init_file(struct TVector *aVector, FILE *aInputFile)
	{
	return false;
	}

bool vector_clone(const struct TVector *aVector, struct TVector *aVectorClone)
	{
	return false;
	}

bool vector_store_file(const struct TVector *aVector, FILE *aOutputFile)
	{
	return false;
	}

TVectorElement vector_value(const struct TVector *aVector, size_t aPos)
	{
	return (TVectorElement) { 0 };
	}

void vector_set_value(struct TVector *aVector, size_t aPos, TVectorElement aValue)
	{
	}

size_t vector_size(const struct TVector *aVector)
	{
	return 0;
	}

bool vector_resize(struct TVector *aVector, size_t aNewSize)
	{
	return false;
	}

void vector_destroy(struct TVector *aVector)
	{
	}

struct TVectorIterator vector_iterator_begin(const struct TVector *aVector)
	{
	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

struct TVectorIterator vector_iterator_pos(const struct TVector *aVector, size_t aPos)
	{
	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

struct TVectorIterator vector_iterator_last(const struct TVector *aVector)
	{
	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

bool vector_iterator_is_valid(const struct TVectorIterator *aIter)
	{
	return false;
	}

bool vector_iterator_to_next(struct TVectorIterator *aIter)
	{
	return false;
	}

bool vector_iterator_to_prev(struct TVectorIterator *aIter)
	{
	return false;
	}

TVectorElement vector_iterator_value(const struct TVectorIterator *aIter)
	{
	return (TVectorElement) { 0 };
	}

bool vector_iterator_set_value(const struct TVectorIterator *aIter, TVectorElement aValue)
	{
	return false;
	}
