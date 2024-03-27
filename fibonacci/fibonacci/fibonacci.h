#ifndef FIBONACCI_H
#define FIBONACCI_H
/** \file fibonacci.h
 *  \brief Deklarace různých variant implementace výpočtu hodnot Fibonacciho posloupnosti
 *  \author Petyovský
 *  \version 2024
 *  $Id: fibonacci.h 2663 2024-03-25 10:50:21Z petyovsky $
 */

#include "memoizer.h"

/** \defgroup Fibonacci 1. Fibonacci
 *  \brief Implementace funkcí pro výpočet Fibonacciho čísel
 *  \{
 */

/** \defgroup Fibonacci_recursion 1.1. Fibonacci - Implementace stromovou rekurzí
 *  \brief Implementace funkce pro výpočet Fibonacciho čísel pomocí stromové rekurze
 *  \{
 */

/** \brief Implementace výpočtu Fibonacciho čísla pomocí stromové rekurze
 *  \details Výpočte hodnotu Fibonacciho čísla pomocí implementace využívající stromovou rekurzi.
 *  \param[in] n Vstupní parametr určující kolikáté Fibonacciho číslo chceme vypočítat
 *  \return Výsledná hodnota Fibonacciho čísla
 */
unsigned fibonacci1(unsigned n);
/** \} Fibonacci_recursion */

/** \defgroup Fibonacci_iteration 1.2. Fibonacci - Implementace pomocí iterace
 *  \brief Implementace funkce pro výpočet Fibonacciho čísel pomocí iterace
 *  \{
 */

/** \brief Implementace výpočtu Fibonacciho čísla pomocí iterace (cyklu)
 *  \details Výpočte hodnotu Fibonacciho čísla pomocí implementace s iterací (cyklem).
 *  \param[in] n Vstupní parametr určující kolikáté Fibonacciho číslo chceme vypočítat
 *  \return Výsledná hodnota Fibonacciho čísla
 */
unsigned fibonacci2(unsigned n);
/** \} Fibonacci_iteration */

/** \defgroup Fibonacci_recursion_memoize 1.3. Fibonacci - Implementace stromovou rekurzí s memoizací
 *  \brief Implementace funkce pro výpočet Fibonacciho čísel pomocí stromové rekurze s využitím memoizace
 *  \{
 */
extern struct TMemoizer mfibonacci3;	///< Deklarace instance memoizeru

/** \brief Implementace výpočtu Fibonacciho čísla pomocí stromové rekurze s využitím memoizace
 *  \details Výpočte hodnotu Fibonacciho čísla pomocí implementace stromovou rekurzí s memoizací. Tato funkce slouží k inicializaci memoizeru a nebude přímo z `main()` volána.
 *  \param[in] n Vstupní parametr určující kolikáté Fibonacciho číslo chceme vypočítat
 *  \return Výsledná hodnota Fibonacciho čísla
 */
unsigned fibonacci3(unsigned n);

/** \brief Funkce výpočtu Fibonacciho čísla pomocí stromové rekurze s využitím memoizace volaná z `main()` jako tzv. obalová funkce.
 *  \details Funkce pouze zavolá funkci `memoizer_eval()` se stejným parametrem a příslušnou strukturou pro memoizované hodnoty. Je zde proto, abychom dodrželi shodné API pro všechny funkce generující Fibonacciho posloupnosti.
 *  \param[in] n Vstupní parametr určující kolikáté Fibonacciho číslo chceme vypočítat
 *  \return Výsledná hodnota Fibonacciho čísla
 */
unsigned fibonacci3_wrapper(unsigned n);
/** \} Fibonacci_recursion_memoize */

/** \defgroup Fibonacci_recursion_tail_call 1.4. Fibonacci - Implementace s koncovou rekurzí
 *  \brief Implementace funkce pro výpočet Fibonacciho čísel pomocí koncové rekurze
 *  \{
 */

/** \brief Implementace výpočtu Fibonacciho čísla pomocí koncové rekurze
 *  \details Výpočte hodnotu Fibonacciho čísla pomocí implementace využívající koncovou rekurzi.
 *  \param[in] n Vstupní parametr určující kolikáté Fibonacciho číslo chceme vypočítat
 *  \return Výsledná hodnota Fibonacciho čísla
 */
unsigned fibonacci4(unsigned n);
/** \} Fibonacci_recursion_tail_call */

/** \} Fibonacci */

#endif /* FIBONACCI_H */
