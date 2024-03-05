/** \file TStack.c
 *  \brief Implementace API pro typ zásobník (realizace pomocí pole)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TStack.c 2592 2024-02-09 17:45:42Z petyovsky $
 */

#include "TStack.h"

void stack_init(struct TStack* aStack)
	{
	if (aStack) // if(aStack != NULL) // Pokud je místo alokované, tak nastavím iCount na 0, tím inicializuji
		{
		aStack->iCount = 0;
		aStack->iValues = NULL;
		aStack->iCapacity = 0;
		}
	}

bool stack_is_empty(const struct TStack* aStack)
	{
	if (aStack == NULL)
		return true;

	return aStack->iCount == 0; // vrátí true nebo false
	}

bool /* TStackIterator */ stack_top(const struct TStack* aStack, TStackElement* aValue)
	{
	if (stack_is_empty(aStack))
		return false;
	if (aValue == NULL) // pokud nová hodnota bude nula tak nemám co zapsat
		return false;

	*aValue = aStack->iValues[aStack->iCount - 1]; // vloží hodnotu z aValue
	return true;
	}

bool stack_push(struct TStack* aStack, TStackElement aValue)
	{
	if (aStack == NULL) // Kontrola vstupních 
		return false;

	if (aStack->iCapacity == 0) // Není žádná kapacita v poli
		{
		aStack->iValues = (TStackElement*)malloc(sizeof(TStackElement)); // Pokusím se alokovat
		if (aStack->iValues == NULL) // Chyba nedošlo k alokaci
			{
			printf("\nNepodaradilo alokovat\n");
			return false;
			}
		aStack->iCapacity += 1;
		aStack->iValues[aStack->iCount] = aValue;
		++aStack->iCount;
		return true;
		}

	if (aStack->iCount < aStack->iCapacity) // Není třeba alokovat dostatek místa
		{
		aStack->iValues[aStack->iCount] = aValue;
		++aStack->iCount;
		return true;
		}

	if (aStack->iCount >= aStack->iCapacity) // Capacita je stejně velká jako iSize je třeba naalokovat 2* víc místa (amortizace)
		{
		TStackElement* new_values = (TStackElement*) calloc(2 * aStack->iCapacity, sizeof(TStackElement)); // využít realloc 
		// realloc(new_values, sizeof(TStackElement));
		if (new_values == NULL) // Chyba po alokaci
			{
			printf("\nNepodaradilo alokovat\n");
			return false;
			}

		for (size_t i = 0; i < (aStack->iCount - 1); ++i)
			new_values[i] = aStack->iValues[i];			
		
		free(aStack->iValues);

		printf("\nRealokace!\n");
		aStack->iValues = new_values; // Předám nové hodnoty adresy, které byli alokovány
		aStack->iValues[aStack->iCount] = aValue; // Přidám hodnoty na adresy pole
		aStack->iCount++;
		aStack->iCapacity *= 2; // Dvakrat zvětšená kapacita
		return true;
		}
	return false;
	}


bool stack_pop(struct TStack *aStack)
	{
	if (stack_is_empty(aStack))
		return false;

	if (--aStack->iCount == 0) // Po odebrání nebude zásobník obsahovat nic, tak proč držet alokované pole
		{
		stack_destroy(aStack);
		return true;
		}

	aStack->iCount -= 1; // Odeberu jeden prvek
	if (aStack->iCount > aStack->iCapacity / 4) // Obsahuje > 75% prvků (aStack->iCapacity / 4) = 25 %
		return true;

	else // Pop 
		{
		TStackElement* new_values = (TStackElement*)calloc(aStack->iCapacity / 2, sizeof(TStackElement));
		if (new_values == NULL) // Chyba realokace
			{
			stack_destroy(aStack);
			return false;
			}

		aStack->iCapacity /= 2; // Zmenši kapacitu na polovinu
		for (size_t i = 0; i < aStack->iCount; i++)
			new_values[i] = aStack->iValues[i];
		
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
