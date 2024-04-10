/** \file THeap.c
 *  \brief Implementace API typu Heap pro přístup k elementům vektoru jako k uzlům haldy (Sortable Vector with shareable sortstats)
 *  \author Petyovský
 *  \version 2024
 *  $Id: THeap.c 2670 2024-03-25 14:21:39Z petyovsky $
 */

#include <stdlib.h>
#include <assert.h>
#include "THeap.h"

bool heap_iterator_to_left(struct THeapIterator *aIter)
	{
	if(!heap_iterator_is_valid(aIter))
		return false;

	aIter->iVectorIter.iPos = heap_pos_to_left(aIter->iVectorIter.iPos);
	if(aIter->iVectorIter.iPos >= aIter->iVectorIter.iVector->iSize)
		{
		*aIter = (struct THeapIterator) { .iVectorIter = (struct TVectorIterator) { .iVector = NULL, .iPos = 0 } };
		return false;
		}

	return true;
	}

bool heap_iterator_to_right(struct THeapIterator *aIter)
	{
	if(!heap_iterator_is_valid(aIter))
		return false;

	aIter->iVectorIter.iPos = heap_pos_to_right(aIter->iVectorIter.iPos);
	if(aIter->iVectorIter.iPos >= aIter->iVectorIter.iVector->iSize)
		{
		*aIter = (struct THeapIterator) { .iVectorIter = (struct TVectorIterator) { .iVector = NULL, .iPos = 0 } };
		return false;
		}

	return true;
	}

bool heap_iterator_to_parent(struct THeapIterator *aIter)
	{
	if(!heap_iterator_is_valid(aIter))
		return false;

	if(!aIter->iVectorIter.iPos)
		{
		*aIter = (struct THeapIterator) { .iVectorIter = (struct TVectorIterator) { .iVector = NULL, .iPos = 0 } };
		return false;
		}

	aIter->iVectorIter.iPos = heap_pos_to_parent(aIter->iVectorIter.iPos);
	return true;
	}
