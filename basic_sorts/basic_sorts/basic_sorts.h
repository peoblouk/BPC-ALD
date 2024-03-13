#ifndef BASIC_SORTS_H
#define BASIC_SORTS_H
/** \file basic_sorts.h
 *  \brief Deklarace API přímých metod třídění
 *  \author Petyovský
 *  \version 2024
 *  $Id: basic_sorts.h 2631 2024-03-08 16:40:08Z petyovsky $
 */

#include "TVector.h"

/** \defgroup BasicSorts 3. Přímé metody třídění
 *  \brief Deklarace API přímých metod třídění
 *  \{
 */

/** \brief Vytisknutí elementu vektoru pomocí iterátoru
 *  \details Tisk hodnoty elementu vektoru, na který ukazuje iterátor.
 *  \param[in] aIter Ukazatel na existující iterátor
 */
void print_element(const struct TVectorIterator *aIter);

/** \brief Třídění přímým vkládáním (Insert Sort)
 *  \details Setřídí vzestupně elementy v zadaném vektoru \p aVector pomocí metody třídění přímým vkládáním (Insert Sort).
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void insert_sort(struct TVector *aVector);

/** \brief Třídění přímým výběrem (Select Sort)
 *  \details Setřídí vzestupně elementy v zadaném vektoru \p aVector pomocí metody třídění přímým výběrem (Select Sort).
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void select_sort(struct TVector *aVector);

/** \brief Třídění přímou výměnou (Bubble Sort)
 *  \details Setřídí vzestupně elementy v zadaném vektoru \p aVector pomocí metody třídění přímou výměnou (Bubble Sort).
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void bubble_sort(struct TVector *aVector);

/** \brief Třídění přímou výměnou včetně optimalizace s pomocným příznakem ukončení třídění (Bubble Sort Optimized)
 *  \details Setřídí vzestupně elementy v zadaném vektoru \p aVector pomocí metody třídění přímou výměnou využívající optimalizaci s pomocným příznakem ukončení. Příznak umožňuje dřívější zastavení algoritmu, pokud jsou již data setříděna (Bubble Sort Optimized).
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void bubble_sort_optimized(struct TVector *aVector);

/** \brief Třídění přímou výměnou včetně optimalizace s výpočtem indexu poslední změny (Bubble Sort Optimized2)
 *  \details Setřídí vzestupně elementy v zadaném vektoru \p aVector pomocí metody třídění přímou výměnou využívající optimalizaci s výpočtem indexu poslední změny (Bubble Sort Optimized2).
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void bubble_sort_optimized2(struct TVector *aVector);

/** \brief Třídění přímou výměnou se změnou postupu třídění (Shaker Sort)
 *  \details Setřídí vzestupně elementy v zadaném vektoru \p aVector pomocí metody třídění přímou výměnou se změnou postupu třídění (Shaker Sort).
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void shaker_sort(struct TVector *aVector);

/** \} BasicSorts */

#endif /* BASIC_SORTS_H */
