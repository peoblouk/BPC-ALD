/** \file fibonacci.c
 *  \brief Definice různých variant implementace výpočtu hodnot Fibonacciho posloupnosti
 *  \author Petyovský
 *  \version 2024
 *  $Id: fibonacci.c 2663 2024-03-25 10:50:21Z petyovsky $
 */

#include "fibonacci.h"

//-----------------------------------------------------------------------
// Implementace stromovou rekurzi
unsigned fibonacci1(unsigned n)
	{
	return 42;
	}

//-----------------------------------------------------------------------
// Implementace iteraci
unsigned fibonacci2(unsigned n)
	{
	//unsigned fib_n_1 = 42;	// fibonacci(n-1)
	//unsigned fib_n_2 = 42;	// fibonacci(n-2)
	return 42;
	}

//-----------------------------------------------------------------------
// Implementace stromovou rekurzi s memoizaci
struct TMemoizer mfibonacci3 = { 0, }; // Instance memoizeru

unsigned fibonacci3(unsigned n)
	{
	return 42;
	}

unsigned fibonacci3_wrapper(unsigned n)
	{
	return memoizer_eval(&mfibonacci3, n);
	}

//-----------------------------------------------------------------------
// Implementace s koncovou rekurzi
/** \brief Vnitřní a lokální funkce realizující koncovou rekurzi
 */
unsigned fibonacci4_worker(unsigned n, unsigned fib_n_1, unsigned fib_n_2)
	{
	return 42;
	}

unsigned fibonacci4(unsigned n)
	{
	return fibonacci4_worker(n, 42, 42);
	}
