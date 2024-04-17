#ifndef TSET_ELEMENT_H
#define TSET_ELEMENT_H
/** \file TSetElement.h
 *  \brief Definice typu Basic SetElement a implementace API
 *  \author Petyovský
 *  \version 2024
 *  $Id: TSetElement.h 2704 2024-04-12 15:39:57Z petyovsky $
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "check.h"

/** \defgroup TSetElement 1. SetElement
 *  \brief Definice datového typu SetElement a jeho funkcí
 *  \{
 */

extern size_t gSetElementComparisons;	///< Deklarace globální proměnné realizující počítadlo operací porovnání dvou hodnot typu SetElement

typedef int TSetElement;				///< Definice typu SetElement
#define TSET_ELEMENT_FRMSTR "%d"		///< Definice formátovacího řetězce pro souborové operace s typem SetElement

/** \brief Porovnání dvou elementů
 *  \details Provede trojcestné porovnání hodnot dvou elementů, předaných pomocí ukazatelů.
 *  \param[in] aLeft Ukazatel na levou porovnávanou hodnotu (tzv. LHS - Left Hand Side)
 *  \param[in] aRight Ukazatel na pravou porovnávanou hodnotu (tzv. RHS - Right Hand Side)
 *  \retval -1 Pokud (LHS < RHS)
 *  \retval  0 Pokud (LHS = RHS)
 *  \retval +1 Pokud (LHS > RHS)
 *  \attention Funkce ověřuje platnost obou ukazatelů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline int set_element_comparator(const TSetElement *aLeft, const TSetElement *aRight)
	{
	assert(aLeft);
	assert(aRight);
	++gSetElementComparisons;
	if(*aLeft == *aRight)
		return 0;
	if(*aLeft < *aRight)
		return -1;
	return 1;
	}

/** \brief Načtení elementu ze souboru
 *  \details Načte hodnotu elementu z předem otevřeného souboru.
 *  \param[in,out] aElement Ukazatel na místo v paměti určené pro načtení hodnoty
 *  \param[in,out] aInputFile Ukazatel na soubor otevřený v módu pro čtení
 *  \return \c true pokud byla hodnota elementu ze souboru úspěšně načtena
 *  \attention Funkce ověřuje platnost obou ukazatelů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline bool set_element_load_file(TSetElement *aElement, FILE *aInputFile)
	{
	assert(aElement);
	assert(aInputFile);
	return fscanf(aInputFile, TSET_ELEMENT_FRMSTR, aElement) == 1;
	}

/** \brief Uložení elementu do souboru
 *  \details Uloží hodnotu elementu do předem otevřeného souboru.
 *  \param[in] aElement Hodnota elementu určená pro uložení do souboru
 *  \param[in,out] aOutputFile Ukazatel na soubor otevřený v módu pro zápis
 *  \return \c true pokud byla hodnota elementu do souboru úspěšně uložena
 *  \attention Funkce ověřuje platnost ukazatele \p aOutputFile \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline bool set_element_store_file(TSetElement aElement, FILE *aOutputFile)
	{
	assert(aOutputFile);
	return fprintf(aOutputFile, TSET_ELEMENT_FRMSTR " ", aElement) >= 0;
	}

/** \brief Vrací nový element s náhodnou hodnotou
 *  \details Vytváří a vrací nový element inicializovaný pomocí náhodné hodnoty.
 *  \return Nový element obsahující náhodnou hodnotu
 */
static inline TSetElement set_element_random_value(void)
	{
	return (TSetElement) { (TSetElement)rand() };
	}

/** \} TSetElement */

#endif /* TSET_ELEMENT_H */
