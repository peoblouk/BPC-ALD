/** \file testbench.c
 *  \brief Implementace testbench API pro testování operací zvoleného ADT
 *  \author Petyovský
 *  \version 2024
 *  $Id: testbench.c 2703 2024-04-12 15:38:04Z petyovsky $
 */
#include <stdio.h>
#include <time.h>
#include "testbench.h"

void print_value_by_iterator(const TTestedADTIterator *aIter)
	{
	set_element_store_file(set_iterator_value(aIter), stdout);
	}

static inline double difftimespec(struct timespec aEnd, struct timespec aBegin)
	{
	return difftime(aEnd.tv_sec, aBegin.tv_sec) + (((long long) aEnd.tv_nsec - aBegin.tv_nsec) / 1000000000.0);
	}

#ifdef __MACH__ // Detect macOS
	#include <Availability.h>
	#if __MAC_OS_X_VERSION_MIN_REQUIRED < 101500 // macOS < 10.15 (pre Catalina) does not have timespec_get, use clock_get_time
		#include <mach/clock.h>
		#include <mach/mach.h>

		#define TIME_UTC 1
		static inline int timespec_get(struct timespec *ts, int unused)
			{
			clock_serv_t cclock;
			mach_timespec_t mts;
			host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
			clock_get_time(cclock, &mts);
			mach_port_deallocate(mach_task_self(), cclock);
			ts->tv_sec = mts.tv_sec;
			ts->tv_nsec = mts.tv_nsec;
			return unused;
			}
	#endif // __MAC_OS_X_VERSION_MIN_REQUIRED
#endif // __MACH__

/** \brief Provedení jednoho test z testovacího plánu na předaném ADT
 *  \details Funkce provede jeden test z testovacího plánu, změří a vytiskne celkový výpočetní čas a velikost paměti, kterou funkce potřebovala k dokončení výpočtu.
 *  \param[in,out] aADT Ukazatel na ADT na němž proběhne daný test
 *  \param[in] aTestPlan Ukazatel na test s určenou: operací, polem vstupních hodnot a správných návratových hodnot
 *  \return \c true pokud test pro daný test pro pole zadaných vstupních hodnot dopadl úspěšně
 */
static bool test_run(TTestedADT *aADT, const struct TTestPlan *aTestPlan)
	{
	if(!aADT || !aTestPlan)
		return false;

	if(aTestPlan->iMessage && aTestPlan->iMessage[0] != '\0')
		printf("----- %s -----\n", aTestPlan->iMessage);

	if(aTestPlan->iReportBefore)
		{
		printf("\n  Report of state before test:\n\t");
		set_for_each(set_iterator_begin(aADT), print_value_by_iterator);
		putchar('\n');
		}

	if(aTestPlan->iListOperations)
		{
		printf("\n  Test listing with %zu operation(s):\n", aTestPlan->iInputs.iSize);
		for(size_t i = 0; i < aTestPlan->iInputs.iSize; ++i)
			{
			printf("\t%s( ", aTestPlan->iOperation.iFuncStr);
			set_element_store_file(aTestPlan->iInputs.iValues[i], stdout);
			printf(") = " TTEST_FUNC_OUTPUT_FRMSTR "\n", aTestPlan->iOutput.iValue[i]);
			}
		}

	const size_t alloc_count_before = get_alloc_count();
	const size_t realloc_count_before = get_realloc_count();
	const size_t dealloc_count_before = get_dealloc_count();
	const size_t alloc_size_before = get_alloc_actual_size();
	const size_t alloc_total_before = get_alloc_total_size();
	const size_t dealloc_total_before = get_dealloc_total_size();
	const size_t set_element_comparisons_before = gSetElementComparisons;

	puts("\n  Test started...");
	struct timespec start_timespec = {0,}, end_timespec = {0,};
	if(!timespec_get(&start_timespec, TIME_UTC))
		printf("Error on start timespec_get\n");

	for(size_t i = 0; i < aTestPlan->iInputs.iSize; ++i)
		{
		TTestFuncOutput result = aTestPlan->iOperation.iFunc(aADT, aTestPlan->iInputs.iValues[i]);
		if(result != aTestPlan->iOutput.iValue[i])
			{
			printf("\t%s( ", aTestPlan->iOperation.iFuncStr);
			set_element_store_file(aTestPlan->iInputs.iValues[i], stdout);
			printf(") = " TTEST_FUNC_OUTPUT_FRMSTR ", is wrong (must be " TTEST_FUNC_OUTPUT_FRMSTR ")!! FAIL :-(\n", 
				result, aTestPlan->iOutput.iValue[i]);
			return false;
			}
		}

	if(!timespec_get(&end_timespec, TIME_UTC))
		printf("Error on end timespec_get\n");

	puts("  Test finished...");
	printf("  All test results was valid. OK :-)\n");
	printf("  Calculation time: %lf seconds.\n\n", difftimespec(end_timespec, start_timespec));

	if(aTestPlan->iMemoryStats)
		{
		printf("  Dynamic memory statistics:\n\tAllocations:   %u, (dif: %+td)\n", get_alloc_count(), get_alloc_count() - alloc_count_before);
		printf("\tReallocations: %u, (dif: %+td)\n", get_realloc_count(), get_realloc_count() - realloc_count_before);
		printf("\tDeallocations: %u, (dif: %+td)\n\n", get_dealloc_count(), get_dealloc_count() - dealloc_count_before);

		printf("\tNow allocated:       %zu b, (dif: %+td b)\n", 
			get_alloc_actual_size(), get_alloc_actual_size() - alloc_size_before);
		printf("\tTotally allocated:   %zu b, (dif: %+td b)\n", 
			get_alloc_total_size(), get_alloc_total_size() - alloc_total_before);
		printf("\tTotally deallocated: %zu b, (dif: %+td b)\n\n", 
			get_dealloc_total_size(), get_dealloc_total_size() - dealloc_total_before);
		}

	printf("  TSetElement comparisons: %zu, (dif: %+td)\n", gSetElementComparisons, gSetElementComparisons - set_element_comparisons_before);

	if(aTestPlan->iReportAfter)
		{
		printf("\n  Report of state after test:\n\t");
		set_for_each(set_iterator_begin(aADT), print_value_by_iterator);
		putchar('\n');
		}

	putchar('\n');
	return true;
	}

bool testbench_run(TTestedADT *aADT, size_t aTestPlanSize, const struct TTestPlan aTestPlan[/*aTestPlanSize*/])
	{
	if(!aADT || !aTestPlan)
		return false;

	puts("Testbench registered new testplan...");
	printf("Testbench checking testplan with %zu test(s)...\n", aTestPlanSize);
	for(size_t i = 0; i < aTestPlanSize; ++i)
		{
		if(!aTestPlan[i].iOperation.iFunc)
			{
			printf("error: testplan #%zu: Missing OPERATION specifier!\a\n", i);
			return false;
			}
		if(aTestPlan[i].iInputs.iSize != aTestPlan[i].iOutput.iSize)
			{
			printf("error: testplan #%zu (%s): Different number of items in the INPUTS and OUTPUT test commands!\a\n", i, aTestPlan[i].iOperation.iFuncStr);
			return false;
			}
		}
	puts("Testbench accepted entire testplan...");

	for(size_t i = 0; i < aTestPlanSize; ++i)
		{
		printf("\nTestbench processing Test#%zu of %zu...\n", i, aTestPlanSize);
		if(!test_run(aADT, &aTestPlan[i]))
			{
			printf("\nTest#%zu failed...\n", i);
			return false;
			}
		}

	puts("Testbench successfully finished entire testplan...\n");
	return true;
	}
