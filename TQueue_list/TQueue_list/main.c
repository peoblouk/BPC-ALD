/** \file main.c
 *  \brief Hlavní program pro testování ADT TQueue
 *  \author Petyovský
 *  \version 2024
 *  $Id: main.c 2616 2024-02-16 15:36:28Z petyovsky $
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
	printf("<%d> ", queue_iterator_value(aIter));
	}

/** \brief Porovnání hodnoty elementu fronty pomocí iterátoru s hodnotou 43
 *  \details Příklad predikátové funkce. Porovnání hodnoty elementu fronty, na který ukazuje předaný iterátor s hodnotou 43.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je hodnota elementu fronty na který ukazuje iterátor rovna hodnotě 43
 */
bool is_element_with_value_43(const struct TQueueIterator *aIter)
	{
	return queue_iterator_value(aIter) == 43;
	}

int main()
	{
	struct TQueue queue1 = { 0 };
	queue_init(&queue1);
	printf("queue1 is empty: %s\n", queue_is_empty(&queue1) ? "true" : "false");
	queue_push(&queue1, 0);
	printf("queue1 is empty: %s\n", queue_is_empty(&queue1) ? "true" : "false");

	queue_push(&queue1, 222);
	for(struct TQueueIterator it = queue_iterator_begin(&queue1); queue_iterator_is_valid(&it); queue_iterator_to_next(&it))
		printf("%d ", queue_iterator_value(&it));
	putchar('\n');

	queue_push(&queue1, 42);
	queue_push(&queue1, 43);
	queue_push(&queue1, 40);

	for(struct TQueueIterator it = queue_iterator_begin(&queue1); queue_iterator_is_valid(&it); queue_iterator_to_next(&it))
		print_element(&it);
	putchar('\n');

	queue_for_each(queue_iterator_begin(&queue1), print_element);
	putchar('\n');

	struct TQueueIterator it = queue_find_if(queue_iterator_begin(&queue1), is_element_with_value_43);
	queue_iterator_set_value(&it, 128);
	putchar('\t');
	queue_for_each(it, print_element);
	putchar('\n');

	it = queue_find_if_not(queue_iterator_begin(&queue1), is_element_with_value_43);
	queue_iterator_set_value(&it, 64);
	putchar('\t');
	queue_for_each(it, print_element);
	putchar('\n');

	queue_for_each(queue_iterator_begin(&queue1), print_element);
	putchar('\n');

	TQueueElement val = 0;
	if(queue_front(&queue1, &val))
		printf("queue1 front: %d\n", val);
	if(queue_back(&queue1, &val))
		printf("queue1 back: %d\n", val);

	while(!queue_is_empty(&queue1))
		queue_pop(&queue1);

	queue_push(&queue1, 99);
	queue_for_each(queue_iterator_begin(&queue1), print_element);
	putchar('\n');

	queue_destroy(&queue1);
	return 0;
	}

/** \} MainProgram */
