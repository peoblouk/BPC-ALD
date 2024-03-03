/** \file main.c
 *  \brief Hlavní program pro testování ADT TStack
 *  \author Petyovský
 *  \version 2024
 *  $Id: main.c 2592 2024-02-09 17:45:42Z petyovsky $
 */

#include <stdio.h>
#include <ctype.h>
#include "TStack.h"

/** \defgroup MainProgram 2. Hlavní program
 *  \brief Definice funkcí hlavního programu
 *  \{
 */

/** \brief Testování API Stack
 *  \details Testovací funkce pro API Stack. Funkce načítá z konzoly znaky, které ukládá po slovech do zásobníku. Slovo obsahuje pouze písmena a číslice. Po nalezení konce slova, následně znaky ze zásobníku vybírá, ruší a tiskne.
 */
void demo_tiskni_slova_pozpatku(void)
	{
	struct TStack stack = { 0 };
	stack_init(&stack);
	int znak = 0;
	do
		{
		znak = getchar();
		while(isalnum(znak))
			{
			stack_push(&stack, znak);
			znak = getchar();
			}

		while(!stack_is_empty(&stack))
			{
			int val = 0;
			stack_top(&stack, &val);
			stack_pop(&stack);
			putchar(val);
			}
		putchar(znak);
		}
	while(znak != '\n');

	stack_destroy(&stack);
	}

int main()
	{
	struct TStack stack1 = { 0 };
	stack_init(&stack1);
	printf("stack1 is empty: %s\n", stack_is_empty(&stack1) ? "true" : "false");
	stack_push(&stack1, 0);
	printf("stack1 is empty: %s\n", stack_is_empty(&stack1) ? "true" : "false");

	stack_push(&stack1, 222);
	for(struct TStackIterator it = stack_iterator_begin(&stack1); stack_iterator_is_valid(&it); stack_iterator_to_next(&it))
		printf("%d ", stack_iterator_value(&it));
	putchar('\n');

	stack_push(&stack1, 42);
	stack_push(&stack1, 43);
	stack_push(&stack1, 40);

	for(struct TStackIterator it = stack_iterator_begin(&stack1); stack_iterator_is_valid(&it); stack_iterator_to_next(&it))
		printf("%d ", stack_iterator_value(&it));
	putchar('\n');

	TStackElement val = 0;
	if(stack_top(&stack1, &val))
		printf("%d\n", val);
	stack_pop(&stack1);

	for(struct TStackIterator it = stack_iterator_begin(&stack1); stack_iterator_is_valid(&it); stack_iterator_to_next(&it))
		printf("%d ", stack_iterator_value(&it));
	putchar('\n');

	stack_destroy(&stack1);

	printf("Zadej retezec: \n");
	demo_tiskni_slova_pozpatku();

	return 0;
	}

/** \} MainProgram */
