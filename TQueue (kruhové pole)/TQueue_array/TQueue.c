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
	if (aQueue == NULL)
		return;

	aQueue->iFrontPos = aQueue->iBackPos = 0;
	}

bool queue_is_empty(const struct TQueue *aQueue)
	{
	if (aQueue == NULL)
		return true;

	assert(aQueue->iBackPos < QUEUE_MAXCOUNT); // Koncový ukazatel nemůže být větší jak MAXCOUNT
	assert(aQueue->iFrontPos < QUEUE_MAXCOUNT); // Začáteční ukazatel nemůže být větší jak MAXCOUNT

	return aQueue->iFrontPos == aQueue->iBackPos; // Oba jsou na stejném prvku tudiž fronta je prázdná
	}

bool /* TQueueIterator */ queue_front(const struct TQueue *aQueue, TQueueElement *aValue) // Začátek fronty
	{
	if (queue_is_empty(aQueue) == true)
		return false;

	if (aValue == NULL) // Výstupní proměnná musí mít adresu kam zapsat
		return false;

	*aValue = aQueue->iValues[aQueue->iFrontPos];
	return true;
	}

bool /* TQueueIterator */ queue_back(const struct TQueue *aQueue, TQueueElement *aValue) // Konec fronty
	{
	if (queue_is_empty(aQueue) == true)
		return false;

	if (aValue == NULL) // Výstupní proměnná musí mít adresu kam zapsat
		return false;

	if (aQueue->iBackPos == 0)
		*aValue = aQueue->iValues[QUEUE_MAXCOUNT - 1]; // Nezapomenout že iBackPos ukazuje na další volné místo ve frontě
	else
		*aValue = aQueue->iValues[aQueue->iBackPos - 1]; // Nezapomenout že iBackPos ukazuje na další volné místo ve frontě
	return true;
	}

bool queue_push(struct TQueue *aQueue, TQueueElement aValue)
	{
	if (aQueue == NULL)
		return false;

	size_t new_index = (aQueue->iBackPos + 1) % QUEUE_MAXCOUNT; // Výpočet pomocné proměnné

	if (new_index == aQueue->iFrontPos) // Fronta je plná
		return false;
	
	aQueue->iValues[aQueue->iBackPos] = aValue;
	aQueue->iBackPos = new_index;
	return true;
	}

bool queue_pop(struct TQueue *aQueue)
	{
	if (aQueue == NULL)
		return false;

	if (queue_is_empty(aQueue) == true)
		return false;

	aQueue->iFrontPos = (aQueue->iFrontPos + 1) % QUEUE_MAXCOUNT;
	return true;
	}

void queue_destroy(struct TQueue *aQueue)
	{
	if (queue_is_empty(aQueue) == true)
		return;

	queue_init(aQueue); // Funkce dá iBackPos a iFrontPos na nulu
	}

struct TQueueIterator queue_iterator_begin(const struct TQueue *aQueue)
	{
	if (queue_is_empty(aQueue) == true)
		return (struct TQueueIterator) { .iQueue = NULL, .iPos = 0 };

	return (struct TQueueIterator) { .iQueue = aQueue, .iPos = aQueue->iFrontPos };
	}

bool queue_iterator_is_valid(const struct TQueueIterator *aIter)
	{
	if (aIter != NULL)
		if (queue_is_empty(aIter->iQueue) != true)
			if (aIter->iPos != aIter->iQueue->iBackPos) // Ukazuje
				return true;

	return false;
	}

bool queue_iterator_to_next(struct TQueueIterator* aIter)
	{
	if (aIter == NULL)
		return false;

	if (queue_iterator_is_valid(aIter) == false) // Pokud je nevalidní
		{
		aIter->iQueue = NULL; // Zruš navázanou frontu
		aIter->iPos = 0;
		return false;
		}
	aIter->iPos = (aIter->iPos + 1) % QUEUE_MAXCOUNT; // Posun pomocí modulo 

	if (aIter->iPos == aIter->iQueue->iBackPos) // Nedošli jsme
		{
		return false;
		aIter->iQueue = NULL; // Zneplatnění iterátoru
		aIter->iPos = 0;
		}
	return true;
	}

TQueueElement queue_iterator_value(const struct TQueueIterator *aIter)
	{
	if (aIter == NULL)
		return (TQueueElement) { 0 };
	if (queue_iterator_is_valid(aIter) == false)
		return (TQueueElement) { 0 };

	return (TQueueElement) { aIter->iQueue->iValues[aIter->iPos] };
	}

bool queue_iterator_set_value(const struct TQueueIterator* aIter, TQueueElement aValue)
	{
	if (queue_iterator_is_valid(aIter) == false)
		return false;
	
	/* Přetypování proměnné */
	((struct TQueue*)aIter->iQueue)->iValues[aIter->iPos] = aValue; // V definici iterátoru je to const struct, musím si jí přetypovat na ukazatele na nomální struct
	// aIter->iQueue->iValues[aIter->iPos] = aValue; // TOTO NELZE NAPSAT JE TO const struct !! 
	return true;
	}
