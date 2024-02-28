#ifndef TQUEUE_H
#define TQUEUE_H
/** \file TQueue.h
 *  \brief Definice typu fronta (realizace pomocí lineárního jednosměrně vázaného seznamu)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TQueue.h 2616 2024-02-16 15:36:28Z petyovsky $
 */

#include <stdbool.h>
#include <stdlib.h>
#include "check.h"

/** \defgroup TQueue 1. Fronta
 *  \brief Definice datového typu Queue a jeho funkcí (realizace fronty pomocí lineárního jednosměrně vázaného seznamu)
 *  \{
 */

typedef int TQueueElement;					///< Definice typu QueueElement (datový typ elementů fronty)

/** \brief Definice typu Queue
 *  \details Typ Queue obsahuje ukazatele na dynamicky alokované proměnné typu QueueNode, které představují uzly s hodnotami elementů uspořádanými do lineárního jednosměrně vázaného seznamu. Fronta umožňuje pracovat se svými elementy pomocí definovaného API.
 *  \invariant Obecně platný a testovatelný předpoklad: `(iFront == NULL && iBack == NULL)` nebo `(iFront != NULL && iBack != NULL)`.
 */
struct TQueue
	{
	struct TQueueNode *iFront;				///< PIMPL ukazatel na první uzel lineárního jednosměrně vázaného seznamu realizujícího frontu
	struct TQueueNode *iBack;				///< PIMPL ukazatel na poslední uzel lineárního jednosměrně vázaného seznamu realizujícího frontu
	};

/** \brief Inicializace prázdné fronty
 *  \details Inicializuje složky struktury tak, aby byla výsledkem prázdná fronta.
 *  \param[in,out] aQueue Ukazatel na místo v paměti určené pro inicializaci fronty
 */
void queue_init(struct TQueue *aQueue);

/** \brief Zjištění, zda je fronta prázdná
 *  \details Funkce (predikát) vracející \c bool hodnotu reprezentující test, zda je fronta prázdná.
 *  \param[in] aQueue Ukazatel na existující frontu
 *  \return \c true pokud je fronta prázdná
 *  \attention Funkce ověřuje konzistenci obou ukazatelů fronty a pomocí `assert` hlásí (při překladu v režimu `Debug`) případnou běhovou chybu!
 */
bool queue_is_empty(const struct TQueue *aQueue);

/** \brief Získání hodnoty elementu z čela fronty
 *  \details Přečte hodnotu elementu z čela fronty.
 *  \param[in] aQueue Ukazatel na existující frontu
 *  \param[in,out] aValue Ukazatel na místo v paměti určené pro načtení hodnoty elementu z čela fronty
 *  \return \c true pokud byla hodnota elementu z čela fronty úspěšně načtena
 */
bool /* TQueueIterator */ queue_front(const struct TQueue *aQueue, TQueueElement *aValue);

/** \brief Získání hodnoty elementu z konce fronty
 *  \details Přečte hodnotu elementu z konce fronty.
 *  \param[in] aQueue Ukazatel na existující frontu
 *  \param[in,out] aValue Ukazatel na místo v paměti určené pro načtení hodnoty elementu z konce fronty
 *  \return \c true pokud byla hodnota elementu z konce fronty úspěšně načtena
 */
bool /* TQueueIterator */ queue_back(const struct TQueue *aQueue, TQueueElement *aValue);

/** \brief Vložení elementu na konec fronty
 *  \details Vkládá hodnotu elementu na konec fronty.
 *  \param[in,out] aQueue Ukazatel na existující frontu určenou pro vložení elementu
 *  \param[in] aValue Hodnota elementu vkládaná do fronty
 *  \return \c true pokud byla hodnota do fronty úspěšně vložena
 */
bool queue_push(struct TQueue *aQueue, TQueueElement aValue);

/** \brief Odstranění elementu z čela fronty
 *  \details Odstraní hodnotu elementu z čela fronty.
 *  \param[in,out] aQueue Ukazatel na existující frontu určenou pro odstranění elementu
 *  \return \c true pokud byla hodnota z fronty úspěšně odstraněna
 */
bool queue_pop(struct TQueue *aQueue);

/** \brief Deinicializace fronty
 *  \details Deinicializuje frontu, nastaví počet elementů fronty na hodnotu 0.
 *  \param[in,out] aQueue Ukazatel na existující frontu
 */
void queue_destroy(struct TQueue *aQueue);
/** \} TQueue */

/** \defgroup TQueueIterator 2. Iterátor fronty
 *  \brief Definice datového typu QueueIterator a jeho funkcí
 *  \{
 */

/** \brief Definice typu QueueIterator
 *  \details QueueIterator se při vzniku naváže na zvolenou frontu a následně umožňuje přistupovat k jednotlivým elementům pomocí definovaného API.
 */
struct TQueueIterator
	{
	const struct TQueue *iQueue;	///< Ukazatel na navázanou frontu (mutable iterátor - umožňuje měnit elementy QueueElement)
	struct TQueueNode *iActual;		///< Ukazatel na aktuální uzel v lineárním jednosměrně vázaného seznamu asociované fronty
	};

/** \brief Vytvoření nového iterátoru ukazujícího na čelo fronty
 *  \details Vytvoří a vrací nový iterátor, který je navázán (asociován) na zadanou frontu a ukazuje na element na jejím čele.
 *  \param[in] aQueue Ukazatel na existující frontu
 *  \return Nový iterátor asociovaný s frontou \p aQueue ukazující na element na jejím čele
 */
struct TQueueIterator queue_iterator_begin(const struct TQueue *aQueue);

/** \brief Zjištění platnosti iterátoru
 *  \details Funkce (predikát) vracející \c bool hodnotu definující platnost iterátoru.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje na platné místo v asociované frontě
 */
bool queue_iterator_is_valid(const struct TQueueIterator *aIter);

/** \brief Posunutí iterátoru vpřed
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, zajistí jeho posun vpřed (tj. na následující element v asociované frontě).
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociované frontě
 */
bool queue_iterator_to_next(struct TQueueIterator *aIter);

/** \brief Přečtení hodnoty elementu z fronty pomocí iterátoru
 *  \details Přečte hodnotu elementu fronty z pozice určené iterátorem.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return Hodnota elementu fronty z pozice, na kterou ukazuje iterátor \p aIter, nebo nulový element (pokud je iterátor neplatný).
 */
TQueueElement queue_iterator_value(const struct TQueueIterator *aIter);

/** \brief Zapsání hodnoty elementu do fronty pomocí iterátoru
 *  \details Zapíše hodnotu elementu do fronty na pozice určenou iterátorem. Původní hodnota elementu fronty je přepsána novou hodnotou \p aValue.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \param[in] aValue Hodnota elementu zapisovaná do fronty na pozici určenou iterátorem
 *  \return \c true pokud je iterátor \p aIter platný a hodnota \p aValue byla do fronty úspěšně zapsána
 */
bool queue_iterator_set_value(const struct TQueueIterator *aIter, TQueueElement aValue);
/** \} TQueueIterator */

/** \defgroup IteratorAlgorithms 4. Univerzální funkce pro práci s iterátory
 *  \brief Definice funkcí používající iterátory
 *  \{
 */

/** \brief Zavolání zvolené funkce na každý element fronty od pozice určené iterátorem až do konce fronty.
 *  \details Zavolá zadanou funkci \p aOperation na každý element fronty v rozsahu od pozice určené iterátorem až do konce fronty.
 *  \param[in] aIter Ukazatel na existující iterátor, jenž je předem asociovaný se zvolenou frontou a který tak definuje počáteční element pro zvolenou operaci
 *  \param[in] aOperation Ukazatel na funkci vracející \c void a mající jeden parametr typu ukazatel na iterátor
 */
static inline void queue_for_each(struct TQueueIterator aIter, void(*aOperation)(const struct TQueueIterator *aIter))
	{
	for(bool valid = queue_iterator_is_valid(&aIter); valid; valid = queue_iterator_to_next(&aIter))
		aOperation(&aIter);
	}

/** \brief Vyhledání prvního elementu fronty splňujícího zadaný predikát
 *  \details Vyhledá první element fronty splňující zadaný predikát \p aPredicate. Vyhledávání probíhá od elementu určeného iterátorem \p aIter, až do konce fronty.
 *  \param[in] aIter Ukazatel na existující iterátor, jenž je předem asociovaný se zvolenou frontou a který tak definuje počáteční element pro zvolenou operaci
 *  \param[in] aPredicate Ukazatel na predikátovou funkci (funkci vracející \c bool a mající jeden parametr typu ukazatel na iterátor)
 *  \return Hodnota iterátoru ukazujícího na první nalezený element fronty splňující zadaný predikát \p aPredicate, nebo neplatný iterátor, pokud nebyl nalezen žádný vhodný element.
 */
static inline struct TQueueIterator queue_find_if(struct TQueueIterator aIter, bool(*aPredicate)(const struct TQueueIterator *aIter))
	{
	for(bool valid = queue_iterator_is_valid(&aIter); valid; valid = queue_iterator_to_next(&aIter))
		if(aPredicate(&aIter))
			return aIter;
	return aIter;
	}

/** \brief Vyhledání prvního elementu fronty nesplňujícího zadaný predikát
 *  \details Vyhledá první element fronty nesplňující zadaný predikát \p aPredicate. Vyhledávání probíhá od elementu určeného iterátorem \p aIter, až do konce fronty.
 *  \param[in] aIter Ukazatel na existující iterátor, jenž je předem asociovaný se zvolenou frontou a který tak definuje počáteční element pro zvolenou operaci
 *  \param[in] aPredicate Ukazatel na predikátovou funkci (funkci vracející \c bool a mající jeden parametr typu ukazatel na iterátor)
 *  \return Hodnota iterátoru ukazujícího na první nalezený element fronty nesplňující zadaný predikát \p aPredicate, nebo neplatný iterátor, pokud nebyl nalezen žádný vhodný element.
 */
static inline struct TQueueIterator queue_find_if_not(struct TQueueIterator aIter, bool(*aPredicate)(const struct TQueueIterator *aIter))
	{
	for(bool valid = queue_iterator_is_valid(&aIter); valid; valid = queue_iterator_to_next(&aIter))
		if(!aPredicate(&aIter))
			return aIter;
	return aIter;
	}

/** \} IteratorAlgorithms */

#endif /* TQUEUE_H */
