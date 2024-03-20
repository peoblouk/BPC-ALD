#ifndef MAIN_SUPPORT_H
#define MAIN_SUPPORT_H
/** \file main_support.h
 *  \brief Soubor pro multiplatformní podporu programu v různých OS
 *  \author Petyovský
 *  \version 2024
 *  $Id: main_support.h 2581 2024-02-05 14:24:55Z petyovsky $
 */

#include <time.h>

#ifndef ITEMS_OF
 #define ITEMS_OF(aArray) (sizeof(aArray) / sizeof((aArray)[0]))	///< Vrací počet prvků pole předaného jako parametr \p aArray
#endif

/** \brief Výpočet doby trvání časového intervalu
 *  \details Výpočte délku trvání časového intervalu předaného jako parametry.
 *  \param[in] aEnd Konec časového intervalu
 *  \param[in] aBegin Začátek časového intervalu
 *  \return Doba trvání časového intervalu (desetinné číslo v sekundách typu \c double)
 */
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

#endif /* MAIN_SUPPORT_H */
