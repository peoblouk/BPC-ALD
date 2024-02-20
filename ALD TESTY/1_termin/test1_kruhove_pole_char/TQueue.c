/** \file TQueue.c
 *  \brief Implementace API pro typ fronta (realizace pomocí kruhového pole)
 *  \author Petyovský
 *  \version 2023
 *  $Id: TQueue.c 2190 2023-02-21 14:49:16Z petyovsky $
 */

#include <assert.h>
#include "TQueue.h"

void queue_init(struct TQueue *aQueue)
	{
	if (aQueue)
		aQueue->iFrontPos = aQueue->iBackPos = 0;
	}

bool queue_push_front(struct TQueue *aQueue, TQueueElement aValue){
	if(!aQueue)
		return false;
	if(aQueue->iFrontPos == 0){
		if(QUEUE_MAXCOUNT - 1 == aQueue->iBackPos)
			return false;
		aQueue->iValues[QUEUE_MAXCOUNT - 1] = aValue;
		aQueue->iFrontPos = QUEUE_MAXCOUNT - 1;
		return true;
	}
	
	if(aQueue->iFrontPos - 1 == aQueue->iBackPos){
		return false;
	}
	else{
		aQueue->iFrontPos = aQueue->iFrontPos - 1;
		aQueue->iValues[aQueue->iFrontPos] = aValue;
	}
	
	return true;
}

bool queue_pop_back(struct TQueue *aQueue){
	if(!aQueue){
		return false;
	}
	
	if(aQueue->iBackPos == 0){
		if(aQueue->iFrontPos == QUEUE_MAXCOUNT - 1)
			return false;
		aQueue->iBackPos = QUEUE_MAXCOUNT - 1;
		return true;
	}
	
	if(aQueue->iFrontPos == aQueue->iBackPos - 1)
		return false;
	else
		aQueue->iBackPos -= 1;
	
	return true;
}

bool queue_is_empty(const struct TQueue *aQueue)
	{
	if (aQueue)
		return aQueue->iFrontPos == aQueue->iBackPos;

	return true;
	}

bool /* TQueueIterator */ queue_front(const struct TQueue *aQueue, TQueueElement *aValue)
	{
	if(queue_is_empty(aQueue) || !aValue) //abychom vraceli prvek z čela fronty, fronta nesmí být prázdná
		return false;

	*aValue = aQueue->iValues[aQueue->iFrontPos];
	return true;
	}

bool /* TQueueIterator */ queue_back(const struct TQueue* aQueue, TQueueElement* aValue)
	{
	if (queue_is_empty(aQueue) || !aValue)
		return false;

	size_t backpos = (aQueue->iBackPos) ? (aQueue->iBackPos - 1) : (QUEUE_MAXCOUNT - 1);	//kdyz je iBackPos nenulovy -> vysledkem bude prvni zavorka
	
	*aValue = aQueue->iValues[backpos];
	return true;																			//, jinak vysledkem bude druha
	}																						//"?" = ternarni operand								

bool queue_push(struct TQueue *aQueue, TQueueElement aValue)
	{
	if (!aQueue)
		return false;

	size_t nextpos = (aQueue->iBackPos + 1) % QUEUE_MAXCOUNT;

	if(nextpos == aQueue->iFrontPos)
		return false;
	
	aQueue->iValues[aQueue->iBackPos] = aValue ;												//pushpost ukazuje na misto, kam v budoucnu vlozim prvek								
	aQueue->iBackPos = nextpos;
	return true;
	}

bool queue_pop(struct TQueue *aQueue)
	{
	if (queue_is_empty(aQueue))
		return false;
	
	aQueue->iFrontPos = (aQueue->iFrontPos + 1) % QUEUE_MAXCOUNT;
	
	//bz = bz % 5;
	//bz %= 5;
	
	return true;
	}

void queue_destroy(struct TQueue *aQueue)
	{
	queue_init(aQueue);
	}

struct TQueueIterator queue_iterator_begin(const struct TQueue *aQueue)
	{
	if(aQueue)
		return (struct TQueueIterator) {.iQueue = aQueue, .iPos = aQueue->iFrontPos };			//kdyz fronta neni prazdna

	return (struct TQueueIterator) { .iQueue = NULL, .iPos = 0 };								//kdyz je fronta prazdna
	}

bool queue_iterator_is_valid(const struct TQueueIterator *aIter)
	{
	if (aIter)																					//aIter nesmi byt null, aby byl validni
		if (!queue_is_empty(aIter->iQueue))
			return aIter->iPos != aIter->iQueue->iBackPos;										//pokud se dostane iPos na stejny prvek jako iBackPos, tak nadale nema kam ukazovat

	return false;
	}

bool queue_iterator_to_next(struct TQueueIterator *aIter)
	{
	if (aIter && aIter->iQueue)
		{
		if (aIter->iPos != aIter->iQueue->iBackPos)
			{
			aIter->iPos = (aIter->iPos + 1) % QUEUE_MAXCOUNT;
			if (aIter->iPos != aIter->iQueue->iBackPos)
				return true;
			}
		*aIter = (struct TQueueIterator){ .iQueue = NULL, .iPos = 0 };
		}
	return false;
	}

TQueueElement queue_iterator_value(const struct TQueueIterator *aIter)
	{
	if (!queue_iterator_is_valid(aIter))
		return (TQueueElement) { 0 };

	return aIter->iQueue->iValues[aIter->iPos];
	}

bool queue_iterator_set_value(const struct TQueueIterator *aIter, TQueueElement aValue)
	{
	if (!queue_iterator_is_valid(aIter))
		return false;
	
	((struct TQueue*)aIter->iQueue)->iValues[aIter->iPos] = aValue; //explicitni konverze = zde menim iQueue (pretypovani ukazatele, ze nebude ukazovat na konstantni strukturu,
	//int a;														//, protoze konst. strukturu nemuzeme zmenit, zatim co obycejnou ano, coz my chceme)
	//float b = ((float)a); kdyz chci zmenit z int na float
	
	return true;
	}
