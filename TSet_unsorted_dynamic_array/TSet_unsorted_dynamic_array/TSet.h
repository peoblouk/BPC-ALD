#ifndef TSET_H
#define TSET_H
/** \file TSet.h
 *  \brief Definice typu Set - (realizace množiny pomocí nesetříděného flexibilního dynamicky alokovaného pole)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TSet.h 2704 2024-04-12 15:39:57Z petyovsky $
 */

#include <stdbool.h>
#include <stddef.h>
#include "TSetUnsortedFlexArray.h"
#include "check.h"

/** \defgroup TSet 2. Set
 *  \brief Definice datového typu Set a jeho funkcí (realizace množiny pomocí nesetříděného dynamicky alokovaného pole)
 *  \{
 */

/** \brief Definice typu Set
 *  \details Typ Set obsahuje nesetříděné pole prvků typu SetElement a umožňuje s nimi pracovat pomocí definovaného API.
 */
struct TSet
	{
	size_t iSize;								///< Počet elementů množiny
	struct TSetUnsortedFlexArray *iFlexArray;	///< PIMPL ukazatel na hodnoty typu SetUnsortedFlexArray (Pointer to private IMPLementation)
	};

/** \brief Inicializace prázdné množiny
 *  \details Inicializuje složky struktury tak, aby byla výsledkem prázdná množina.
 *  \param[in,out] aSet Ukazatel na místo v paměti určené pro inicializaci množiny
 */
void set_init(struct TSet *aSet);

/** \brief Přečtení počtu elementů množiny
 *  \details Vrací velikost (počet elementů) množiny.
 *  \param[in] aSet Ukazatel na existující množinu
 *  \return Počet elementů množiny nebo hodnota 0 v případě chyby
 */
size_t set_size(const struct TSet *aSet);

/** \brief Zjištění, zda je množina prázdná
 *  \details Funkce (predikát) vracející \c bool hodnotu reprezentující test, zda je množina prázdná.
 *  \param[in] aSet Ukazatel na existující množinu
 *  \return \c true pokud je množina prázdná
 */
static inline bool set_is_empty(const struct TSet *aSet)
	{
	return !set_size(aSet);
	}

/** \brief Vložení elementu do množiny
 *  \details Vkládá hodnotu elementu do množiny.
 *  \param[in,out] aSet Ukazatel na existující množinu
 *  \param[in] aValue Hodnota elementu vkládaná do množiny
 *  \return \c true pokud nebyla hodnota v původní množině nalezena a byla následně do množiny úspěšně vložena
 */
bool set_insert(struct TSet *aSet, TSetElement aValue);

/** \brief Odstranění elementu z množiny
 *  \details Odstraní hodnotu elementu z množiny.
 *  \param[in,out] aSet Ukazatel na existující množinu
 *  \param[in] aValue Hodnota elementu odebíraného z množiny
 *  \return \c true pokud byla hodnota v množině nalezena a následně úspěšně odstraněna
 */
bool set_erase(struct TSet *aSet, TSetElement aValue);

/** \brief Zjištění, zda množina obsahuje zadanou hodnotu elementu
 *  \details Funkce (predikát) vracející \c bool hodnotu reprezentující test, zda množina obsahuje zadanou hodnotu elementu.
 *  \param[in] aSet Ukazatel na existující množinu
 *  \param[in] aValue Hodnota elementu hledaná v množině
 *  \return \c true pokud byl element se zadanou hodnotou v množině nalezen
 */
bool set_is_element_of(const struct TSet *aSet, TSetElement aValue);

/** \brief Získání nového iterátoru ukazujícího na první element se zadanou hodnotou v množině nalezen, pokud byl nalezen
 *  \details Funkce vracející nový iterátor směřující na první nalezený element se zadanou hodnotou, pokud nebyl element v množině nalezen vrací neplatný iterátor.
 *  \param[in] aSet Ukazatel na existující množinu
 *  \param[in] aValue Hodnota elementu hledaná v množině
 *  \return Nový iterátor asociovaný s množinou \p aSet směřující na první nalezený element se zadanou hodnotou, nebo neplatný iterátor
 */
struct TSetIterator set_find(const struct TSet *aSet, TSetElement aValue);

/** \brief Deinicializace množiny
 *  \details Deinicializuje množinu, nastaví počet elementů množiny na hodnotu 0.
 *  \param[in,out] aSet Ukazatel na existující frontu
 */
void set_destroy(struct TSet *aSet);
/** \} TSet */

/** \defgroup TSetIterator 3. Iterátor množiny
 *  \brief Definice datového typu SetIterator a jeho funkcí
 *  \{
 */

/** \brief Definice typu SetIterator
 *  \details SetIterator se při vzniku naváže na zvolenou množinu a následně umožňuje číst hodnoty jednotlivých elementů pomocí definovaného API.
 */
struct TSetIterator
	{
	const struct TSet *iSet;	///< Ukazatel na navázanou množinu (iterátor neumožňuje měnit hodnoty elementů v množině)
	size_t iPos;				///< Aktuální pozice pro indexaci elementu v navázané množině
	};

/** \brief Vytvoření nového iterátoru ukazujícího na první element množiny
 *  \details Vytvoří a vrací nový iterátor, který je navázán (asociován) na zadanou množinu a ukazuje na element na první element.
 *  \param[in] aSet Ukazatel na existující množinu
 *  \return Nový iterátor asociovaný s množinou \p aSet ukazující na její první element
 */
struct TSetIterator set_iterator_begin(const struct TSet *aSet);

/** \brief Zjištění platnosti iterátoru
 *  \details Funkce (predikát) vracející \c bool hodnotu definující platnost iterátoru.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje na platné místo v asociované množině
 */
bool set_iterator_is_valid(const struct TSetIterator *aIter);

/** \brief Posunutí iterátoru vpřed
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, zajistí jeho posun vpřed (tj. na následující element v asociované množině).
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociované množině
 */
bool set_iterator_to_next(struct TSetIterator *aIter);

/** \brief Přečtení hodnoty elementu z množiny pomocí iterátoru
 *  \details Přečte hodnotu elementu množiny z pozice určené iterátorem.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return Hodnota elementu množiny z pozice, na kterou ukazuje iterátor \p aIter, nebo nulový element (pokud je iterátor neplatný).
 */
TSetElement set_iterator_value(const struct TSetIterator *aIter);
/** \} TSetIterator */

/** \defgroup IteratorAlgorithms 4. Univerzální funkce pro práci s iterátory
 *  \brief Definice funkcí používající iterátory
 *  \{
 */

/** \brief Zavolání zvolené funkce na každý element množiny od pozice určené iterátorem až do posledního elementu množiny včetně
 *  \details Zavolá zadanou funkci \p aOperation na každý element množiny v rozsahu od pozice určené iterátorem až do posledního elementu množiny včetně.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \param[in] aOperation Ukazatel na funkci vracející \c void a mající jeden parametr typu ukazatel na iterátor
 */
static inline void set_for_each(struct TSetIterator aIter, void(*aOperation)(const struct TSetIterator *aIter))
	{
	for(bool valid = set_iterator_is_valid(&aIter); valid; valid = set_iterator_to_next(&aIter))
		aOperation(&aIter);
	}

/** \brief Zavolání zvolené funkce na prvních N elementů množiny od pozice určené iterátorem
 *  \details Zavolá zadanou funkci \p aOperation na prvních \p aN elementů množiny od počáteční pozice určené iterátorem. Pokud má množina méně než \p aN elementů, funkce se ukončí dříve a vrátí neplatný iterátor.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \param[in] aN Hodnota udávající prvních N elementů množiny, na které se má zavolat funkce \p aOperation
 *  \param[in] aOperation Ukazatel na funkci vracející \c void a mající jeden parametr typu ukazatel na iterátor
 *  \return Hodnota iterátoru ukazujícího za `aN`-tý element množiny, nebo neplatný iterátor pokud množina nemá další elementy.
 */
static inline struct TSetIterator set_for_n(struct TSetIterator aIter, size_t aN, void(*aOperation)(const struct TSetIterator *aIter))
	{
	for(bool valid = set_iterator_is_valid(&aIter); aN && valid; --aN, valid = set_iterator_to_next(&aIter))
		aOperation(&aIter);
	return aIter;
	}

/** \brief Vyhledání prvního elementu množiny splňujícího zadaný predikát
 *  \details Vyhledá první element množiny splňující zadaný predikát \p aPredicate. Vyhledávání probíhá od elementu určeného iterátorem \p aIter, až do konce množiny.
 *  \param[in] aIter Ukazatel na existující iterátor, jenž je předem asociovaný se zvolenou množinou a který tak definuje počáteční element pro zvolenou operaci
 *  \param[in] aPredicate Ukazatel na predikátovou funkci (funkci vracející \c bool a mající jeden parametr typu ukazatel na iterátor)
 *  \return Hodnota iterátoru ukazujícího na první nalezený element množiny splňující zadaný predikát \p aPredicate, nebo neplatný iterátor, pokud nebyl nalezen žádný vhodný element.
 */
static inline struct TSetIterator set_find_if(struct TSetIterator aIter, bool(*aPredicate)(const struct TSetIterator *aIter))
	{
	for(bool valid = set_iterator_is_valid(&aIter); valid; valid = set_iterator_to_next(&aIter))
		if(aPredicate(&aIter))
			return aIter;
	return aIter;
	}

/** \brief Vyhledání prvního elementu množiny nesplňujícího zadaný predikát
 *  \details Vyhledá první element množiny nesplňující zadaný predikát \p aPredicate. Vyhledávání probíhá od elementu určeného iterátorem \p aIter, až do konce množiny.
 *  \param[in] aIter Ukazatel na existující iterátor, jenž je předem asociovaný se zvolenou množinou a který tak definuje počáteční element pro zvolenou operaci
 *  \param[in] aPredicate Ukazatel na predikátovou funkci (funkci vracející \c bool a mající jeden parametr typu ukazatel na iterátor)
 *  \return Hodnota iterátoru ukazujícího na první nalezený element množiny nesplňující zadaný predikát \p aPredicate, nebo neplatný iterátor, pokud nebyl nalezen žádný vhodný element.
 */
static inline struct TSetIterator set_find_if_not(struct TSetIterator aIter, bool(*aPredicate)(const struct TSetIterator *aIter))
	{
	for(bool valid = set_iterator_is_valid(&aIter); valid; valid = set_iterator_to_next(&aIter))
		if(!aPredicate(&aIter))
			return aIter;
	return aIter;
	}

/** \} IteratorAlgorithms */

#endif /* TSET_H */
