/** \file TStack.c
 *  \brief Implementace API pro typ zásobník (realizace pomocí lineárního jednosměrně vázaného seznamu)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TStack.c 2592 2024-02-09 17:45:42Z petyovsky $
 */

#include "TStack.h"

/** \brief Úplná definice privátního typu StackNode
 *  \details Privátní typ StackNode (nedostupný mimo funkce ze souboru TStack.c) reprezentuje typ pro jednotlivé uzly lineárního jednosměrně vázaného seznamu.
 */
struct TStackNode
	{
	struct TStackNode *iNext;				///< Ukazatel na následující uzel lineárního jednosměrně vázaného seznamu
	TStackElement iValue;					///< Element zásobníku uložený v uzlu lineárního jednosměrně vázaného seznamu
	};

void stack_init(struct TStack *aStack)
	{
	if (aStack)
		aStack->iTop == NULL;
	}
// Konec zásobníku poznám podle NULL
// 

bool stack_is_empty(const struct TStack *aStack)
	{
	if (aStack == NULL)
		return true;

	return aStack->iTop == NULL;
	}

bool /* TStackIterator */ stack_top(const struct TStack *aStack, TStackElement *aValue)
	{
	if (stack_is_empty(aStack))
		return false;
	if (aValue == NULL)
		return false;
	
	*aValue = aStack->iTop->iValue;
	return true;
	}

bool stack_push(struct TStack *aStack, TStackElement aValue)
	{
	if(aStack == NULL) // Kontrola zda aStack není NULL
		return false;
	
	struct TStackNode* newnode = malloc(sizeof(struct TStackNode)); // Pole pro novou hodnotu
	if (newnode == NULL) // Chyba alokace
		return false;
	newnode->iValue = aValue;
	newnode->iNext = aStack->iTop;
	
	aStack->iTop = newnode; // Dám nový prvek na vrch listu


	return true;
	}

bool stack_pop(struct TStack *aStack)
	{
	if (stack_is_empty(aStack))
		return false;

	struct TStackNode* oldnode = aStack->iTop;
	aStack->iTop = oldnode->iNext; // Změním adresu top na adresu z iNext
	free(oldnode);

	return true;
	}

void stack_destroy(struct TStack *aStack)
	{
	if (aStack == NULL)
		return;

	struct TStackNode* actnode = aStack->iTop; // Pomocná proměnná co ukazuje na vrchol zásobníku 
	aStack->iTop = NULL;

	while (actnode != NULL)
		{
		struct TStackNode* oldnode = actnode; // Pomocná proměnná
		actnode = actnode->iNext;
		free(oldnode);
		}
	}

bool stack_init_file(struct TSTack* aStack, FILE* InputFile)
	{
	if (aStack == NULL || InputFile == NULL)
		return false;

	stack_init(aStack);


	}

struct TStackIterator stack_iterator_begin(const struct TStack *aStack)
	{
	if (aStack == NULL) // Je aStack NULL?
		return (struct TStackIterator) { .iStack = NULL, .iActual = NULL }; // Vytvoř prázdný iterátor
	
	return (struct TStackIterator) { .iStack = aStack, .iActual = aStack->iTop }; // Vytvoř prázdný iterátor
	}

bool stack_iterator_is_valid(const struct TStackIterator *aIter)
	{
	if (aIter)
		if (aIter->iStack) // Jestli není iStack prázdný
			if (aIter->iStack->iTop) // Stack není prázdný
				if (aIter->iActual) // Jestli není ukazovátko prázdné
					return true;

	return false;
	}

bool stack_iterator_to_next(struct TStackIterator *aIter)
	{
	// if (aIter->iActual == NULL) // Neexistuje další prvek (tuto podmínku není nutno prý ověřovat
		//return false;
	
	aIter->iActual = aIter->iActual->iNext; // Posuneme o jeden

	return aIter->iActual != NULL;
	}

TStackElement stack_iterator_value(const struct TStackIterator *aIter)
	{
	if(stack_iterator_is_valid(aIter) == false)
		return (TStackElement) { 0 };

	return aIter->iActual->iValue;
	}
