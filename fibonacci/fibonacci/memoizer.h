#ifndef MEMOIZER_H
#define MEMOIZER_H
/** \file memoizer.h
 *  \brief Definice typu reprezentující memoizační funktor
 *  \author Petyovský
 *  \version 2024
 *  $Id: memoizer.h 2663 2024-03-25 10:50:21Z petyovsky $
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/** \defgroup TMemoizer 3. Memoizer
 *  \brief Definice datového typu TMemoizer a jeho funkcí
 *  \{
 */

/** \defgroup TMemoizedFunc 3.1 Memoizovaná funkce
 *  \brief Definice datových typů definujících memoizovanou funkcí
 *  \{
 */
typedef unsigned TMemoizedFuncInput;	///< Definice vstupního typu pro memoizer (shodný typ jako má vstupní argument memoizované funkce)
typedef unsigned TMemoizedFuncOutput;	///< Definice výstupního typu pro memoizer (shodný typ jako má návratová hodnota memoizované funkce)
typedef TMemoizedFuncOutput TMemoizedFunc(TMemoizedFuncInput);	///< Definice typu TMemoizedFunc reprezentující požadovaný prototyp pro memoizované funkce
/** \} TMemoizedFunc */

/** \brief Definice typu TMemoizerItem
 *  \details Typ MemoizerItem představuje archivovaný pár hodnot: vstup a výstup (návratová hodnota) memoizované funkce 
 */
struct TMemoizerItem
	{
	TMemoizedFuncInput iInput;			///< Memoizovaná hodnota vstupního argumentu funkce (vstupní hodnota)
	TMemoizedFuncOutput iOutput;		///< Memoizovaná návratová hodnota funkce (pro danou vstupní hodnotu)
	};

/** \brief Definice typu MemoizerStats
 *  \details Typ MemoizerStats spravuje statistiky o celkovém počtu použití memoizeru, počtu nalezení dříve spočtené hodnoty funkce a počtu nutných volání asociované funkce.
 */
struct TMemoizerStats
	{
	size_t iMemoizations;				///< Celkový počet úspěšných nálezů dříve spočtené hodnoty funkce pro zadaný parametr, (Memoizer Hit)
	size_t iCalculations;				///< Celkový počet neúspěšných nálezů dříve spočtené hodnoty funkce, které vedou k novému volání funkce, (Memoizer Miss)
	};

/** \brief Definice typu TMemoizer
 *  \details Typ Memoizer spravuje dynamicky alokované pole prvků typu MemoizerItem a umožňuje s nimi pracovat pomocí definovaného API.
 */
struct TMemoizer
	{
	TMemoizedFunc *iMemoizedFunc;		///< Ukazatel na asociovanou funkci vracející typ \c TMemoizedFuncOutput a mající jeden parametr typu \c TMemoizedFuncInput
	struct TMemoizerItem *iArchive;		///< Ukazatel na počáteční prvek pole (archiv) hodnot typu MemoizerItem (tj. pár: vstup a výstup funkce)
	size_t iCapacity;					///< Celkový počet alokovaných prvků v poli (archivu) hodnot typu MemoizerItem
	size_t iPos;						///< Aktuální index prvku v poli (archivu) hodnot typu MemoizerItem určeného pro vložení nově zapamatovaného páru.
	size_t iSize;						///< Aktuálně archivovaný počet prvků v poli (archivu) hodnot typu MemoizerItem
	struct TMemoizerStats iStats;		///< Statistika počtu volání memoizeru
	TMemoizedFuncOutput iTmpOutput;		///< Optimalizace pro úsporu 1 proměnné na zásobníku pro každou úroveň rekurze
	};

/** \brief Inicializace memoizeru, asociace s memoizovanou funkcí, alokace vnitřního pole pro pár hodnot vstup a výstup memoizované funkce
 *  \details Dynamicky alokuje paměť pro pole o zadaném počtu prvků, provede asociaci s memoizovanou funkcí, inicializuje všechny složky na poč. hodnoty.
 *  \param[in,out] aM Ukazatel na místo v paměti určené pro inicializaci memoizeru
 *  \param[in] aCapacity Požadovaný počet prvků vnitřního pole určeného pro zapamatování párů vstupní a návratová hodnota memoizované funkce
 *  \param[in] aMemoizedFunc Ukazatel na asociovanou funkci vracející typ \c TMemoizedFuncOutput a mající jeden parametr typu \c TMemoizedFuncInput
 *  \return \c true pokud byl memoizer úspěšně inicializován a alokováno vnitřní pole
 */
static inline bool memoizer_init(struct TMemoizer *aM, size_t aCapacity, TMemoizedFunc *aMemoizedFunc)
	{
	if(!aM || !aMemoizedFunc || !aCapacity)
		return false;

	struct TMemoizerItem *archive = malloc(aCapacity * sizeof(struct TMemoizerItem));
	if(!archive)
		return false;

	*aM = (struct TMemoizer) { .iMemoizedFunc = aMemoizedFunc, .iArchive = archive, .iCapacity = aCapacity, };
	return true;
	}

/** \brief Volání memoizeru zajišťující získání výsledku asociované funkce pro zadaný parametr.
 *  \details Prohledá archiv zapamatovaných párů vstup, výstup a vrací uloženou návratovou hodnotu asociované funkce pro zadanou hodnotu parametru. Pokud návratová hodnota funkce pro zadanou vstupní hodnotu nebyla nalezena (není archivována), určí výsledek pomocí volání asociované funkce a tuto hodnotu si archivuje. Pokud je archiv již plný, přepíše nejstarší archivovaný pár.
 *  \param[in,out] aM Ukazatel na existující memoizer
 *  \param[in] aInput Hodnota požadovaného vstupního parametru asociované funkce
 *  \return Návratová hodnota asociované funkce pro zadaný vstupní parametr
 */
static inline TMemoizedFuncOutput memoizer_eval(struct TMemoizer *aM, TMemoizedFuncInput aInput)
	{
	assert(aM);
	assert(aM->iArchive);
	assert(aM->iMemoizedFunc);

	for(size_t i = 0; i < aM->iSize; ++i)
		if(aM->iArchive[i].iInput == aInput)
			{
			++aM->iStats.iMemoizations;
			return aM->iArchive[i].iOutput;
			}
	
	aM->iTmpOutput = aM->iMemoizedFunc(aInput);
	++aM->iStats.iCalculations;
	aM->iPos %= aM->iCapacity;
	aM->iArchive[aM->iPos] = (struct TMemoizerItem) { .iInput = aInput, .iOutput = aM->iTmpOutput };
	
	if(aM->iSize < aM->iCapacity)
		++aM->iSize;
	
	return aM->iArchive[aM->iPos++].iOutput;
	}

/** \brief Zobrazení hodnoty statistik počtu volání memoizeru
 *  \details Vytiskne na konzolu (stdout) celkové počty volání memoizeru, počty nalezení dříve spočtené hodnoty funkce a počty nutných volání funkce.
 *  \param[in] aM Ukazatel na memoizer jehož statistiky chceme zobrazit
 *  \param[in,out] aOutputFile Ukazatel na soubor otevřený v módu pro zápis
 *  \return \c true pokud byl ukazatel na memoizer platný a statistiky byly zobrazeny na konzolu
 */
static inline bool memoizer_print_stats(const struct TMemoizer *aM, FILE *aOutputFile)
	{
	if(!aM || !aOutputFile)
		return false;

	return fprintf(aOutputFile,
				"\tMemoizations: %zu\n"
				"\tCalculations: %zu\n"
				"\tTotal: %zu\n",
				aM->iStats.iMemoizations, aM->iStats.iCalculations, aM->iStats.iMemoizations + aM->iStats.iCalculations) >= 0;
	}

/** \brief Smazání archivovaných hodnot v memoizeru, vynulování statistik o počty volání memoizeru
 *  \details Smaže archivované páry pro vstup a návratovou hodnotu memoizované funkce, vynuluje statistiky o počty volání memoizeru. Ponechává asociaci s memoizovanou funkcí a také ponechá alokovanou paměť pole hodnot typu MemoizerItem tak, aby bylo možné memoizer znovu používat.
 *  \param[in,out] aM Ukazatel na existující memoizer jehož archiv a statistiky chceme smazat
 *  \return \c true pokud byl ukazatel na memoizer platný a zapamatované hodnoty a statistiky byly vymazány
 */
static inline bool memoizer_clear(struct TMemoizer *aM)
	{
	if(!aM)
		return false;

	aM->iPos = aM->iSize = 0;
	aM->iStats = (struct TMemoizerStats) { 0, };
	return true;
	}

/** \brief Deinicializace memoizeru
 *  \details Deinicializuje memoizer, dealokuje pole hodnot typu MemoizerItem, ruší asociaci s memoizovanou funkcí, nuluje statistiky.
 *  \param[in,out] aM Ukazatel na existující memoizer
 */
static inline void memoizer_destroy(struct TMemoizer *aM)
	{
	if(!aM)
		return;

	free(aM->iArchive);
	*aM = (struct TMemoizer) { 0, };
	}

/** \} TMemoizer */

#endif /* MEMOIZER_H */
