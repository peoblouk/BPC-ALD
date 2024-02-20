#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "check.h"
#include "TStack.h"
#include <ctype.h>

/** \defgroup MainProgram 2. Hlavní program
 *  \brief Definice funkcí hlavního programu
 *  \{
 */

/** \brief Testování API Stack
 *  \details Testovací funkce pro API Stack. Funkce načítá z konzoly znaky, které ukládá po slovech do zásobníku. Slovo obsahuje pouze písmena a číslice. Po nalezení konce slova, následně znaky ze zásobníku vybírá, ruší a tiskne.
 */
void demo_tiskni_slova_pozpatku(void)
	{
	struct TStack stack = { 0 }; //zalozeni prazdneho zasobniku
	stack_init(&stack); //inicializace na nulu -> init funkce
	int znak = 0; //inicializace promenne
	do
		{
		znak = getchar(); //nactu z konzole znak
		while(isalnum(znak)) //kontrola zdali je znak alfanumericky tzn. pokud je to cislice nebo pismeno -> pokracuje; pokud neni -> opakuje se dokola
			{
			stack_push(&stack, znak); //nacteni znaku a vlozeni do zasobniku
			znak = getchar();
			}

		while(!stack_is_empty(&stack)) //dokud neni stack prazdny, opakuje se do nekonecna
			{
			int val = 0;
			stack_top(&stack, &val); //precteni hodnoty na vrcholu zasobniku
			stack_pop(&stack); //odebrani hodnoty z vrcholu zasobniku
			putchar(val); //vytisknuti hodnoty
			}
		putchar(znak); //vytisknuti znaku, ktery nacetl
		}
	while(znak != '\n'); //pokud je nactenym znakem ENTER, pokracuje dale v programu jinak opakuje loop

	stack_destroy(&stack);
	}

int main(void)
	{
	struct TStack stack1 = { 0 };
	stack_init(&stack1);
	printf("stack1 is empty: %s\n", stack_is_empty(&stack1) ? "true" : "false"); //pokud je zasobnik prazdny, vrati TRUE, jinak FALSE
	stack_push(&stack1, 0);
	printf("stack1 is empty: %s\n", stack_is_empty(&stack1) ? "true" : "false");

	stack_push(&stack1, 222);
	for(struct TStackIterator it = stack_iterator_begin(&stack1); stack_iterator_is_valid(&it); stack_iterator_to_next(&it))
		printf("%d ", stack_iterator_value(&it)); //trik s iteratorama - projde vsechny prvky iteratoru a vytiskne je
	putchar('\n');

	stack_push(&stack1, 42); //do zasobniku vlozim hodnotu 42
	stack_push(&stack1, 43); //do zasobniku vlozim hodnotu 43
	stack_push(&stack1, 40); //do zasobniku vlozim hodnotu 40

	for(struct TStackIterator it = stack_iterator_begin(&stack1); stack_iterator_is_valid(&it); stack_iterator_to_next(&it))
		printf("%d ", stack_iterator_value(&it)); //trik s iteratorama - projde vsechny prvky iteratoru a znovu je vytiskne
	putchar('\n');

	TStackElement val = 0; //vytvoreni a inicializace promenne value
	if(stack_top(&stack1, &val)) //zeptam se, co je na vrcholu zasobniku -> pokud bude TRUE -> vytiskne hodnotu
		printf("%d\n", val);
	stack_pop(&stack1); //nasledne odebrani hodnoty (pise 40)

	for(struct TStackIterator it = stack_iterator_begin(&stack1); stack_iterator_is_valid(&it); stack_iterator_to_next(&it))
		printf("%d ", stack_iterator_value(&it)); //trik s iteratorama - projde vsechny prvky iteratoru a znovu je vytiskne (pise 43 42 222 0)
	putchar('\n');

	stack_destroy(&stack1); //zniceni vektoru stack1

	printf("Zadej retezec: \n");
	demo_tiskni_slova_pozpatku(); //funkce

	return 0;
	}

/** \} MainProgram */
