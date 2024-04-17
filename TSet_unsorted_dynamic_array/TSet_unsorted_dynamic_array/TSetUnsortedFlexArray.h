#ifndef TSET_UNSORTED_FLEX_ARRAY_H
#define TSET_UNSORTED_FLEX_ARRAY_H
/** \file TSetUnsortedFlexArray.h
 *  \brief Deklarace typu SetUnsortedFlexArray a jeho API pro realizaci množiny pomocí nesetříděného flexibilního dynamicky alokovaného pole
 *  \author Petyovský
 *  \version 2024
 *  $Id: TSetUnsortedFlexArray.h 2711 2024-04-12 16:58:56Z petyovsky $
 */

#include <stdbool.h>
#include <stddef.h>
#include "TSetElement.h"

/** \defgroup TSetUnsortedFlexArray 2.1. SetUnsortedFlexArray
 *  \brief Deklarace typu SetUnsortedFlexArray a jeho funkcí (pro realizaci množiny pomocí nesetříděného flexibilního dynamicky alokovaného pole)
 *  \ingroup TSet
 *  \{
 */

/** \brief Deklarace privátního typu SetUnsortedFlexArray
 */
struct TSetUnsortedFlexArray;
//typedef size_t TSetUnsortedFlexArrayPos;	///< Definice typu pro pozici v SetUnsortedFlexArray

/** \brief Funkce zjišťující, zda flexibilní pole obsahuje element o zadané hodnotě
 *  \details Funkce (predikát) vracející \c bool hodnotu reprezentující test, zda flexibilní pole obsahuje zadanou hodnotu elementu.
 *  \param[in] aFlexArray Ukazatel na existující flexibilní pole
 *  \param[in] aValue Hodnota elementu hledaná v poli
 *  \param[in,out] aPosPtr Ukazatel na proměnnou určenou pro uložení pozice na hledaný element v poli (hodnota pozice je do paměti zapsána vždy tj. bez ohledu na výsledek hledání)
 *  \param[in] aSize Aktuální počet elementů ve flexibilním poli
 *  \retval -2 Pokud má pole nulovou délku, (\p *aPosPtr = 0)
 *  \retval -1 Pokud element nebyl v poli nalezen a hledaná hodnota elementu je menší, než hodnota na poslední platné pozici \p *aPosPtr
 *  \retval  0 Pokud element byl v poli nalezen a hledaná hodnota elementu je na pozici \p *aPosPtr
 *  \retval +1 Pokud element nebyl v poli nalezen a hledaná hodnota elementu je větší, než hodnota na poslední platné pozici \p *aPosPtr
 *  \note Pokud element s danou hodnotou nebyl nalezen, obsahuje \p *aPosPtr poslední platnou pozici, kde bylo hledání ukončeno.
 *  \attention Funkce ověřuje platnost obou ukazatelů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
int set_flex_array_search(const struct TSetUnsortedFlexArray *aFlexArray, TSetElement aValue, size_t *aPosPtr, size_t aSize);

/** \brief Vložení elementu do flexibilního pole
 *  \details Vkládá unikátní hodnotu elementu na konec flexibilního pole.
 *  \param[in,out] aFlexArrayPtr Ukazatel na místo v paměti, kde je ukazatel na flexibilní pole
 *  \param[in] aValue Hodnota elementu vkládaná do flexibilního pole
 *  \param[in] aSize Aktuální počet elementů ve flexibilním poli
 *  \return \c true pokud hodnota elementu nebyla v původním poli nalezena a pokud byla tato hodnota do pole následně úspěšně vložena
 *  \attention Funkce ověřuje platnost ukazatele \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
bool set_flex_array_insert(struct TSetUnsortedFlexArray **aFlexArrayPtr, TSetElement aValue, size_t aSize);

/** \brief Odstranění elementu z flexibilního pole
 *  \details Odstraní hodnotu elementu z flexibilního pole.
 *  \param[in,out] aFlexArrayPtr Ukazatel na místo v paměti, kde je ukazatel na flexibilní pole
 *  \param[in] aValue Hodnota elementu odebíraného z flexibilního pole
 *  \param[in] aSize Aktuální počet elementů ve flexibilním poli
 *  \return \c true pokud hodnota elementu byla v původním poli nalezena a pokud byla tato hodnota z pole následně úspěšně odstraněna
 *  \attention Funkce ověřuje platnost ukazatele a parametru \p aSize \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
bool set_flex_array_erase(struct TSetUnsortedFlexArray **aFlexArrayPtr, TSetElement aValue, size_t aSize);

/** \brief Deinicializace flexibilního pole
 *  \details Deinicializace a odalokace paměti flexibilního pole.
 *  \param[in,out] aFlexArray Ukazatel na flexibilní pole
 */
void set_flex_array_destroy(struct TSetUnsortedFlexArray *aFlexArray);

/** \brief Výpočet první platné pozice ve flexibilním poli
 *  \details Funkce vrací první pozici prvního platného elementu ve flexibilním poli.
 *  \param[in] aFlexArray Ukazatel na existující flexibilní pole
 *  \return Pozice (index) prvního platného elementu ve flexibilním poli
 */
size_t set_flex_array_begin_pos(const struct TSetUnsortedFlexArray *aFlexArray);

/** \brief Zjištění platnosti pozice ve flexibilním poli
 *  \details Funkce (predikát) vracející \c bool hodnotu definující platnost zadaného indexu do flexibilního pole.
 *  \param[in] aFlexArray Ukazatel na existující flexibilní pole
 *  \param[in] aPos Pozice (index) elementu ve flexibilním poli
 *  \param[in] aSize Aktuální počet elementů ve flexibilním poli
 *  \return \c true pokud je pozice platným indexem do flexibilního pole
 */
bool set_flex_array_is_valid_pos(const struct TSetUnsortedFlexArray *aFlexArray, size_t aPos, size_t aSize);

/** \brief Výpočet následující platné pozice ve flexibilním poli
 *  \details Výpočet pozice následujícího platného elementu ve flexibilním poli.
 *  \param[in] aFlexArray Ukazatel na existující flexibilní pole
 *  \param[in] aPos Pozice (index) aktuálního elementu ve flexibilním poli
 *  \return Pozice (index) následujícího platného elementu ve flexibilním poli
 */
size_t set_flex_array_next_pos(const struct TSetUnsortedFlexArray *aFlexArray, size_t aPos);

/** \brief Přečtení hodnoty elementu z flexibilního pole
 *  \details Přečte hodnotu elementu ze zadané pozice ve flexibilním poli.
 *  \param[in] aFlexArray Ukazatel na flexibilní pole
 *  \param[in] aPos Pozice (index) elementu ve flexibilním poli
 *  \return Hodnota elementu flexibilního pole na dané pozici.
 *  \attention Funkce ověřuje platnost ukazatele a parametru \p aSize \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
TSetElement set_flex_array_value_at_pos(const struct TSetUnsortedFlexArray *aFlexArray, size_t aPos);

/** \} TSetUnsortedFlexArray */

#endif /* TSET_UNSORTED_FLEX_ARRAY_H */
