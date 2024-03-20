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

/** \brief Definice typu TStudent
 *  \details Typ TStudent obsahuje informace o osobě studenta a jeho bodovém hodnocení.
 */
struct TStudent
	{
	char iFirstName[20 + 1];					///< Jméno studenta (20 znaků + 1 ukončovací znak '\0')
	char iLastName[30 + 1];						///< Příjmení studenta (30 znaků + 1 ukončovací znak '\0')
	size_t iId;									///< ID studenta
	unsigned short iPoints;						///< Bodové hodnocení studenta
	};

typedef struct TStudent TVectorElement;					///< Definice typu VectorElement (alias na typ struct TStudent)
#define TVECTOR_ELEMENT_PRT_FRMSTR_FIRST_NAME "%s"		///< Definice formátovacího řetězce pro tisk položky: \ref TStudent::iFirstName "Jméno studenta"
#define TVECTOR_ELEMENT_SCN_FRMSTR_FIRST_NAME "%20s"	///< Definice formátovacího řetězce pro načtení položky: \ref TStudent::iFirstName "Jméno studenta"
#define TVECTOR_ELEMENT_PRT_FRMSTR_LAST_NAME "%s"		///< Definice formátovacího řetězce pro tisk položky: \ref TStudent::iLastName "Příjmení studenta"
#define TVECTOR_ELEMENT_SCN_FRMSTR_LAST_NAME "%30s"		///< Definice formátovacího řetězce pro načtení položky: \ref TStudent::iLastName "Příjmení studenta"
#define TVECTOR_ELEMENT_FRMSTR_ID "%zu"					///< Definice formátovacího řetězce pro tisk i načtení položky: \ref TStudent::iId "ID studenta"
#define TVECTOR_ELEMENT_FRMSTR_POINTS "%hu"				///< Definice formátovacího řetězce pro tisk i načtení položky: \ref TStudent::iPoints "Bodové hodnocení studenta"

/** \brief Definice výčtového typu TVectorElementCompareSelect
 *  \details Výčtový typ TVectorElementCompareSelect určuje podle které složky struktury TStudent bude prováděno porovnání.
 */
enum TVectorElementCompareSelect
	{
	EByFirstName,								///< Porovnání dle položky: \ref TStudent::iFirstName "Jméno studenta"
	EByLastName,								///< Porovnání dle položky: \ref TStudent::iLastName "Příjmení studenta"
	EById,										///< Porovnání dle položky: \ref TStudent::iId "ID studenta"
	EByPoints									///< Porovnání dle položky: \ref TStudent::iPoints "Bodové hodnocení studenta"
	};

extern enum TVectorElementCompareSelect compare_by;			///< Deklarace globální proměnné udržující informaci o položce zvolené pro porovnání dvou elementů

/** \brief Nastavení položky pro porovnání dvou elementů
 *  \details Nastaví dle zadaného parametru položku struktury, podle které budou prováděna všechna následující porovnání dvou elementů.
 *  \param[in] aCompareBy Definuje podle které složky struktury TStudent bude prováděno porovnání (viz \ref TVectorElementCompareSelect)
 */
static inline void vector_element_set_comparator(enum TVectorElementCompareSelect aCompareBy)
	{
	compare_by = aCompareBy;
	}

/** \brief Porovnání dvou elementů
 *  \details Provede trojcestné porovnání hodnot dvou elementů, předaných pomocí ukazatelů.
 *  \param[in] aLeft Ukazatel na levou porovnávanou hodnotu (tzv. LHS - Left Hand Side)
 *  \param[in] aRight Ukazatel na pravou porovnávanou hodnotu (tzv. RHS - Right Hand Side)
 *  \retval -1 Pokud (LHS < RHS)
 *  \retval  0 Pokud (LHS = RHS)
 *  \retval +1 Pokud (LHS > RHS)
 *  \attention Funkce ověřuje platnost obou ukazatelů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 *  \attention Funkce hlásí běhovou chybu `perror` v případě, že byla zvolená neznámá volba položky pro porovnání!
 */
static inline int vector_element_comparator(const TVectorElement *aLeft, const TVectorElement *aRight)
	{
	assert(aLeft);
	assert(aRight);

	switch(compare_by)
		{
		case EByPoints:
			if(aLeft->iPoints == aRight->iPoints)
				return 0;
			if(aLeft->iPoints < aRight->iPoints)
				return -1;
			return 1;
		break; /* EByPoints */

		case EById:
			if(aLeft->iId == aRight->iId)
				return 0;
			if(aLeft->iId < aRight->iId)
				return -1;
			return 1;
		break; /* EById */

		case EByFirstName:
			return strcmp(aLeft->iFirstName, aRight->iFirstName);
		break; /* EByFirstName */

		case EByLastName:
			return strcmp(aLeft->iLastName, aRight->iLastName);
		break; /* EByLastName */

		default:
			perror("Unknown TVectorElementCompareSelect value");
		break;
		}
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

	//if(fscanf(aInputFile, TVECTOR_ELEMENT_SCN_FRMSTR_FIRST_NAME, aElement->iFirstName) != 1)
	//	return false;

	//if(fscanf(aInputFile, TVECTOR_ELEMENT_SCN_FRMSTR_LAST_NAME, aElement->iLastName) != 1)
	//	return false;

	//if(fscanf(aInputFile, TVECTOR_ELEMENT_FRMSTR_ID, &aElement->iId) != 1)
	//	return false;

	//return fscanf(aInputFile, TVECTOR_ELEMENT_FRMSTR_POINTS, &aElement->iPoints) == 1;

	return fscanf(aInputFile,
				  TVECTOR_ELEMENT_SCN_FRMSTR_FIRST_NAME
				  TVECTOR_ELEMENT_SCN_FRMSTR_LAST_NAME
				  TVECTOR_ELEMENT_FRMSTR_ID
				  TVECTOR_ELEMENT_FRMSTR_POINTS,
				  aElement->iFirstName, aElement->iLastName, &aElement->iId, &aElement->iPoints) == 4;
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

	//if(fprintf(aOutputFile, TVECTOR_ELEMENT_PRT_FRMSTR_FIRST_NAME " ", aElement.iFirstName) < 1)
	//	return false;

	//if(fprintf(aOutputFile, TVECTOR_ELEMENT_PRT_FRMSTR_LAST_NAME " ", aElement.iLastName) < 1)
	//	return false;

	//if(fprintf(aOutputFile, TVECTOR_ELEMENT_FRMSTR_ID " ", aElement.iId) < 2)
	//	return false;

	//return fprintf(aOutputFile, TVECTOR_ELEMENT_FRMSTR_POINTS "\n" aElement.iPoints) >= 2;

	return fprintf(aOutputFile,
				   TVECTOR_ELEMENT_PRT_FRMSTR_FIRST_NAME " "
				   TVECTOR_ELEMENT_PRT_FRMSTR_LAST_NAME " "
				   TVECTOR_ELEMENT_FRMSTR_ID " "
				   TVECTOR_ELEMENT_FRMSTR_POINTS "\n",
				   aElement.iFirstName, aElement.iLastName, aElement.iId, aElement.iPoints) >= 6;
	}

/** \brief Vrací nový element s náhodnou hodnotou
 *  \details Vytváří a vrací nový element inicializovaný pomocí náhodné hodnoty.
 *  \return Nový element obsahující náhodné hodnoty
 */
static inline TVectorElement vector_element_random_value(void)
	{
	return (TVectorElement) {
							.iFirstName = "Franta",
							.iLastName = "Jetel",
							.iId = rand(),
							.iPoints = (unsigned short) ((100LL * rand()) / RAND_MAX)
							};
	}

/** \} TVectorElement */

#endif /* TVECTOR_ELEMENT_H */
