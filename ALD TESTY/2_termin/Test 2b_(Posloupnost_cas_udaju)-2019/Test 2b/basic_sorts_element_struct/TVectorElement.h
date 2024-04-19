#ifndef TVECTOR_ELEMENT_H
#define TVECTOR_ELEMENT_H
/** \file TVectorElement.h
 *  \brief Definice strukturovaného typu VectorElement a implementace API
 *  \author Petyovský
 *  \version 2024
 *  $Id: TVectorElement.h 2631 2024-03-08 16:40:08Z petyovsky $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "check.h"

/** \defgroup TVectorElement 1. VectorElement (strukturovaný)
 *  \brief Definice strukturovaného datového typu VectorElement a jeho funkcí
 *  \{
 */

struct TVectorSortStats
	{
	size_t iComparisons;			///< Celkový počet operací porovnání hodnot mezi dvěma elementy vektoru
	size_t iSwaps;					///< Celkový počet operací výměny hodnot mezi dvěma elementy vektoru
	size_t iMoves;					///< Celkový počet operací přesunu hodnoty mezi dvěma elementy vektoru
	size_t iGets;					///< Celkový počet operací přečtení hodnoty z elementu vektoru
	size_t iSets;					///< Celkový počet operací zápisu hodnoty do elementu vektoru
	};

struct TTime
	{
	unsigned iHours;
	unsigned iMinutes;
	unsigned iSeconds;
	};

typedef struct TTime TVectorElement;

struct TVector
	{
	TVectorElement* iValues;		///< Ukazatel na počáteční prvek pole hodnot typu VectorElement
	size_t iSize;					///< Počet elementů vektoru
	struct TVectorSortStats iStats;	///< Statistika počtu operací s elementy Sortable vektoru (_mutable položka - může se měnit i když je Vektor konstantní strukturou_)
	};

enum TVectorElementCompareSelect
	{
	EAscending,
	EDescending,
	};

enum TVectorElementSortingBy
	{
	EByHours,
	EByMinutes,
	EBySeconds,
	};

enum TVectorElementCompareSelect compare_by;
enum TVectorElementSortingBy sorting_by;

static inline void vector_element_set_comparator(enum TVectorElementCompareSelect aCompareBy, enum TVectorElementSortingBy aSortingBy)
	{
	compare_by = aCompareBy;
	sorting_by = aSortingBy;
	}

static inline void vector_element_comparator(struct TVector* tVector) // Musí se pozměnit
	{
	assert(tVector);

	if (compare_by == EAscending)
		{
		if (sorting_by == EByHours)
			{
			for (int i = 1; i < tVector->iSize; i++)
				{
				unsigned temp_hours = tVector->iValues[i].iHours;
				unsigned temp_minutes = tVector->iValues[i].iMinutes;
				unsigned temp_seconds = tVector->iValues[i].iSeconds;

				int j = i - 1;

				while (j >= 0 && tVector->iValues[j].iHours > temp_hours)
					{
					tVector->iValues[j + 1].iHours = tVector->iValues[j].iHours;
					tVector->iValues[j + 1].iMinutes = tVector->iValues[j].iMinutes;
					tVector->iValues[j + 1].iSeconds = tVector->iValues[j].iSeconds;
					j--;
					}

				tVector->iValues[j + 1].iHours = temp_hours;
				tVector->iValues[j + 1].iMinutes = temp_minutes;
				tVector->iValues[j + 1].iSeconds = temp_seconds;
				}
			}

		else if (sorting_by == EByMinutes)
			{
			for (int i = 1; i < tVector->iSize; i++)
				{
				unsigned temp_hours = tVector->iValues[i].iHours;
				unsigned temp_minutes = tVector->iValues[i].iMinutes;
				unsigned temp_seconds = tVector->iValues[i].iSeconds;

				int j = i - 1;

				while (j >= 0 && tVector->iValues[j].iMinutes > temp_minutes)
					{
					tVector->iValues[j + 1].iHours = tVector->iValues[j].iHours;
					tVector->iValues[j + 1].iMinutes = tVector->iValues[j].iMinutes;
					tVector->iValues[j + 1].iSeconds = tVector->iValues[j].iSeconds;
					j--;
					}

				tVector->iValues[j + 1].iHours = temp_hours;
				tVector->iValues[j + 1].iMinutes = temp_minutes;
				tVector->iValues[j + 1].iSeconds = temp_seconds;
				}
			}

		else if (sorting_by = EBySeconds)
			{
			for (int i = 1; i < tVector->iSize; i++)
				{
				unsigned temp_hours = tVector->iValues[i].iHours;
				unsigned temp_minutes = tVector->iValues[i].iMinutes;
				unsigned temp_seconds = tVector->iValues[i].iSeconds;

				int j = i - 1;

				while (j >= 0 && tVector->iValues[j].iSeconds > temp_seconds)
					{
					tVector->iValues[j + 1].iHours = tVector->iValues[j].iHours;
					tVector->iValues[j + 1].iMinutes = tVector->iValues[j].iMinutes;
					tVector->iValues[j + 1].iSeconds = tVector->iValues[j].iSeconds;
					j--;
					}

				tVector->iValues[j + 1].iHours = temp_hours;
				tVector->iValues[j + 1].iMinutes = temp_minutes;
				tVector->iValues[j + 1].iSeconds = temp_seconds;
				}
			}
		}
	}


// načtení údajů ze souboru
static inline bool vector_element_load_file(TVectorElement* aElement, FILE* aInputFile)
	{
	assert(aElement);
	assert(aInputFile);

	// vrací true v případě, že se ze souboru úspěšně načtou 3 hodnoty
	return fscanf(aInputFile, "%u:%u:%u", &aElement->iHours, &aElement->iMinutes, &aElement->iSeconds) == 3;
	}

// načtení údajů do souboru
static inline bool vector_element_store_file(TVectorElement aElement, FILE* aOutputFile)
	{
	assert(aOutputFile);

	// vrací true v případě, že se do souboru úspěšně zapíše 8 hodnot (1 znak hodiny + 1 dvojtečka + 2 znaky minuty + 1 dvojtečka + 2 znaky sekundy + 1 odřadkování)
	return fprintf(aOutputFile, "%u:%02u:%02u\n", aElement.iHours, aElement.iMinutes, aElement.iSeconds) == 8;
	}

bool vector_init_file(struct TVector* aVector, FILE* aInputFile)
	{
	if (aVector == NULL || aInputFile == NULL)
		{
		return false;
		}

	size_t new_size = 0;

	if (fscanf(aInputFile, "%zu", &new_size) != 1)
		{
		printf("Chyba pri cteni poctu dat!");
		return false;;
		}

	TVectorElement* new_values = NULL;

	if (new_size != 0)
		{
		new_values = malloc(new_size * sizeof(TVectorElement));

		if (new_values == NULL)
			{
			return false;
			}

		for (size_t i = 0; i < new_size; ++i)
			{
			if (vector_element_load_file(&new_values[i], aInputFile) == false)
				{
				free(new_values);
				return false;
				}
			}
		}

	// *aVector = (struct TVector){ .iValues = new_values, .iSize = new_size };
	aVector->iValues = new_values;
	aVector->iSize = new_size;

	return true;
	}

bool vector_store_file(const struct TVector* aVector, FILE* aOutputFile)
	{
	if (aVector == NULL || aOutputFile == NULL)
		{
		return false;
		}

	assert((!aVector->iValues && !aVector->iSize) || (aVector->iValues && aVector->iSize));

	// pokud by fprintf zapsala do souboru méně než 2 znaky (potřebujeme zapsat 2 znaky: iSize + \n), tak funkce vrátí false
	if (fprintf(aOutputFile, "%zu\n", aVector->iSize) < 2)
		{
		return false;
		}

	if (aVector->iValues != NULL)
		{
		for (size_t i = 0; i < aVector->iSize; ++i)
			{
			if (vector_element_store_file(aVector->iValues[i], aOutputFile) == false)
				{
				return false;
				}
			}
		}

	return true;
	}

/** \brief Vrací nový element s náhodnou hodnotou
 *  \details Vytváří a vrací nový element inicializovaný pomocí náhodné hodnoty.
 *  \return Nový element obsahující náhodné hodnoty
 */
static inline TVectorElement vector_element_random_value(void)
	{
	return (TVectorElement) {
							.iHours = 99,
							.iMinutes = 59,
							.iSeconds = 59
							};
	}

/** \} TVectorElement */

#endif /* TVECTOR_ELEMENT_H */
