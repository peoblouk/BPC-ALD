#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "TQueue.h"

static void print_element(const struct TQueueIterator* aIter); // Tisk hodnot iterátoru

int main(void)
	{
	struct TQueue queue1 = { 0 };
	queue_init(&queue1);
	
	while (true) // Načítání hodnot z konzole
	{
		TQueueElement value;
		if (scanf("%d", &value) != 1)
			return 1;

		if (value < 0) // Hodnota je záporná
			break;

		if (queue_push(&queue1, value) == false)
			return 1;
	}

	printf("queue1: ");
	struct TQueueIterator Iterator = queue_iterator_begin(&queue1); // Iterátor na tisk
	if (queue_iterator_is_valid(&Iterator) == false)
		return 1;
	
	queue_for_each(Iterator, print_element); // TIsk
	
	struct TQueue queue2 = { 0 };
	queue_init(&queue2); // Proměnná pro clonování


	return 1;
	}

static void print_element(const struct TQueueIterator* aIter) // Tisk elementů po 
	{
	printf("<%d> ", queue_iterator_value(aIter));
	}