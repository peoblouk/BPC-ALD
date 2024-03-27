#ifndef UT_ADAPTER_H
#define UT_ADAPTER_H
/** \file ut_adapter.h
 *  \brief Unit Test adapter for M$ Visual Studio and Apple XCode main include file
 *  \author Petyovský
 *  \version 2024
 *  $Id: ut_adapter.h 2646 2024-03-18 13:41:07Z petyovsky $
 */

#ifdef _MSC_VER
	#include "ut_adapter_vs.h"

#elif defined(__MACH__)
	#include "ut_adapter_xc.h"

#else
	#error "Unknown compiler infrastructure"

#endif

#endif /* UT_ADAPTER_H */
