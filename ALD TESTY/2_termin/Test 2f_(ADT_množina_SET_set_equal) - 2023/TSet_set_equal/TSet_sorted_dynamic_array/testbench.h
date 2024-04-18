#ifndef TESTBENCH_H
#define TESTBENCH_H
/** \file testbench.h
 *  \brief Definice testbench API pro testování operací zvoleného ADT
 *  \author Petyovský
 *  \version 2024
 *  $Id: testbench.h 2703 2024-04-12 15:38:04Z petyovsky $
 */

#include <stdbool.h>
#include "TSet.h"

#define TEST_MAXITEMS 30						///< Definuje maximální počet vstupních hodnot pro zadanou operací v jednom testu

typedef struct TSet			TTestedADT;			///< Definice typu testovaného ADT
typedef struct TSetIterator	TTestedADTIterator;	///< Definice typu iteratoru testovaného ADT

typedef TSetElement			TTestFuncInput;		///< Definice typu pro vstupní hodnoty operací testovaného ADT
typedef bool				TTestFuncOutput;	///< Definice typu pro výstupní hodnoty z operací testovaného ADT
#define TTEST_FUNC_OUTPUT_FRMSTR "%u"			///< Definice formátovacího řetězce pro tisk výstupní hodnoty z operací testovaného ADT

/// \brief Definice typu TTestFunc reprezentující požadovaný prototyp pro testovanou funkci
typedef TTestFuncOutput TTestFunc(TTestedADT*, TTestFuncInput);

/// \brief Typ TestOperation slouží k uchování názvu a adresy funkce reprezentující testovanou operaci
struct TTestOperation
		{
		const char *iFuncStr;					///< Ukazatel na řetězec představující název testované operace
		TTestFunc *iFunc;						///< Ukazatel na asociovanou funkci reprezentující testovanou operaci
		};

/// \brief Typ TestInputs slouží k uchování pole se vstupními hodnotami pro jednotlivé testovací operace
struct TTestInputs
		{
		size_t iSize;							///< Počet hodnot v poli vstupních hodnot
		TTestFuncInput iValues[TEST_MAXITEMS];	///< Pole vstupních hodnot pro jednotlivé testovací operace
		};

/// \brief Typ TestOutput slouží k uchování pole s očekávanými výstupními hodnotami z jednotlivých testovacích operací
struct TTestOutput
		{
		size_t iSize;							///< Počet hodnot v poli výstupních hodnot
		TTestFuncOutput iValue[TEST_MAXITEMS];	///< Pole s očekávanými výstupními hodnotami z jednotlivých testovacích operací
		};

/// \brief Typ TestPlan slouží k uchování položek definující jeden test testovacího plánu
struct TTestPlan
	{
	struct TTestOperation iOperation;			///< Testovaná operace
	struct TTestInputs iInputs;					///< Položky se vstupními hodnotami pro jednotlivé testovací operace
	struct TTestOutput iOutput;					///< Položky s očekávanými výstupními hodnotami z jednotlivých testovacích operací
	const char *iMessage;						///< Ukazatel na řetězec zprávy zobrazující se před začátkem testu případně může být i `NULL`
	bool iListOperations;						///< Příznak pro zobrazení kontrolního výpisu plánovaných operací před začátkem testu
	bool iReportBefore;							///< Příznak pro vypis obsahu testovaného ADT před začátkem testu
	bool iReportAfter;							///< Příznak pro vypis obsahu testovaného ADT po provedení testu
	bool iMemoryStats;							///< Příznak pro vypis statistik alokované paměti
	};

#ifndef ITEMS_OF
 #define ITEMS_OF(aArray) (sizeof(aArray) / sizeof((aArray)[0]))	///< Vrací počet prvků pole předaného jako parametr \p aArray
#endif

#define TEST_BRACED_INIT_LIST(...) {__VA_ARGS__}					///< Vytváří seznam variadic parametrů ve složených závorkách pro zpracování maker dle normy jazyka C

/// \brief \b Povinný příkaz pro definici testu: definuje testovanou operaci
/// \param[in] aFunc Jméno funkce která, reprezentuje testovanou operaci
#define TEST_OPERATION(aFunc)	.iOperation = { .iFuncStr = #aFunc, .iFunc = (TTestFunc*)(aFunc) },

/// \brief \b Povinný příkaz pro definici testu: definuje vstupní hodnoty pro jednotlivé operace
/// \param[in] ... Čárkami oddělený seznam hodnot
/// \warning Počet položek uvedený v seznamu hodnot u tohoto příkazu musí být shodný s počtem položek uvedený v příkazu \c TEST_OUTPUT
#define TEST_INPUTS(...)		.iInputs = { .iSize = ITEMS_OF((const TTestFuncInput[]) TEST_BRACED_INIT_LIST(__VA_ARGS__)), .iValues = TEST_BRACED_INIT_LIST(__VA_ARGS__) },

/// \brief \b Povinný příkaz pro definici testu: definuje očekávané návratové hodnoty pro jednotlivé operace
/// \param[in] ... Čárkami oddělený seznam hodnot
/// \warning Počet položek uvedený v seznamu hodnot u tohoto příkazu musí být shodný s počtem položek uvdedený v příkazu \c TEST_INPUTS
#define TEST_OUTPUT(...)		.iOutput = { .iSize = ITEMS_OF((const TTestFuncOutput[]) TEST_BRACED_INIT_LIST(__VA_ARGS__)), .iValue = TEST_BRACED_INIT_LIST(__VA_ARGS__) },

/// \brief Nepovinný příkaz pro definici testu: definuje zobrazení zadané zprávy před začátkem testu
/// \param[in] aStr Řetězec se zadanou zprávou
#define TEST_MESSAGE(aStr)		.iMessage = (aStr),

/// \brief Nepovinný příkaz pro definici testu: Povoluje zobrazení kontrolního výpisu plánovaných operací před začátkem testu
#define TEST_LIST_OPERATIONS	.iListOperations = true,

/// \brief Nepovinný příkaz pro definici testu: Povoluje vypis obsahu testovaného ADT před začátkem testu
#define TEST_REPORT_BEFORE		.iReportBefore = true,

/// \brief Nepovinný příkaz pro definici testu: Povoluje vypis obsahu testovaného ADT po provedení testu
#define TEST_REPORT_AFTER		.iReportAfter = true,

/// \brief Nepovinný příkaz pro definici testu: Povoluje vypis statistik alokované paměti
#define TEST_PRINT_MEMORY_STATS	.iMemoryStats = true,

/** \brief Vytisknutí hodnoty v ADT pomocí iterátoru
 *  \details Tisk hodnoty ADT, na kterou ukazuje předaný iterátor.
 *  \param[in] aIter Ukazatel na existující iterátor
 */
void print_value_by_iterator(const TTestedADTIterator *aIter);

/** \brief Provedení všech testů zadaného testovacího plánu na předaném ADT
 *  \details Funkce provede všechny testy zadaného testovacího plánu, změří a vytiskne celkový výpočetní čas a velikost paměti, kterou funkce potřebovala k dokončení výpočtu.
 *  \param[in,out] aADT Ukazatel na ADT na němž proběhne daný test
 *  \param[in] aTestPlanSize Počet testů v testovacím plánu
 *  \param[in] aTestPlan Ukazatel na pole testů s určenou: operací, polem vstupních hodnot a správných návratových hodnot
 *  \return \c true pokud test pro daný test pro pole zadaných vstupních hodnot dopadl úspěšně
 */
bool testbench_run(TTestedADT *aADT, size_t aTestPlanSize, const struct TTestPlan aTestPlan[/*aTestPlanSize*/]);

#endif /* TESTBENCH_H */
