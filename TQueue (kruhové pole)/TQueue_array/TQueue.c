/** \file TQueue.c
 *  \brief Implementace API pro typ fronta (realizace pomocí kruhového pole)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TQueue.c 2615 2024-02-16 15:36:07Z petyovsky $
 */

#include <assert.h>
#include "TQueue.h"

void queue_init(struct TQueue *aQueue)
	{
	}

bool queue_is_empty(const struct TQueue *aQueue)
	{
	return true;
	}

bool /* TQueueIterator */ queue_front(const struct TQueue *aQueue, TQueueElement *aValue)
	{
	return false;
	}

bool /* TQueueIterator */ queue_back(const struct TQueue *aQueue, TQueueElement *aValue)
	{
	return false;
	}

bool queue_push(struct TQueue *aQueue, TQueueElement aValue)
	{
	return false;
	}

bool queue_pop(struct TQueue *aQueue)
	{
	return false;
	}

void queue_destroy(struct TQueue *aQueue)
	{
	queue_init(aQueue);
	}

struct TQueueIterator queue_iterator_begin(const struct TQueue *aQueue)
	{
	return (struct TQueueIterator) { .iQueue = NULL, .iPos = 0 };
	}

bool queue_iterator_is_valid(const struct TQueueIterator *aIter)
	{
	return false;
	}

bool queue_iterator_to_next(struct TQueueIterator *aIter)
	{
	return false;
	}

TQueueElement queue_iterator_value(const struct TQueueIterator *aIter)
	{
	return (TQueueElement) { 0 };
	}

bool queue_iterator_set_value(const struct TQueueIterator *aIter, TQueueElement aValue)
	{
	return false;
	}
