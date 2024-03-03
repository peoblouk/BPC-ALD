/** \file TStack.c
 *  \brief Implementace API pro typ zásobník (realizace pomocí pole)
 *  \author Petyovský
 *  \version 2023
 *  $Id: TStack.c 2126 2023-02-10 15:54:58Z petyovsky $
 */

#include "TStack.h"

void stack_init(struct TStack *aStack)
{
	if (aStack)
	{ // pokud je dana rozumna hodnota aStacku, mam co delat
		aStack->iCount = 0;
		aStack->iCapacity = 0;
		aStack->iValues = NULL;
	}
}

bool stack_is_empty(const struct TStack *aStack)
{
	if (aStack)
		// if (aStack->iCount == 0)
		//	return true;
		// else
		//	return false;
		return aStack->iCount == 0; // pokud je pocet prvku aStacku nula tzn. zasobnik(stack) je prazdny -> vracim TRUE, jinak false

	return true; // pokud je stack neplatny, vracim TRUE
}

bool /* TStackIterator */ stack_top(const struct TStack *aStack, TStackElement *aValue)
{
	if (stack_is_empty(aStack) || !aValue) // pokud je zasobnik prazdny, nemam co vracet a to stejne plati pro aValue, kdyz se rovna NULL
		return false;

	*aValue = aStack->iValues[aStack->iCount - 1]; // do aValue zapisuje hodnotu(propise se *), ktera je prvni v poli iValues (tzn. pocet prvku - 1 -> jedna se o index pole)
	return true;
}

bool stack_push(struct TStack *aStack, TStackElement aValue)
{
	if (aStack && aStack->iCapacity == 0)
	{
		if ((aStack->iValues = (TStackElement *)malloc(sizeof(TStackElement))) == NULL)
		{
			printf("Není volná paměť.\n");
			return false;
		}
		aStack->iCapacity += 1;
		aStack->iValues[aStack->iCount] = aValue;
		++aStack->iCount;
		return true;
	}
	if (aStack && aStack->iCount < aStack->iCapacity)
	{
		aStack->iValues[aStack->iCount] = aValue;
		++aStack->iCount;
		return true;
	}
	if (aStack && aStack->iCount == aStack->iCapacity)
	{
		TStackElement *new_values = (TStackElement *)calloc(2 * aStack->iCapacity, sizeof(TStackElement));
		if (!new_values)
		{
			printf("Není volná paměť.\n");
			return false;
		}

		for (size_t i = 0; i < aStack->iCount; ++i)
		{
			new_values[i] = aStack->iValues[i];
		}
		free(aStack->iValues);
		aStack->iValues = new_values;

		aStack->iCapacity *= 2;
		aStack->iValues[aStack->iCount] = aValue;
		++aStack->iCount;
		return true;
	}

	return false;
}

bool stack_pop(struct TStack *aStack) // odebirani prvku ze zasobniku
{
	if (stack_is_empty(aStack))
		return false;

	--aStack->iCount;
	if (aStack->iCount == 0)
	{
		stack_destroy(aStack);
		return true;
	}

	if (aStack->iCount > aStack->iCapacity / 4)
	{
		return true;
	}

	else
	{
		TStackElement *new_values = (TStackElement *)calloc(aStack->iCapacity / 2, sizeof(TStackElement));
		if (!new_values)
		{
			printf("Není volná paměť.\n");
			return false;
		}

		aStack->iCapacity /= 2;
		for (size_t i = 0; i < aStack->iCount; ++i)
		{
			new_values[i] = aStack->iValues[i];
		}

		free(aStack->iValues);
		aStack->iValues = new_values;
		aStack->iCapacity /= 2;
	}

	return true;
}

void stack_destroy(struct TStack *aStack)
{
	free(aStack->iValues);
	stack_init(aStack);
}

struct TStackIterator stack_iterator_begin(const struct TStack *aStack) // konstruktor, do ktereho se vlozi zasobnik
{																		// a on my vrati iterator, ktery bude s timto stackem navzdy spojeny
	if (!stack_is_empty(aStack))
		return (struct TStackIterator){.iStack = aStack, .iPos = aStack->iCount - 1}; // sjednotim iterator se zasobnikem

	return (struct TStackIterator){.iStack = NULL, .iPos = 0};
}

bool stack_iterator_is_valid(const struct TStackIterator *aIter) // jestli zasobnik, na ktery iterator ukazuje je jeste platny
{																 // a nebo zdali je pozice na kterou ukazuje je jeste platna
	if (aIter)
		if (aIter->iStack && aIter->iPos < aIter->iStack->iCount) // kdyz iterator je platny a zaroven iterator ukazuje na platny zasobnik
			return true;										  // a zaroven iteratorska pozice je mensi nez pozice zasobniku s kterym je ten iterator spojeny s poctem prvku

	return false;
}

bool stack_iterator_to_next(struct TStackIterator *aIter) // funkce pro posun iteratoru
{
	if (stack_iterator_is_valid(aIter))
		if (aIter->iPos > 0)
		{
			--aIter->iPos;
			return true;
		}

	*aIter = (struct TStackIterator){.iStack = NULL, .iPos = 0};
	return false;
}

TStackElement stack_iterator_value(const struct TStackIterator *aIter) // precteni hodnoty elementu z toho zasobniku
{																	   //, ktery je v asociaci s danym iteratorem na urcitou hodnotu
	if (stack_iterator_is_valid(aIter))
		return aIter->iStack->iValues[aIter->iPos];
	return (TStackElement){0};
}
