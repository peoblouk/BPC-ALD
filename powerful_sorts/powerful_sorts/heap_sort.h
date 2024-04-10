#ifndef HEAP_SORT_H
#define HEAP_SORT_H
/** \file heap_sort.h
 *  \brief Deklarace API výkonnější metody třídění (Heap sort), využívající poziční API pro přístup k uzlům haldy
 *  \author Petyovský
 *  \version 2024
 *  $Id: heap_sort.h 2691 2024-04-05 14:58:43Z petyovsky $
 */

#include "TVector.h"

/** \addtogroup PowerfulSorts
 *  \{
 */

/** \brief Třídění pomocí haldy (Heap Sort) využívající poziční API pro přístup k elementům vektoru
 *  \details Setřídí vzestupně elementy ve vektoru \p aVector metodou třídění pomocí haldy.
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void heap_sort(struct TVector *aVector);

/** \} PowerfulSorts */

#endif /* HEAP_SORT_H */
