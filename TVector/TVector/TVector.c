/** \file TVector.c
 *  \brief Implementace Basic Vector API
 *  \author Petyovský
 *  \version 2024
 *  $Id: TVector.c 2584 2024-02-05 14:33:25Z petyovsky $
 */

#include <stdlib.h>
#include <assert.h>
#include "TVector.h"
#include "check.h"

bool vector_init(struct TVector* aVector, size_t aSize)
	{
	if (aVector == NULL)
		return false;

	TVectorElement* new_values = NULL;
	
	if (aSize != 0) // Kontrola zda je aSize nějaké číslo
		{
		new_values = calloc(aSize, sizeof(TVectorElement));

		if (new_values == NULL) // Kontrola zda došlo k kolokaci
			return false;
		}

	*aVector = (struct TVector){ .iValues = new_values, .iSize = aSize };
	// zjednodušený zápis 
	// aVector->iValues = new_values;
	// aVector->iSize = aSize;
	return true;
	}

bool vector_init_random(struct TVector* aVector, size_t aSize)
	{
	if (aVector == NULL)
		return false;

	TVectorElement* new_values = NULL;

	if (aSize != 0) // Kontrola zda je aSize nějaké číslo
		{
		new_values = malloc(aSize * sizeof(TVectorElement));

		if (new_values == NULL) // Kontrola zda došlo k kolokaci
			return false;

		for (size_t i = 0; i < aSize; i++)
			new_values[i] = vector_element_random_value();
		}

	*aVector = (struct TVector){ .iValues = new_values, .iSize = aSize };
	return true;
	}

bool vector_init_file(struct TVector *aVector, FILE *aInputFile)
	{
	if (aVector == NULL || aInputFile == NULL)
		return false;

	size_t new_size = 0;
	if (fscanf(aInputFile, "%zu", &new_size) != 1)
		return false;

	TVectorElement* new_values = NULL;

	if (new_size != 0){ // Kontrola zda je new_size nějaké číslo
		new_values = malloc(new_size * sizeof(TVectorElement));
		
		if (new_values == NULL) // Kontrola zda došlo k kolokaci
			return false;

		for (size_t i = 0; i < new_size; i++)
			{
			if (vector_element_load_file(&new_values[i], aInputFile) == false)
				{
				free(new_values);
				return false;
				}
			}
		}

	*aVector = (struct TVector){ .iValues = new_values, .iSize = new_size};
	return true;
	}

bool vector_clone(const struct TVector *aVector, struct TVector *aVectorClone)
	{
	if (aVector == NULL || aVectorClone == NULL) // Kontrola vstupu
		return false;

	if (aVector == aVectorClone) // Kontrola aby nebyli dva stejné vektory
		return true;

	TVectorElement* new_values = NULL;

	if (aVector->iSize != 0) // Kontrola zda je aSize nějaké číslo
		{
		new_values = malloc(aVector->iSize * sizeof(TVectorElement));
		if (new_values == NULL)
			return false;

		for (size_t i = 0; i < aVector->iSize; i++) // Klonování vektoru
			{
			new_values[i] = aVector->iValues[i];
			}
		}
	free(aVectorClone->iValues); // Uvolníme aVectorClone


	*aVectorClone = (struct TVector){ .iValues = new_values, .iSize = aVector->iSize};

	return true;
	}

bool vector_store_file(const struct TVector *aVector, FILE *aOutputFile)
	{
	if (aVector == NULL || aOutputFile == NULL) // Kontrola vstupu
		return false;

	if (fprintf(aOutputFile, "%zu", aVector->iSize) < 0) // Zápis dat do souboru
		return false;
	
	if (aVector->iSize != 0)
		{
		for (size_t i = 0; i < aVector->iSize; i++)
			{
			if (vector_element_store_file(aVector->iValues[i], aOutputFile) == false)
				return false;
			}
		}
	
	return true;
	}

TVectorElement vector_value(const struct TVector *aVector, size_t aPos)
	{
	assert(aVector); // Assert používáme jelikož funkce nemůže vracet INT, kontrola vstupů
	assert(aVector->iValues);
	assert(aVector->iSize > aPos);

	return aVector->iValues[aPos];
	}

void vector_set_value(struct TVector *aVector, size_t aPos, TVectorElement aValue)
	{
	assert(aVector);
	assert(aVector->iValues);
	assert(aVector->iSize > aPos);

	aVector->iValues[aPos] = aValue; 
	}

size_t vector_size(const struct TVector *aVector)
	{
	if (aVector->iValues != NULL)
		return aVector->iSize;

	return 0;
	}

bool vector_resize(struct TVector *aVector, size_t aNewSize)
	{
	if (aVector == NULL)
		return false;

	if (aVector->iSize == aNewSize) // Jestli velikosti jsou stejné není nic potřeba dělat
		return true;

	TVectorElement* new_values = NULL;

	if (aNewSize != 0) // Kontrola zda je aSize nějaké číslo
		{
		if (aVector->iSize > aNewSize) // Původní vector je vetší jak číslo aNewSize
			new_values = malloc(aNewSize * sizeof(TVectorElement));
		else
			new_values = calloc(aNewSize, sizeof(TVectorElement));

		if (new_values == NULL)
			return false;

		size_t copy_size = (aVector->iSize > aNewSize) ? (aNewSize) : (aVector->iSize); // Ternární operátor, kopíruji velikost aNewSize nebo aVector->iSize

		for (size_t i = 0; i < copy_size; i++) // Klonování vektoru
			new_values[i] = aVector->iValues[i];
		}
	free(aVector->iValues); // Uvolníme aVectorClone

	*aVector = (struct TVector){ .iValues = new_values, .iSize = aNewSize };
	return true;
	}

void vector_destroy(struct TVector* aVector)
	{
	if (aVector != NULL) // Pokud není prázdné uvolni
		{
		free(aVector->iValues);
		*aVector = (struct TVector){ .iValues = NULL, .iSize = 0 };
		}
	}
struct TVectorIterator vector_iterator_begin(const struct TVector *aVector)
	{
	if (aVector == NULL || aVector->iSize == 0)
		return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };

	return (struct TVectorIterator) { .iVector = (struct TVector*) aVector, .iPos = 0 };
	}

struct TVectorIterator vector_iterator_pos(const struct TVector *aVector, size_t aPos)
	{
	if (aVector == NULL || aPos >= aVector->iSize)
		return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };

	return (struct TVectorIterator) { .iVector = (struct TVector*) aVector, .iPos = aPos };
	}

struct TVectorIterator vector_iterator_last(const struct TVector *aVector)
	{
	if (aVector == NULL || aVector->iSize == 0)
		return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };

	return (struct TVectorIterator) { .iVector = (struct TVector*) aVector, .iPos = aVector->iSize - 1};
	}

bool vector_iterator_is_valid(const struct TVectorIterator *aIter)
	{
	if (aIter != NULL)
		if (aIter->iVector != NULL)
			if ((aIter->iVector->iSize - 1) < aIter->iPos) // Ukazuje mimo
				return true;
	return false;
	}

bool vector_iterator_to_next(struct TVectorIterator *aIter)
	{
	if (vector_iterator_is_valid(aIter) == false)
		return false;
	
	++aIter->iPos;
	if (aIter->iPos >= aIter->iVector->iSize) // Nepřesáhla velikost ?
		{
		*aIter = (struct TVectorIterator){ .iVector = NULL, .iPos = 0 };
		return false;
		}
	
	return true;
	}

bool vector_iterator_to_prev(struct TVectorIterator *aIter)
	{
	if (vector_iterator_is_valid(aIter) == false)
		return false;

	if (aIter->iPos <= 0) // Jsme na nultém prvku tudiž nemůžu posunout dolů
		{
		*aIter = (struct TVectorIterator){ .iVector = NULL, .iPos = 0 };
		return false;
		}

	--aIter->iPos;
	return true;
	}

TVectorElement vector_iterator_value(const struct TVectorIterator *aIter)
	{
	if (vector_iterator_is_valid(aIter) == false)
		return (TVectorElement) { 0 };

	return (TVectorElement) { aIter->iVector->iValues[aIter->iPos] };
	}

bool vector_iterator_set_value(const struct TVectorIterator *aIter, TVectorElement aValue)
	{
	if (vector_iterator_is_valid(aIter) == false)
		return false;
	
	((struct TVector*)aIter->iVector)->iValues[aIter->iPos] = aValue;
	// aIter->iVector->iValues[aIter->iPos] = aValue; // Nelze napsat jelikož je to const struct
	return true;
	}
