/** \file TStack.c
 *  \brief Implementace API pro typ zásobník (realizace pomocí pole)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TStack.c 2592 2024-02-09 17:45:42Z petyovsky $
 */

#include "TStack.h"

void stack_init(struct TStack *aStack)
	{
	if (aStack) // if(aStack != NULL) // Pokud je místo alokované, tak nastavím iCount na 0, tím inicializuji
		aStack->iCount = 0;
	}

bool stack_is_empty(const struct TStack *aStack)
	{
	if (aStack == NULL)
		return true;
	
	return aStack->iCount == 0; // vrátí true nebo false
	}

bool /* TStackIterator */ stack_top(const struct TStack *aStack, TStackElement *aValue)
	{
	if (stack_is_empty(aStack))
		return false;
	if (aValue == NULL) // pokud nová hodnota bude nula tak nemám co zapsat
		return false;
	
	*aValue = aStack->iValues[aStack->iCount - 1]; // vloží hodnotu z aValue
	
	return true;
	}

bool stack_push(struct TStack *aStack, TStackElement aValue)
	{
	if(!aStack)
		return false;
	
	if (aStack->iCount >= STACK_MAXCOUNT) // Kontrola abychom nepřekročili velikost pole
		return false;

	aStack->iValues[aStack->iCount++] = aValue; // Přidání hodnoty
	}

bool stack_pop(struct TStack *aStack)
	{
	if (stack_is_empty(aStack))
		return false;
	--aStack->iCount;

	return true;
	}

void stack_destroy(struct TStack *aStack)
	{
	stack_init(aStack);
	}

struct TStackIterator stack_iterator_begin(const struct TStack *aStack)
	{
	if(stack_is_empty(aStack))
		return (struct TStackIterator) { .iStack = NULL, .iPos = 0 };

	return (struct TStackIterator) { .iStack = aStack, 
									 .iPos = aStack->iCount - 1};
	}

bool stack_iterator_is_valid(const struct TStackIterator *aIter)
	{
	if (aIter) // zda iterátor odkazuje na platná data zásobníku
		if(aIter->iStack) // pokud je iterátor asociován s platným zásobníkem
			if(aIter->iPos < aIter->iStack->iCount) // pokud iterátor odkazuje do oblasti platných dat zásobníku
				return true;
	
	return false;
	}

bool stack_iterator_to_next(struct TStackIterator *aIter)
	{
	if (aIter == NULL && aIter->iStack == NULL)
		return false;

	if ((aIter > 0) && (aIter->iPos < aIter->iStack->iCount)) // Pokud ukazuje mimo pole je neplatný
		{
		--aIter->iPos;
		return true;
	}

	*aIter = (struct TStackIterator) { .iStack = NULL, .iPos = 0 }; // Místo kde ukazuje aIter tak vynuluji iStack a iPos
	return false;
	}

TStackElement stack_iterator_value(const struct TStackIterator *aIter)
	{
	if (stack_iterator_is_valid(aIter))
		return aIter->iStack->iValues[aIter->iPos];
	
	return (TStackElement) { 0 };
	}
