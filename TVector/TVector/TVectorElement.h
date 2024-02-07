#ifndef TVECTOR_ELEMENT_H
#define TVECTOR_ELEMENT_H
/** \file TVectorElement.h
 *  \brief Definice typu Basic VectorElement a implementace API
 *  \author Petyovský
 *  \version 2024
 *  $Id: TVectorElement.h 2584 2024-02-05 14:33:25Z petyovsky $
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "check.h"
/** \defgroup TVectorElement 1. VectorElement
 *  \brief Definice datového typu VectorElement a jeho funkcí
 *  \{
 */

typedef int TVectorElement;				///< Definice typu VectorElement
#define TVECTOR_ELEMENT_FRMSTR "%d"		///< Definice formátovacího řetězce pro souborové operace s typem VectorElement

/** \brief Porovnání dvou elementů
 *  \details Provede trojcestné porovnání hodnot dvou elementů, předaných pomocí ukazatelů.
 *  \param[in] aLeft Ukazatel na levou porovnávanou hodnotu (tzv. LHS - Left Hand Side)
 *  \param[in] aRight Ukazatel na pravou porovnávanou hodnotu (tzv. RHS - Right Hand Side)
 *  \retval -1 Pokud (LHS < RHS)
 *  \retval  0 Pokud (LHS = RHS)
 *  \retval +1 Pokud (LHS > RHS)
 *  \attention Funkce ověřuje platnost obou ukazatelů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline int vector_element_comparator(const TVectorElement *aLeft, const TVectorElement *aRight)
	{
	assert(aLeft);
	assert(aRight);
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
static inline bool vector_element_load_file(TVectorElement *aElement, FILE *aInputFile)
	{
	assert(aElement);
	assert(aInputFile);
	return fscanf(aInputFile, TVECTOR_ELEMENT_FRMSTR, aElement) == 1;
	}

/** \brief Uložení elementu do souboru
 *  \details Uloží hodnotu elementu do předem otevřeného souboru.
 *  \param[in] aElement Hodnota elementu určená pro uložení do souboru
 *  \param[in,out] aOutputFile Ukazatel na soubor otevřený v módu pro zápis
 *  \return \c true pokud byla hodnota elementu do souboru úspěšně uložena
 *  \attention Funkce ověřuje platnost ukazatele \p aOutputFile \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline bool vector_element_store_file(TVectorElement aElement, FILE *aOutputFile)
	{
	assert(aOutputFile);
	return fprintf(aOutputFile, TVECTOR_ELEMENT_FRMSTR " ", aElement) >= 0;
	}

/** \brief Vrací nový element s náhodnou hodnotou
 *  \details Vytváří a vrací nový element inicializovaný pomocí náhodné hodnoty.
 *  \return Nový element obsahující náhodnou hodnotu
 */
static inline TVectorElement vector_element_random_value(void)
	{
	return (TVectorElement) { (TVectorElement)rand() };
	}

/** \} TVectorElement */

#endif /* TVECTOR_ELEMENT_H */
