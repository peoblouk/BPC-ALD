#ifndef MERGE_SORT_H
#define MERGE_SORT_H
/** \file merge_sort.h
 *  \brief Deklarace API výkonnější metody třídění slučováním (Merge sort), využívající rekurzi
 *  \author Petyovský
 *  \version 2024
 *  $Id: merge_sort.h 2691 2024-04-05 14:58:43Z petyovsky $
 */

#include "TVector.h"

/** \addtogroup PowerfulSorts
 *  \{
 */

/** \brief Třídění pomocí metody slučováním (Merge sort) využívající rekurzivní rozklad
 *  \details Setřídí vzestupně elementy ve vektoru \p aVector rekurzivní variantou metody slučováním.
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void merge_sort(struct TVector *aVector);

/** \} PowerfulSorts */

#endif /* MERGE_SORT_H */
