#ifndef ADVANCED_SORTS_H
#define ADVANCED_SORTS_H
/** \file advanced_sorts.h
 *  \brief Deklarace API účinnějších metod třídění
 *  \author Petyovský
 *  \version 2024
 *  $Id: advanced_sorts.h 2680 2024-04-02 10:28:36Z petyovsky $
 */

#include "TVector.h"

/** \defgroup AdvancedSorts 6. Účinnější metody třídění
 *  \brief Deklarace API účinnějších metod třídění
 *  \{
 */

/** \brief Shellovo třídění vkládáním s ubývajícím krokem (Shell Sort) dle Hibbardovy posloupnosti
 *  \details Setřídí vzestupně elementy ve vektoru \p aVector pomocí metody Shellova třídění využívající Hibbardovu posloupnost.
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void shell_sort(struct TVector *aVector);

/** \brief Rychlé třídění výměnou (Quick Sort) s volbou prostředního prvku jako pivota
 *  \details Setřídí vzestupně elementy ve vektoru \p aVector pomocí metody rychlého třídění výměnou s volbou prostředního prvku jako pivota.
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void quick_sort(struct TVector *aVector);

/** \} AdvancedSorts */

#endif /* ADVANCED_SORTS_H */
