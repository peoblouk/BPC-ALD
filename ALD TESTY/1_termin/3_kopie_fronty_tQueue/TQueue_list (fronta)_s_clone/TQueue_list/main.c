#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "TQueue.h"

static void print_element(const struct TQueueIterator* aIter); // Tisk hodnot iterátoru

int main(void)
	{
	struct TQueue queue1 = { 0 };
	queue_init(&queue1);
	// printf("Zadej cisla:\n");

	while (true) // Načítání hodnot z konzole
	{
		TQueueElement value;
		if (scanf("%d", &value) != 1)
			queue_destroy(&queue1);
			return 1;

		if (value < 0) // Hodnota je záporná
			break;

		if (queue_push(&queue1, value) == false)
			queue_destroy(&queue1);
			return 1;
	}

	printf("queue1: ");
	struct TQueueIterator Iterator = queue_iterator_begin(&queue1); // Iterátor pro tisk
	if (queue_iterator_is_valid(&Iterator) == false)
		return 1;
	queue_for_each(Iterator, print_element); // Tisk jednotlivých proměnných
	putchar('\n');

	struct TQueue queue2 = { 0 };
	queue_init(&queue2); // Proměnná pro clonování
	
	if (queue_clone(&queue1, &queue2) == false)
		return 2;
	
	printf("queue2: ");
	queue_for_each(queue_iterator_begin(&queue2), print_element); // Iterátor pro tisk

	queue_destroy(&queue1);
	printf("queue1 is empty: %s\n", queue_is_empty(&queue1) ? "true" : "false");

	queue_destroy(&queue2);
	return 0;
	}

static void print_element(const struct TQueueIterator* aIter) // Tisk elementů po 
	{
	printf("<%d> ", queue_iterator_value(aIter));
	}