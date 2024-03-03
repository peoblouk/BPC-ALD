#ifndef TSTACK_H
#define TSTACK_H
/** \file TStack.h
 *  \brief Definice typu zásobník (realizace pomocí pole)
 *  \author Petyovský
 *  \version 2023
 *  $Id: TStack.h 2126 2023-02-10 15:54:58Z petyovsky $
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** \defgroup TStack 1. Stack
 *  \brief Definice datového typu Stack a jeho funkcí (realizace zásobníku pomocí pole)
 *  \{
 */

typedef int TStackElement;					///< Definice typu StackElement (datový typ elementů zásobníku)

//#define STACK_MAXCOUNT 1024					///< Maximální počet elementů zásobníku

/** \brief Definice typu Stack
 *  \details Typ Stack obsahuje pole prvků typu StackElement a umožňuje s nimi pracovat pomocí definovaného API.
 */
struct TStack
	{
	size_t iCount;///< Aktuální počet elementů zásobníku
	size_t iCapacity; 
	TStackElement *iValues;
};

/** \brief Inicializace prázdného zásobníku
 *  \details Inicializuje složky struktury tak, aby byl výsledkem prázdný zásobník.
 *  \param[in,out] aStack Ukazatel na místo v paměti určené pro inicializaci zásobníku
 */
void stack_init(struct TStack *aStack);

/** \brief Zjištění, zda je zásobník prázdný
 *  \details Funkce (predikát) vracející \c bool hodnotu reprezentující test, zda je zásobník prázdný.
 *  \param[in] aStack Ukazatel na existující zásobník
 *  \return \c true pokud je zásobník prázdný
 */
bool stack_is_empty(const struct TStack *aStack);

/** \brief Získání hodnoty elementu na vrcholu zásobníku
 *  \details Přečte hodnotu elementu na vrcholu zásobníku.
 *  \param[in] aStack Ukazatel na existující zásobník
 *  \param[in,out] aValue Ukazatel na místo v paměti určené pro načtení hodnoty elementu z vrcholu zásobníku
 *  \return \c true pokud byla hodnota elementu z vrcholu zásobníku úspěšně načtena
 */
bool /* TStackIterator */ stack_top(const struct TStack *aStack, TStackElement *aValue);

/** \brief Vložení elementu na vrchol zásobníku
 *  \details Vkládá hodnotu elementu na vrchol zásobníku.
 *  \param[in,out] aStack Ukazatel na existující zásobník určený pro vložení elementu
 *  \param[in] aValue Hodnota elementu vkládaná do zásobníku
 *  \return \c true pokud byla hodnota do zásobníku úspěšně vložena
 */
bool stack_push(struct TStack *aStack, TStackElement aValue);

/** \brief Odstranění elementu na vrcholu zásobníku
 *  \details Odstraní hodnotu elementu na vrcholu zásobníku.
 *  \param[in,out] aStack Ukazatel na existující zásobník určený pro odstranění elementu
 *  \return \c true pokud byla hodnota ze zásobníku úspěšně odstraněna
 */
bool stack_pop(struct TStack *aStack);

/** \brief Deinicializace zásobníku
 *  \details Deinicializuje zásobník, nastaví počet elementů zásobníku na hodnotu 0.
 *  \param[in,out] aStack Ukazatel na existující zásobník
 */
void stack_destroy(struct TStack *aStack);
/** \} TStack */

/** \defgroup TStackIterator 3. Iterátor zásobníku
 *  \brief Definice datového typu StackIterator a jeho funkcí
 *  \{
 */

/** \brief Definice typu StackIterator
 *  \details StackIterator se při vzniku naváže na zvolený zásobník a následně umožňuje přistupovat k jednotlivým elementům pomocí definovaného API.
 */
struct TStackIterator
	{
	const struct TStack *iStack;	///< Ukazatel na navázaný zásobník (immutable iterátor - neumožňuje měnit elementy StackElement)
	size_t iPos;					///< Aktuální pozice pro indexaci elementu v navázaném zásobníku
	};

/** \brief Vytvoření nového iterátoru ukazujícího na vrchol zásobníku
 *  \details Vytvoří a vrací nový iterátor, který je navázán (asociován) na zadaný zásobník a ukazuje na element na jeho vrcholu.
 *  \param[in] aStack Ukazatel na existující zásobník
 *  \return Nový iterátor asociovaný se zásobníkem \p aStack ukazující na element na jeho vrcholu
 */
struct TStackIterator stack_iterator_begin(const struct TStack *aStack);

/** \brief Zjištění platnosti iterátoru
 *  \details Funkce (predikát) vracející \c bool hodnotu definující platnost iterátoru.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje na platné místo v asociovaném zásobníku
 */
bool stack_iterator_is_valid(const struct TStackIterator *aIter);

/** \brief Posunutí iterátoru vpřed
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, zajistí jeho posun vpřed (tj. na následující element v asociovaném zásobníku).
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociovaném zásobníku
 */
bool stack_iterator_to_next(struct TStackIterator *aIter);

/** \brief Přečtení hodnoty elementu ze zásobníku pomocí iterátoru
 *  \details Přečte hodnotu elementu zásobníku z pozice určené iterátorem.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return Hodnota elementu zásobníku z pozice, na kterou ukazuje iterátor \p aIter, nebo nulový element (pokud je iterátor neplatný).
 */
TStackElement stack_iterator_value(const struct TStackIterator *aIter);

/** \} TStackIterator */

#endif /* TSTACK_H */
