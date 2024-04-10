#ifndef TSPAN_H
#define TSPAN_H
/** \file TSpan.h
 *  \brief Definice typu Span zajišťující přístup k omezené části vektoru (vyžaduje Sortable Vector with shareable sortstats)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TSpan.h 2670 2024-03-25 14:21:39Z petyovsky $
 */

#include "TVector.h"

/** \defgroup TSpan 2.2.1. Datový typ Span pro přístup k omezené části vektoru
 *  \brief Definice typu Span, který umožňuje přístup k omezené části vektoru (vyžaduje Sortable Vector s vlastností sdílení statistik třídění)
 *  \ingroup sortable_TVector_with_shareable_sortstats
 *  \{
 */
typedef TVectorElement TSpanElement;		///< Definice typu SpanElement jako alias na typ VectorElement

/** \brief Definice typu Span
 *  \details Typ Span zajišťuje přístup k omezené části již existujícího vektoru a umožňuje pracovat s jeho elementy pomocí definovaného API.
 *  \invariant Obecně platný a testovatelný předpoklad: shodný jako zapouzdřený ADT Sortable Vector with shareable sortstats.
 */
struct TSpan
	{
	struct TVector iVector;					///< Reprezentuje určenou část (rozsah) existujícího vektoru
	};

/** \brief Definice typu SpanIterator
 *  \details SpanIterator se při vzniku naváže na zvolený Span a následně umožňuje přistupovat k jednotlivým prvkům pomocí definovaného API.
 */
struct TSpanIterator
	{
	struct TVectorIterator iVectorIter;		///< Reprezentuje iterátor na určenou část (rozsah) existujícího vektoru
	};

/** \brief Vytvoření Spanu pro celý vektor
 *  \details Vytvoří novou hodnotu typu Span s rozsahem pro přístup k elementům celého vektoru.
 *  \param[in] aVector Existující vektor
 *  \return Nový Span umožňující přístup ke všem elementům vektoru \p aVector
 */
static inline struct TSpan span_create(struct TVector aVector)
	{
	return (struct TSpan) { .iVector = aVector };
	}

/** \brief Vytvoření Spanu pro část vektoru od počátku a definované délce
 *  \details Vytvoří novou hodnotu typu Span s rozsahem pro přístup k elementům vektoru od počátku až do definované délky.
 *  \param[in] aVector Existující vektor
 *  \param[in] aSize Délka rozsahu od počátku existujícího vektoru (Délka = poslední pozice + 1)
 *  \return Nový Span umožňující přístup k elementům vektoru \p aVector od počátku až do definované délky
 */
static inline struct TSpan span_create_left(struct TVector aVector, size_t aSize)
	{
	if(aVector.iSize < aSize)
		{
		aVector.iValues = NULL;
		aVector.iSize = 0;
		}
	else
		aVector.iSize = aSize;

	return (struct TSpan) { .iVector = aVector };
	}

/** \brief Vytvoření Spanu pro část vektoru od definované pozice až do konce vektoru
 *  \details Vytvoří novou hodnotu typu Span s rozsahem pro přístup k elementům vektoru od pozice \p aFromPos až do konce vektoru.
 *  \param[in] aVector Existující vektor
 *  \param[in] aFromPos Pozice elementu v existujícím vektoru, která se stane prvním elementem Spanu
 *  \return Nový Span umožňující přístup k elementům vektoru \p aVector od definované pozice až do konce vektoru
 */
static inline struct TSpan span_create_right(struct TVector aVector, size_t aFromPos)
	{
	if(aVector.iSize <= aFromPos)
		{
		aVector.iValues = NULL;
		aVector.iSize = 0;
		}
	else
		{
		aVector.iValues += aFromPos;
		aVector.iSize -= aFromPos;
		}

	return (struct TSpan) { .iVector = aVector };
	}

/** \brief Vytvoření Spanu pro část vektoru definovanou pomocí pozičního intervalu <od - do>
 *  \details Vytvoří novou hodnotu typu Span s rozsahem pro přístup k elementům původního vektoru od pozice \p aFromPos do pozice \p aToPos vektoru.
 *  \param[in] aVector Existující vektor
 *  \param[in] aFromPos Pozice elementu v existujícím vektoru, která se stane prvním elementem Spanu
 *  \param[in] aToPos Pozice elementu v existujícím vektoru, která se stane posledním elementem Spanu
 *  \return Nový Span umožňující přístup k elementům vektoru \p aVector v definovaném rozsahu pozic vektoru
 */
static inline struct TSpan span_create_range(struct TVector aVector, size_t aFromPos, size_t aToPos)
	{
	if((aVector.iSize <= aFromPos) || (aVector.iSize <= aToPos) || (aFromPos > aToPos))
		{
		aVector.iValues = NULL;
		aVector.iSize = 0;
		}
	else
		{
		aVector.iValues += aFromPos;
		aVector.iSize = aToPos + 1;
		}

	return (struct TSpan) { .iVector = aVector };
	}

/** \brief Vytvoření SubSpanu jako části od počátku a definované délce původního Spanu
 *  \details Vytvoří novou hodnotu typu Span s rozsahem pro přístup k elementům původního Spanu od počátku až do definované délky.
 *  \param[in] aSpan Existující původní Span
 *  \param[in] aSize Délka rozsahu od počátku existujícího původního Spanu (Délka = poslední pozice + 1)
 *  \return Nový SubSpan umožňující přístup k elementům Spanu \p aSpan od počátku až do definované délky
 */
static inline struct TSpan span_create_left_subspan(struct TSpan aSpan, size_t aSize)
	{
	if(aSpan.iVector.iSize < aSize)
		{
		aSpan.iVector.iValues = NULL;
		aSpan.iVector.iSize = 0;
		}
	else
		aSpan.iVector.iSize = aSize;

	return aSpan;
	}

/** \brief Vytvoření SubSpanu jako části od definované pozice až do konce původního Spanu
 *  \details Vytvoří novou hodnotu typu Span s rozsahem pro přístup k elementům původního Spanu od pozice \p aFromPos až do konce původního Spanu.
 *  \param[in] aSpan Existující původní Span
 *  \param[in] aFromPos Pozice elementu v existujícím původním Spanu, která se stane prvním elementem SubSpanu
 *  \return Nový SubSpan umožňující přístup k elementům Spanu \p aSpan od definované pozice až do konce Spanu \p aSpan
 */
static inline struct TSpan span_create_right_subspan(struct TSpan aSpan, size_t aFromPos)
	{
	if(aSpan.iVector.iSize <= aFromPos)
		{
		aSpan.iVector.iValues = NULL;
		aSpan.iVector.iSize = 0;
		}
	else
		{
		aSpan.iVector.iValues += aFromPos;
		aSpan.iVector.iSize -= aFromPos;
		}

	return aSpan;
	}

/** \brief Vytvoření SubSpanu pro část původního Spanu definovanou pomocí pozičního intervalu <od - do>
 *  \details Vytvoří novou hodnotu typu Span s rozsahem pro přístup k elementům původního Spanu od pozice \p aFromPos do pozice \p aToPos původního Spanu.
 *  \param[in] aSpan Existující původní Span
 *  \param[in] aFromPos Pozice elementu v původním Spanu, která se stane prvním elementem SubSpanu
 *  \param[in] aToPos Pozice elementu v původním Spanu, která se stane posledním elementem SubSpanu
 *  \return Nový SubSpan umožňující přístup k elementům Spanu \p aSpan v definovaném rozsahu pozic
 */
static inline struct TSpan span_create_range_subspan(struct TSpan aSpan, size_t aFromPos, size_t aToPos)
	{
	if((aSpan.iVector.iSize <= aFromPos) || (aSpan.iVector.iSize <= aToPos) || aFromPos > aToPos)
		{
		aSpan.iVector.iValues = NULL;
		aSpan.iVector.iSize = 0;
		}
	else
		{
		aSpan.iVector.iValues += aFromPos;
		aSpan.iVector.iSize = aToPos + 1;
		}

	return aSpan;
	}

/** \brief Test platnosti Spanu
 *  \details Testuje, zda je Span asociován s vektorem a nemá nulovou velikost.
 *  \param[in] aSpan Ukazatel na existující Span
 *  \return \c true pokud je Span asociován s vektorem a nemá nulovou velikost
 */
static inline bool span_is_valid(const struct TSpan *aSpan)
	{
	return vector_size(&aSpan->iVector) > 0;
	}

/** \brief Přečtení počtu elementů Spanu
 *  \details Vrací velikost (počet elementů) Spanu.
 *  \param[in] aSpan Ukazatel na existující Span
 *  \return Počet elementů vektoru nebo hodnota 0 v případě chyby
 */
static inline size_t span_size(const struct TSpan *aSpan)
	{
	return aSpan->iVector.iSize;
	}

/** \brief Přečtení hodnoty elementu Spanu na požadované pozici
 *  \details Vrací hodnotu elementu z požadované pozice (indexu) Spanu.
 *  \param[in] aSpan Ukazatel na existující Span
 *  \param[in] aPos Požadovaná pozice (index) elementu
 *  \return Hodnota elementu uložená ve Spanu \p aSpan na pozici \p aPos
 *  \attention Funkce ověřuje platnost ukazatele a rozsah parametru \p aPos \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline TVectorElement span_value(const struct TSpan *aSpan, size_t aPos)
	{
	return vector_value(&aSpan->iVector, aPos);
	}

/** \brief Zapsání hodnoty elementu do Spanu na požadovanou pozici
 *  \details Zápis nové hodnoty elementu do Spanu na požadovanou pozici (index).
 *  \param[in,out] aSpan Ukazatel na existující Span
 *  \param[in] aPos Požadovaná pozice (index) elementu
 *  \param[in] aValue Nová hodnota elementu
 *  \attention Funkce ověřuje platnost ukazatele a rozsah parametru \p aPos \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void span_set_value(struct TSpan *aSpan, size_t aPos, TVectorElement aValue)
	{
	vector_set_value(&aSpan->iVector, aPos, aValue);
	}

/** \brief Přesun hodnoty elementu Spanu
 *  \details Přesun hodnoty elementu Spanu z/do libovolné pozice nebo případně i mezi různými Spany. Parametry \p aToSpan a \p aFromSpan mohou být stejné.
 *  \param[in,out] aToSpan Ukazatel na cílový Span
 *  \param[in] aToPos Cílová pozice (index) elementu ve Spanu \p aToSpan
 *  \param[in] aFromSpan Ukazatel na zdrojový Span
 *  \param[in] aFromPos Zdrojová pozice (index) elementu ve Spanu \p aFromSpan
 *  \attention Funkce ověřuje platnost ukazatelů a rozsah indexů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void span_move_positions(struct TSpan *aToSpan, size_t aToPos, const struct TSpan *aFromSpan, size_t aFromPos)
	{
	vector_move_positions(&aToSpan->iVector, aToPos, &aFromSpan->iVector, aFromPos);
	}

/** \brief Porovnání hodnot elementů Spanů
 *  \details Trojcestně porovná hodnoty elementů mezi zvolenými pozicemi nebo případně i mezi různými Spany. Parametry \p aLeftSpan a \p aRightSpan mohou být stejné.
 *  \param[in] aLeftSpan Ukazatel na levý Span při porovnání
 *  \param[in] aLeftPos Pozice (index) (LHS) elementu ve Spanu \p aLeftSpan
 *  \param[in] aRightSpan Ukazatel na pravý Span při porovnání
 *  \param[in] aRightPos Pozice (index) (RHS) elementu ve Spanu \p aRightSpan
 *  \retval -1 Pokud (LHS < RHS)
 *  \retval  0 Pokud (LHS = RHS)
 *  \retval +1 Pokud (LHS > RHS)
 *  \attention Funkce ověřuje platnost ukazatelů a rozsah indexů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline int span_compare_positions(const struct TSpan *aLeftSpan, size_t aLeftPos, const struct TSpan *aRightSpan, size_t aRightPos)
	{
	return vector_compare_positions(&aLeftSpan->iVector, aLeftPos, &aRightSpan->iVector, aRightPos);
	}

/** \brief Porovnání mezi hodnotou elementu Span a hodnotou typu VectorElement
 *  \details Trojcestně porovná hodnotu elementu Spanu na zvolené pozici se zadanou hodnotou \p aValue.
 *  \param[in] aSpan Ukazatel na Span při porovnání
 *  \param[in] aPos Pozice (index) (LHS) elementu ve Spanu \p aSpan
 *  \param[in] aValue Hodnota stojící při porovnání na pravé straně (RHS)
 *  \retval -1 Pokud (LHS < RHS)
 *  \retval  0 Pokud (LHS = RHS)
 *  \retval +1 Pokud (LHS > RHS)
 *  \attention Funkce ověřuje platnost ukazatele a rozsah indexu vektoru \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline int span_compare_position_value(const struct TSpan *aSpan, size_t aPos, TVectorElement aValue)
	{
	return vector_compare_position_value(&aSpan->iVector, aPos, aValue);
	}

/** \brief Výměna hodnot elementů Spanů
 *  \details Vzájemně zamění hodnoty elementů mezi zvolenými pozicemi nebo případně i mezi různými Spany. Parametry \p aLeftSpan a \p aRightSpan mohou být stejné.
 *  \param[in,out] aLeftSpan Ukazatel na levý Span pro záměnu hodnoty elementu
 *  \param[in] aLeftPos Pozice (index) (LHS) elementu ve Spanu \p aLeftSpan
 *  \param[in,out] aRightSpan Ukazatel na pravý Span při záměně hodnoty elementu
 *  \param[in] aRightPos Pozice (index) (RHS) elementu ve Span \p aRightSpan
 *  \attention Funkce ověřuje platnost ukazatelů a rozsah indexů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void span_swap_positions(struct TSpan *aLeftSpan, size_t aLeftPos, struct TSpan *aRightSpan, size_t aRightPos)
	{
	vector_swap_positions(&aLeftSpan->iVector, aLeftPos, &aRightSpan->iVector, aRightPos);
	}

/** \brief Výměna mezi hodnotou elementu Spanu a proměnnou typu VectorElement
 *  \details Vzájemně zamění hodnotu elementu Spanu na zvolené pozici se zadanou hodnotou v proměnné typu VectorElement.
 *  \param[in,out] aSpan Ukazatel na Span pro záměnu hodnoty elementu
 *  \param[in] aPos Pozice (index) elementu ve Spanu \p aSpan
 *  \param[in,out] aValue Ukazatel na hodnotu proměnné typu VectorElement pro záměnu
 *  \attention Funkce ověřuje platnost obou ukazatelů a rozsah indexu vektoru \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void span_swap_position_value(struct TSpan *aSpan, size_t aPos, TVectorElement *aValue)
	{
	vector_swap_position_value(&aSpan->iVector, aPos, aValue);
	}

/** \brief Vytvoření nového iterátoru ukazujícího na počáteční element
 *  \details Vytvoří a vrací nový iterátor, který je navázán (asociován) na zadaný Span a ukazuje na jeho počáteční element.
 *  \param[in] aSpan Ukazatel na existující Span
 *  \return Nový iterátor asociovaný na \p aSpan ukazující na jeho počáteční element
 */
static inline struct TSpanIterator span_iterator_begin(const struct TSpan *aSpan)
	{
	return (struct TSpanIterator) { vector_iterator_begin(&aSpan->iVector) };
	}

/** \brief Vytvoření nového iterátoru ukazujícího na zadanou pozici
 *  \details Vytvoří a vrací nový iterátor, který je navázán (asociován) na zadaný Span a ukazuje na element na zadané pozici.
 *  \param[in] aSpan Ukazatel na existující Span
 *  \param[in] aPos Zadaná pozice
 *  \return Nový iterátor asociovaný na \p aSpan ukazující na element na zadané pozici \p aPos
 */
static inline struct TSpanIterator span_iterator_pos(const struct TSpan *aSpan, size_t aPos)
	{
	return (struct TSpanIterator) { vector_iterator_pos(&aSpan->iVector, aPos) };
	}

/** \brief Vytvoření nového iterátoru ukazujícího na poslední element
 *  \details Vytvoří a vrací nový iterátor, který je navázán (asociován) na zadaný Span a ukazuje na jeho poslední element.
 *  \param[in] aSpan Ukazatel na existující Span
 *  \return Nový iterátor asociovaný na \p aSpan ukazující na jeho poslední element
 */
static inline struct TSpanIterator span_iterator_last(const struct TSpan *aSpan)
	{
	return (struct TSpanIterator) { vector_iterator_last(&aSpan->iVector) };
	}

/** \brief Zjištění platnosti iterátoru
 *  \details Funkce (predikát) vracející \c bool hodnotu definující platnost iterátoru.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje na platné místo v asociovaném Spanu
 */
static inline bool span_iterator_is_valid(const struct TSpanIterator *aIter)
	{
	return vector_iterator_is_valid(&aIter->iVectorIter);
	}

/** \brief Posunutí iterátoru vpřed
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, zajistí jeho posun vpřed (tj. na následující element v asociovaném Spanu).
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociovaném Spanu
 */
static inline bool span_iterator_to_next(struct TSpanIterator *aIter)
	{
	return vector_iterator_to_next(&aIter->iVectorIter);
	}

/** \brief Posunutí iterátoru vzad
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, zajistí jeho posun vzad (tj. na předchozí element v asociovaném Spanu).
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociovaném Spanu
 */
static inline bool span_iterator_to_prev(struct TSpanIterator *aIter)
	{
	return vector_iterator_to_prev(&aIter->iVectorIter);
	}

/** \brief Přečtení hodnoty elementu z Spanu pomocí iterátoru
 *  \details Vrací hodnotu elementu Span z pozice (indexu) určeného iterátorem.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return Hodnota elementu z pozice, na kterou ukazuje iterátor \p aIter, nebo nulový element (pokud je iterátor neplatný).
 */
static inline TSpanElement span_iterator_value(const struct TSpanIterator *aIter)
	{
	return (TSpanElement)vector_iterator_value(&aIter->iVectorIter);
	}

/** \brief Zapsání hodnoty elementu do Spanu pomocí iterátoru
 *  \details Zápis nové hodnoty elementu do Spanu na pozici určenou iterátorem.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \param[in] aValue Nová hodnota elementu
 *  \return \c true pokud je iterátor platný a zápis mohl proběhnout na platné místo v asociovaném Spanu
 */
static inline bool span_iterator_set_value(const struct TSpanIterator *aIter, TSpanElement aValue)
	{
	return vector_iterator_set_value(&aIter->iVectorIter, (TVectorElement) aValue);
	}

/** \brief Přesun hodnoty elementu Spanu určené dvěma iterátory
 *  \details Přesun hodnoty elementu Span z/do pozice určené iterátory.
 *  \param[in] aToIter Iterátor na cílový element Spanu
 *  \param[in] aFromIter Iterátor na zdrojový element Spanu
 *  \attention Funkce ověřuje platnost obou iterátorů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void span_move_with_iterators(const struct TSpanIterator *aToIter, const struct TSpanIterator *aFromIter)
	{
	vector_move_with_iterators(&aToIter->iVectorIter, &aFromIter->iVectorIter);
	}

/** \brief Porovnání hodnot elementů Spanů určených dvěma iterátory
 *  \details Trojcestně porovná hodnoty elementů určených dvěma iterátory.
 *  \param[in] aLeftIter Iterátor určující levý element při porovnání (LHS)
 *  \param[in] aRightIter Iterátor určující pravý element při porovnání (RHS)
 *  \retval -1 Pokud (LHS < RHS)
 *  \retval  0 Pokud (LHS = RHS)
 *  \retval +1 Pokud (LHS > RHS)
 *  \attention Funkce ověřuje platnost obou iterátorů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline int span_compare_with_iterators(const struct TSpanIterator *aLeftIter, const struct TSpanIterator *aRightIter)
	{
	return vector_compare_with_iterators(&aLeftIter->iVectorIter, &aRightIter->iVectorIter);
	}

/** \brief Porovnání mezi hodnotou elementu Spanu určenou iterátorem a hodnotou typu VectorElement
 *  \details Trojcestně porovná hodnotu elementu Spanu na pozici iterátorem se zadanou hodnotou \p aValue.
 *  \param[in] aIter Iterátor určující levý element při porovnání (LHS)
 *  \param[in] aValue Hodnota stojící při porovnání na pravé straně (RHS)
 *  \retval -1 Pokud (LHS < RHS)
 *  \retval  0 Pokud (LHS = RHS)
 *  \retval +1 Pokud (LHS > RHS)
 *  \attention Funkce ověřuje platnost iterátoru \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline int span_compare_with_iterator_value(const struct TSpanIterator *aIter, TSpanElement aValue)
	{
	return vector_compare_with_iterator_value(&aIter->iVectorIter, (TVectorElement) aValue);
	}

/** \brief Výměna hodnot elementů Spanů určených dvěma iterátory
 *  \details Vzájemně zamění hodnoty elementů určených dvěma iterátory.
 *  \param[in] aLeftIter Iterátor určující levý element Spanu
 *  \param[in] aRightIter Iterátor určující pravý element Spanu
 *  \attention Funkce ověřuje platnost obou iterátorů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void span_swap_with_iterators(const struct TSpanIterator *aLeftIter, const struct TSpanIterator *aRightIter)
	{
	vector_swap_with_iterators(&aLeftIter->iVectorIter, &aRightIter->iVectorIter);
	}

/** \brief Výměna mezi hodnotou elementu Spanu určenou iterátorem a proměnnou typu SpanElement
 *  \details Vzájemně zamění hodnotu elementu Spanu určenou iterátorem se zadanou hodnotou v proměnné typu VectorElement.
 *  \param[in] aIter Iterátor určující element Spanu pro záměnu
 *  \param[in] aValue Ukazatel na hodnotu proměnné typu SpanElement pro záměnu
 *  \attention Funkce ověřuje platnost iterátoru a ukazatele \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void span_swap_with_iterator_value(const struct TSpanIterator *aIter, TSpanElement *aValue)
	{
	vector_swap_with_iterator_value(&aIter->iVectorIter, (TVectorElement *) aValue);
	}

/** \brief Test shodnosti dvou iterátorů
 *  \details Vzájemně porovná hodnotu dvou iterátorů.
 *  \param[in] aLeftIter Levý iterátor
 *  \param[in] aRightIter Pravý iterátor
 *  \return \c true pokud iterátory odkazují na element na stejné pozici a ve stejném Spanu případně jsou oba neplatné
 */
static inline bool span_iterators_are_same(const struct TSpanIterator *aLeftIter, const struct TSpanIterator *aRightIter)
	{
	return vector_iterators_are_same(&aLeftIter->iVectorIter, &aRightIter->iVectorIter);
	}

/** \brief Test, zda iterátor vektoru odkazuje do rozsahu určeného Spanem.
 *  \details Ověří, zda iterátor odkazuje do intervalu pozic definovaných Spanem.
 *  \param[in] aSpan Ukazatel na existující Span
 *  \param[in] aIter Testovaný iterátor
 *  \return \c true pokud iterátor odkazuje na element ležící v rozsahu paměti, který definuje Span.
 */
static inline bool vector_iterator_is_inside_span(const struct TSpan *aSpan, const struct TVectorIterator *aIter)
	{
	if(aIter && aIter->iVector)
		{
		assert(aIter->iVector->iValues);
		const TVectorElement *elem_ptr = aIter->iVector->iValues + aIter->iPos;

		return (elem_ptr >= aSpan->iVector.iValues) && (elem_ptr < aSpan->iVector.iValues + aSpan->iVector.iSize);
		}
	return false;
	}

/** \} TSpan */

#endif /* TSPAN_H */
