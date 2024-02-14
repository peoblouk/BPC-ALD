/** \file TStack.c
 *  \brief Implementace API pro typ zásobník (realizace pomocí pole)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TStack.c 2592 2024-02-09 17:45:42Z petyovsky $
 */

#include "TStack.h"

void stack_init(struct TStack *aStack)
	{
	}

bool stack_is_empty(const struct TStack *aStack)
	{
	return true;
	}

bool /* TStackIterator */ stack_top(const struct TStack *aStack, TStackElement *aValue)
	{
	return false;
	}

bool stack_push(struct TStack *aStack, TStackElement aValue)
	{
	return false;
	}

bool stack_pop(struct TStack *aStack)
	{
	return false;
	}

void stack_destroy(struct TStack *aStack)
	{
	}

struct TStackIterator stack_iterator_begin(const struct TStack *aStack)
	{
	return (struct TStackIterator) { .iStack = NULL, .iPos = 0 };
	}

bool stack_iterator_is_valid(const struct TStackIterator *aIter)
	{
	return false;
	}

bool stack_iterator_to_next(struct TStackIterator *aIter)
	{
	return false;
	}

TStackElement stack_iterator_value(const struct TStackIterator *aIter)
	{
	return (TStackElement) { 0 };
	}
