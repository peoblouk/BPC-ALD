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
	printf("<%c> ", queue_iterator_value(aIter));
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

	TQueueElement read = "";

	while ((read = getchar()) != '\n')
		{
		if (queue_push(&queue1, read) == false)
			return 1;
		}

	queue_for_each(queue_iterator_begin(&queue1), print_element);

	return 0;
}

/** \} MainProgram */
