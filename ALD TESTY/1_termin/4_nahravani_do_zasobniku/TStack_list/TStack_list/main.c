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

void print_element(const struct TStackIterator* aIter)
	{
	printf("<%d> ", stack_iterator_value(aIter));
	}

int main(void)
	{
	struct TStack stack1 = { 0 };
	stack_init(&stack1);
	printf("stack1 is empty: %s\n", stack_is_empty(&stack1) ? "true" : "false");

	FILE* fr = fopen("stack_data.txt", "r");
	if (fr == NULL) // Kontrola otevření
		return 1;

	if (stack_init_file(&stack1, fr) == false)
		{
		fclose(fr);
		stack_destroy(&stack1);
		return 2;
		}
	fclose(fr);
	putchar('\n');

	stack_for_each(stack_iterator_begin(&stack1), print_element); // Print všech elementů
	TStackElement val;

	if (stack_is_empty(&stack1) == false)
		if (stack_top(&stack1, &val) == false)
			{
			stack_destroy(&stack1);
			return 3;
			}
	printf("%d", val);

	if (stack_pop(&stack1) == false)
		return 5;

	
 	TStackElement val2;
	while (stack_is_empty(&stack1) != true)
		{			
		if (stack_top(&stack1, &val2) == false)
			{
			stack_destroy(&stack1);
			return 6;
			}
		if (val2 < val)
			{
			putchar('\n');
			break;
			}

		if (stack_pop(&stack1) == false)
			
			return false;

		printf("%d", val2);
		val = val2;
		}

	stack_destroy(&stack1);
	return 0;
	}

/** \} MainProgram */
