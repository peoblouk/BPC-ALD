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
	return (struct TStackIterator) { .iStack = NULL, .iActual = NULL };
	}

bool stack_iterator_is_valid(const struct TStackIterator *aIter)
	{
	return false;
	}

bool stack_iterator_to_next(struct TStackIterator *aIter)
	{
	bool valid = stack_iterator_is_valid(aIter);
	return valid;
	}

TStackElement stack_iterator_value(const struct TStackIterator *aIter)
	{
	return (TStackElement) { 0 };
	}
