/** \file main.c
 *  \brief Hlavní program pro testování ADT TQueue
 *  \author Petyovský
 *  \version 2024
 *  $Id: main.c 2615 2024-02-16 15:36:07Z petyovsky $
 */

#include <stdio.h>
#include "TQueue.h"

/** \defgroup MainProgram 3. Hlavní program
 *  \brief Definice funkcí hlavního programu
 *  \{
 */

/** \brief Vytisknutí elementu fronty pomocí iterátoru
 *  \details Tisk hodnoty elementu fronty, na který ukazuje předaný iterátor.
 *  \param[in] aIter Ukazatel na existující iterátor
 */
void print_element(const struct TQueueIterator *aIter)
	{
	printf("%c ", queue_iterator_value(aIter));
	}

int main(void)
	{
	struct TQueue queue1 = { 0 };
	queue_init(&queue1);
	printf("queue1 is empty: %s\n", queue_is_empty(&queue1) ? "true" : "false");
	
	TQueueElement read = "";

	while ((read = getchar()) != '\n')
		{
		if (queue_push(&queue1, read) == false)
			return 1;
		}
	
	struct TQueueIterator iterator1 = queue_iterator_begin(&queue1);
	if (queue_iterator_is_valid(&iterator1) == false)
		return 1;
	queue_for_each(iterator1, print_element);
	putchar('\n');

	int num = 0;
	printf("pocet otocek: ");
	if (scanf("%d", &num) != 1)
		return 1;

	if (num < 0) // Rotace do leva <-
		{
			for(size_t i = 0; i < abs(num); i++)
			{
			TQueueElement current;
			queue_back(&queue1, &current);
			queue_pop_back(&queue1);	//chyba ve f-ci pop_back nebo push_front
			queue_push_front(&queue1, current);

			queue_for_each(queue_iterator_begin(&queue1), print_element); // Tisk elementů
			putchar('\n');
			}
		}
	else // Rotace do prava ->
		{
		for(size_t i = 0; i < abs(num); i++)
			{
			TQueueElement current;
			queue_front(&queue1, &current);
			queue_pop(&queue1);
			queue_push(&queue1, current);

			queue_for_each(queue_iterator_begin(&queue1), print_element); // Tisk elementů
			putchar('\n');
			}
		}

	return 0;
	}

/** \} MainProgram */
