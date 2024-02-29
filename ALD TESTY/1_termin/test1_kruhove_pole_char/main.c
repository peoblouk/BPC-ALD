#include <stdio.h>
#include <stdlib.h>
#include "TQueue.h"

int main(void)
{
	struct TQueue queue;
	queue_init(&queue);

	char znak;
	while ((znak = getchar()) != '\n')
	{
		queue_push(&queue, znak);
	}

	for (struct TQueueIterator it = queue_iterator_begin(&queue); queue_iterator_is_valid(&it); queue_iterator_to_next(&it))
		printf("%c", queue_iterator_value(&it));
	putchar('\n');

	int num;

	do
	{
		printf("Pocet otacek: ");
		scanf("%d", &num);
	} while (num == 0);

	putchar('\n');

	for (struct TQueueIterator it = queue_iterator_begin(&queue); queue_iterator_is_valid(&it); queue_iterator_to_next(&it))
	{
		printf("%c", queue_iterator_value(&it));
	}
	putchar('\n');

	if (num > 0)
	{
		while (num != 0)
		{
			TQueueElement current;
			queue_front(&queue, &current);
			queue_pop(&queue);
			queue_push(&queue, current);

			for (struct TQueueIterator it = queue_iterator_begin(&queue); queue_iterator_is_valid(&it); queue_iterator_to_next(&it))
			{
				printf("%c", queue_iterator_value(&it));
			}
			putchar('\n');
			num -= 1;
		}
	}
	else
	{
		while (num != 0)
		{
			TQueueElement current;
			queue_back(&queue, &current);
			queue_pop_back(&queue);
			queue_push_front(&queue, current);

			for (struct TQueueIterator it = queue_iterator_begin(&queue); queue_iterator_is_valid(&it); queue_iterator_to_next(&it))
			{
				printf("%c", queue_iterator_value(&it));
			}
			putchar('\n');
			num += 1;
		}
	}

	return 0;
}
