#ifndef WILDCARD_MATCH_H
#define WILDCARD_MATCH_H
/** \file wildcard_match.h
 *  \brief Deklarace funkce wildcard_match() realizující test na shodu řetězců z nichž jeden obsahuje žolíkové znaky
 *  \author Petyovský
 *  \version 2024
 *  $Id: wildcard_match.h 2663 2024-03-25 10:50:21Z petyovsky $
 */

#include <stdbool.h>

/** \defgroup Wildcard_matching 1. Wildcard matching
 *  \brief Implementace funkce realizující test shody dvou řetězců z nichž jeden obsahuje žolíkové znaky
 *  \{
 */

/** \brief Implementace funkce pro ověření shody řetězců z nichž jeden obsahuje žolíkové znaky
 *  \details Funkce realizující rekurzivní řešení úlohy ověření shody dvou řetězců z nichž jeden obsahuje žolíkové znaky: `*` a `?`.
 *  \param[in] aWildCardStr Vstupní řetězec se vzorem, který může obsahovat zástupné (tzv. žolíkové) znaky
 *  \param[in] aStr Vstupní řetězec reprezentující obecný řetězec
 *  \return \c true pokud test na shodu vzoru a řetězce dopadl úspěšně
 */
bool wildcard_match(const char aWildCardStr[], const char aStr[]);

/** \} Wildcard_matching */

#endif /* WILDCARD_MATCH_H*/
