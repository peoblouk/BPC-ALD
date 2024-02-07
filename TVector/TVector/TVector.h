#ifndef TVECTOR_H
#define TVECTOR_H
/** \file TVector.h
 *  \brief Definice typu Basic Vector
 *  \author Petyovský
 *  \version 2024
 *  $Id: TVector.h 2584 2024-02-05 14:33:25Z petyovsky $
 */

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "TVectorElement.h"
#include "check.h"

/** \defgroup TVector 2. Vector
 *  \brief Definice datového typu Vector a jeho funkcí
 *  \{
 */

/** \brief Definice typu Vector
 *  \details Typ Vector spravuje dynamicky alokované pole prvků typu VectorElement a umožňuje s nimi pracovat pomocí definovaného API.
 *  \invariant Obecně platný a testovatelný předpoklad: `(iValues == NULL && iSize == 0)` nebo `(iValues != NULL && iSize != 0)`.
 */
struct TVector
	{
	TVectorElement *iValues;		///< Ukazatel na počáteční prvek pole hodnot typu VectorElement
	size_t iSize;					///< Počet elementů vektoru
	};

/** \brief Alokace vektoru a inicializace jeho elementů na hodnotu 0
 *  \details Dynamicky alokuje paměť pro pole o zadaném počtu elementů, inicializuje elementy na hodnotu 0.
 *  \param[in,out] aVector Ukazatel na místo v paměti určené pro inicializaci vektoru
 *  \param[in] aSize Požadovaný počet elementů vektoru
 *  \return \c true pokud byl vektor úspěšně alokován a inicializován
 */
bool vector_init(struct TVector *aVector, size_t aSize);

/** \brief Alokace vektoru a inicializace jeho elementů na náhodné hodnoty
 *  \details Dynamicky alokuje paměť pro pole o zadaném počtu elementů, inicializuje elementy na náhodné hodnoty.
 *  \param[in,out] aVector Ukazatel na místo v paměti určené pro inicializaci vektoru
 *  \param[in] aSize Požadovaný počet elementů vektoru
 *  \return \c true pokud byl vektor úspěšně alokován a inicializován
 */
bool vector_init_random(struct TVector *aVector, size_t aSize);

/** \brief Alokace vektoru a inicializace jeho elementů ze souboru
 *  \details Načte ze souboru počet elementů vektoru, dynamicky alokuje paměť pro vektor o načteném počtu elementů, načte ze souboru hodnoty jednotlivých elementů.
 *  \param[in,out] aVector Ukazatel na místo v paměti určené pro inicializaci vektoru
 *  \param[in,out] aInputFile Ukazatel na soubor otevřený v módu pro čtení
 *  \return \c true pokud byl vektor z dat v souboru úspěšně alokován a inicializován
 */
bool vector_init_file(struct TVector *aVector, FILE *aInputFile);

/** \brief Vytvoření úplné kopie vektoru
 *  \details Dynamicky alokuje paměť pro nové pole o shodném počtu elementů, nakopíruje hodnoty všech elementů ze starého do nového vektoru.
 *  \param[in] aVector Ukazatel na místo v paměti obsahující zdrojový vektor
 *  \param[in,out] aVectorClone Ukazatel na místo v paměti určené pro vytvořenou kopii
 *  \return \c true pokud byl nový vektor úspěšně alokován a inicializován
 */
bool vector_clone(const struct TVector *aVector, struct TVector *aVectorClone);

/** \brief Uložení vektoru a jeho elementů do souboru
 *  \details Uloží do souboru počet elementů vektoru a hodnoty jednotlivých elementů.
 *  \param[in] aVector Ukazatel na existující vektor
 *  \param[in,out] aOutputFile Ukazatel na soubor otevřený v módu pro zápis
 *  \return \c true pokud byl vektor a jeho elementy úspěšně uloženy do souboru
 */
bool vector_store_file(const struct TVector *aVector, FILE *aOutputFile);

/** \brief Přečtení hodnoty elementu z vektoru na požadované pozici
 *  \details Vrací hodnotu elementu z požadované pozice (indexu) vektoru.
 *  \param[in] aVector Ukazatel na existující vektor určený pro přečtení hodnoty elementu
 *  \param[in] aPos Požadovaná pozice (index) elementu
 *  \return Hodnota elementu uložená ve vektoru \p aVector na pozici \p aPos
 *  \attention Funkce ověřuje platnost ukazatele a rozsah parametru \p aPos \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
TVectorElement vector_value(const struct TVector *aVector, size_t aPos);

/** \brief Zapsání hodnoty elementu do vektoru na požadovanou pozici
 *  \details Zápis nové hodnoty elementu do vektoru na požadovanou pozici (index).
 *  \param[in,out] aVector Ukazatel na existující vektor určený pro zapsání hodnoty elementu
 *  \param[in] aPos Požadovaná pozice (index) elementu
 *  \param[in] aValue Nová hodnota elementu
 *  \attention Funkce ověřuje platnost ukazatele a rozsah parametru \p aPos \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
void vector_set_value(struct TVector *aVector, size_t aPos, TVectorElement aValue);

/** \brief Přečtení počtu elementů vektoru
 *  \details Vrací velikost (počet elementů) vektoru.
 *  \param[in] aVector Ukazatel na existující vektor
 *  \return Počet elementů vektoru nebo hodnota 0 v případě chyby
 */
size_t vector_size(const struct TVector *aVector);

/** \brief Změna velikosti existujícího vektoru
 *  \details Změna počtu elementů vektoru, alokace nové dynamické paměti pro pole o novém počtu prvků, kopie prvků z původního pole, inicializace doplněných prvků na hodnoty 0, dealokace dynamické paměti původního pole.
 *  \param[in,out] aVector Ukazatel na existující vektor
 *  \param[in] aNewSize Nový počet elementů vektoru
 *  \return \c true pokud byl vektor úspěšně přealokován a všechny elementy nového vektoru byly úspěšně inicializovány
 */
bool vector_resize(struct TVector *aVector, size_t aNewSize);

/** \brief Dealokace vektoru
 *  \details Dealokace dynamicky alokovaného pole, nastavení počtu elementů i ukazatele na hodnotu 0 (NULL).
 *  \param[in,out] aVector Ukazatel na existující vektor
 */
void vector_destroy(struct TVector *aVector);
/** \} TVector */

/** \defgroup TVectorIterator 4. Iterátor vektoru
 *  \brief Definice datového typu VectorIterator a jeho funkcí
 *  \{
 */

/** \brief Definice typu VectorIterator
 *  \details VectorIterator se při vzniku naváže na zvolený vektor a následně umožňuje přistupovat k jednotlivým elementům pomocí definovaného API.
 */
struct TVectorIterator
	{
	struct TVector *iVector;	///< Ukazatel na navázaný vektor (mutable iterátor - umožňuje měnit elementy VectorElement)
	size_t iPos;				///< Aktuální pozice pro indexaci elementu v navázaném vektoru
	};

/** \brief Vytvoření nového iterátoru ukazujícího na počáteční element
 *  \details Vytvoří a vrací nový iterátor, který je navázán (asociován) na zadaný vektor a ukazuje na jeho počáteční element.
 *  \param[in] aVector Ukazatel na existující vektor
 *  \return Nový iterátor asociovaný s vektorem \p aVector ukazující na počáteční element vektoru
 */
struct TVectorIterator vector_iterator_begin(const struct TVector *aVector);

/** \brief Vytvoření nového iterátoru ukazujícího na zadanou pozici
 *  \details Vytvoří a vrací nový iterátor, který je navázán (asociován) na zadaný vektor a ukazuje na element na zadané pozici.
 *  \param[in] aVector Ukazatel na existující vektor
 *  \param[in] aPos Zadaná pozice
 *  \return Nový iterátor asociovaný s vektorem \p aVector ukazující na element na zadané pozici \p aPos
 */
struct TVectorIterator vector_iterator_pos(const struct TVector *aVector, size_t aPos);

/** \brief Vytvoření nového iterátoru ukazujícího na poslední element
 *  \details Vytvoří a vrací nový iterátor, který je navázán (asociován) na zadaný vektor a ukazuje na jeho poslední element.
 *  \param[in] aVector Ukazatel na existující vektor
 *  \return Nový iterátor asociovaný s vektorem \p aVector ukazující na poslední element vektoru
 */
struct TVectorIterator vector_iterator_last(const struct TVector *aVector);

/** \brief Zjištění platnosti iterátoru
 *  \details Funkce (predikát) vracející \c bool hodnotu definující platnost iterátoru.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje na platné místo v asociovaném vektoru
 */
bool vector_iterator_is_valid(const struct TVectorIterator *aIter);

/** \brief Posunutí iterátoru vpřed
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, zajistí jeho posun vpřed (tj. na následující element v asociovaném vektoru).
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociovaném vektoru
 */
bool vector_iterator_to_next(struct TVectorIterator *aIter);

/** \brief Posunutí iterátoru vzad
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, zajistí jeho posun vzad (tj. na předchozí element v asociovaném vektoru).
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociovaném vektoru
 */
bool vector_iterator_to_prev(struct TVectorIterator *aIter);

/** \brief Přečtení hodnoty elementu z vektoru pomocí iterátoru
 *  \details Vrací hodnotu elementu vektoru z pozice (indexu) určeného iterátorem.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return Hodnota elementu vektoru z pozice, na kterou ukazuje iterátor \p aIter, nebo nulový element (pokud je iterátor neplatný).
 */
TVectorElement vector_iterator_value(const struct TVectorIterator *aIter);

/** \brief Zapsání hodnoty elementu do vektoru pomocí iterátoru
 *  \details Zápis nové hodnoty elementu do vektoru na pozici určenou iterátorem.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \param[in] aValue Nová hodnota elementu
 *  \return \c true pokud je iterátor platný a zápis mohl proběhnout na platné místo v asociovaném vektoru
 */
bool vector_iterator_set_value(const struct TVectorIterator *aIter, TVectorElement aValue);

/** \} TVectorIterator */

/** \defgroup IteratorAlgorithms 5. Univerzální funkce pro práci s iterátory
 *  \brief Definice funkcí používající iterátory
 *  \{
 */

/** \brief Zavolání zvolené funkce na každý element vektoru od pozice určené iterátorem až do konce vektoru
 *  \details Zavolá zadanou funkci \p aOperation na každý element vektoru v rozsahu od pozice určené iterátorem až do konce vektoru.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \param[in] aOperation Ukazatel na funkci vracející \c void a mající jeden parametr typu ukazatel na iterátor
 */
static inline void vector_for_each(struct TVectorIterator aIter, void(*aOperation)(const struct TVectorIterator *aIter))
	{
	for(bool valid = vector_iterator_is_valid(&aIter); valid; valid = vector_iterator_to_next(&aIter))
		aOperation(&aIter);
	}

/** \brief Zavolání zvolené funkce na prvních N elementů vektoru od pozice určené iterátorem
 *  \details Zavolá zadanou funkci \p aOperation na prvních \p aN elementů vektoru od počáteční pozice určené iterátorem. Pokud vektor má méně než \p aN elementů, funkce se ukončí dříve a vrátí neplatný iterátor.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \param[in] aN Hodnota udávající prvních N elementů vektoru, na které se má zavolat funkce \p aOperation
 *  \param[in] aOperation Ukazatel na funkci vracející \c void a mající jeden parametr typu ukazatel na iterátor
 *  \return Hodnota iterátoru ukazujícího za `aN`-tý element vektoru, nebo neplatný iterátor pokud vektor nemá další elementy.
 */
static inline struct TVectorIterator vector_for_n(struct TVectorIterator aIter, size_t aN, void(*aOperation)(const struct TVectorIterator *aIter))
	{
	for(bool valid = vector_iterator_is_valid(&aIter); aN && valid; --aN, valid = vector_iterator_to_next(&aIter))
		aOperation(&aIter);
	return aIter;
	}

/** \} IteratorAlgorithms */

#endif /* TVECTOR_H */
