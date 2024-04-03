/*******************************************************************
**                                                                **
**  Knihovna pro kontrolu prace s alokovanou pameti a se soubory  **
**  Autor: Pavel Sabatka a Petr Petyovsky                         **
**  Implementovano v ramci bakalarske prace                       **
**  VUT v Brne, UAMT, FEKT v ak. roce 2007, 2008, 2010 - 2023     **
**  rel. 2012.28                                                  **
**                                                                **
*******************************************************************/
/* $Id: check.h 2088 2023-02-06 16:08:37Z petyovsky $ */

/**
** NAVOD K POUZITI:
**	Pro pouziti teto knihovny je treba:
**	  1)  pripojit soubory check.h a check.c k projektu
**	  2)  pripojit check.h directivou #include. Tj. do
**		  hlavickoveho souboru uvest
**			#include "check.h"
**
** UZIVATELSKA MAKRA:
**	Jsou definovany 3 uzivatelske makra, ktere lze volat z programu:
**		- void memory_stat(void); - aktualni vypis alokovane pameti
**		- void file_stat(void); - vypis otevrenych souboru v dane chvili
**		- void stat(void); - slucuje obe predchozi funkce
**/

#ifndef CHECK_H
#define CHECK_H

#if !defined(_CRT_SECURE_NO_DEPRECATE) && (defined(_MSC_VER) || defined(WIN32) || defined(_WIN64))
	#define _CRT_SECURE_NO_DEPRECATE 1		/* Due to MSVC Security development lifecycle (SDL) checks */
#endif

#define CHECKER_API extern

/* include knihoven */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
	#include <new>
#endif /* __cplusplus */

/*******************************
*   definice maker pro check   *
*******************************/
/* presmerovani vystupu checkeru standardni / chybovy vystup */
#define CHECKER_OUT stderr

/* delka nazvu funkci a souboru */
#define CHECKER_FILENAME_MAXLEN 25
#define CHECKER_FUNCNAME_MAXLEN 20
#define CHECKER_PATHNAME_MAXLEN 25
#define CHECKER_FILEMODE_MAXLEN 10

/* sloupce tabulky vypisu - pri odstraneni definice makra se sloupec nezobrazi */
#define CHECKER_TAB_INDEX
#define CHECKER_TAB_FILE_LINENUM
#define CHECKER_TAB_FUNCTION
#define CHECKER_TAB_SIZE
#define CHECKER_TAB_MEM_ADDR

/* makro __func__  je definovano pouze v C99 a C++11 */
/* ISO/IEC N3242=11-0012 [dcl.fct.def.general]
 * It is unspecified whether such a variable __func__has an address distinct from
 * that of any other object in the program.
 */
#if (__STDC_VERSION__ >= 199901L) || (__cplusplus >= 201103L)
	#define CHECKER_FUNCNAME __func__
#elif (_MSC_VER >= 1500) /* >= Visual Studio 2008 */
	#define CHECKER_FUNCNAME __FUNCTION__
#else
	#define CHECKER_FUNCNAME ""
#endif /* (__STDC_VERSION__ >= 199901L) || (__cplusplus >= 201103L) */

/* definice maker pro funkce */
#define malloc(aSize)				checker_malloc((aSize), __LINE__, __FILE__, CHECKER_FUNCNAME)
#define calloc(aCount, aSize)		checker_calloc((aCount), (aSize), __LINE__, __FILE__, CHECKER_FUNCNAME)
#define realloc(aPtr, aSize)		checker_realloc((aPtr), (aSize), __LINE__, __FILE__, CHECKER_FUNCNAME)
#define free(aPtr)					checker_free((aPtr), __LINE__, __FILE__, CHECKER_FUNCNAME)
#define fopen(aFileName, aMode)		checker_fopen((aFileName), (aMode), __LINE__, __FILE__, CHECKER_FUNCNAME)
#define fclose(aFile)				checker_fclose((aFile), __LINE__, __FILE__, CHECKER_FUNCNAME)

/* uzivatelske makra */
/* vypise aktualni alokovanou pamet a otevrene soubory */
#define stat()						checker_stat(__LINE__, __FILE__, CHECKER_FUNCNAME)
/* vypise aktualni alokovanou pamet */
#define memory_stat()				checker_memory_stat(__LINE__, __FILE__, CHECKER_FUNCNAME)
/* vypise aktualni otevrene soubory */
#define file_stat()					checker_file_stat(__LINE__, __FILE__, CHECKER_FUNCNAME)

/* vraci nejvyssi vrchol alokovane pameti (od spusteni programu az po soucasnost) */
#define get_alloc_max_peak_size()	checker_get_alloc_max_peak_size(__LINE__, __FILE__, CHECKER_FUNCNAME)
/* vraci sumu celkove naalokovane pameti */
#define get_alloc_total_size()		checker_get_alloc_total_size(__LINE__, __FILE__, CHECKER_FUNCNAME)
/* vraci sumu celkove dealokovane pameti */
#define get_dealloc_total_size()	checker_get_dealloc_total_size(__LINE__, __FILE__, CHECKER_FUNCNAME)
/* vraci velikost aktualne alokovane pameti */
#define get_alloc_actual_size()		checker_get_alloc_actual_size(__LINE__, __FILE__, CHECKER_FUNCNAME)
/* vraci celkovy pocet pozadavku na alokaci pameti */
#define get_alloc_count()			checker_get_alloc_count(__LINE__, __FILE__, CHECKER_FUNCNAME)
/* vraci celkovy pocet pozadavku na realokaci pameti */
#define get_realloc_count()			checker_get_realloc_count(__LINE__, __FILE__, CHECKER_FUNCNAME)
/* vraci celkovy pocet pozadavku na dealokaci pameti */
#define get_dealloc_count()			checker_get_dealloc_count(__LINE__, __FILE__, CHECKER_FUNCNAME)
/* vraci celkovy pocet pozadavku na otevreni souboru */
#define get_fopen_count()			checker_get_fopen_count(__LINE__, __FILE__, CHECKER_FUNCNAME)
/* vraci celkovy pocet pozadavku na zavreni souboru */
#define get_fclose_count()			checker_get_fclose_count(__LINE__, __FILE__, CHECKER_FUNCNAME)
/* Aktivace zamku na funkcnich alokaci, vsechny alokace vetsi nez aAfterLimit budou hlasit chybu alokace, funkce vraci klic pomoci ktereho je mozne zamek opet deaktivovat */
#define lock_alloc(aAfterLimit)		checker_lock_alloc(aAfterLimit, __LINE__, __FILE__, CHECKER_FUNCNAME)
/* Odemknuti zamku alokaci, parametrem je hodnota klice vracena pri volani funkce pro zakmuti */
#define unlock_alloc(aAlloc_lock_key) checker_unlock_alloc(aAlloc_lock_key, __LINE__, __FILE__, CHECKER_FUNCNAME)
/* Aktivace zamku na funkcnich realokaci, vsechny realokace vetsi nez aAfterLimit budou hlasit chybu realokace, (mimo volani funkce realloc ktery funguji jako free), funkce vraci klic pomoci ktereho je mozne zamek opet deaktivovat */
#define lock_realloc(aAfterLimit)	checker_lock_realloc(aAfterLimit, __LINE__, __FILE__, CHECKER_FUNCNAME)
/* Odemknuti zamku realokaci, parametrem je hodnota klice vracena pri volani funkce pro zakmuti */
#define unlock_realloc(aRealloc_lock_key) checker_unlock_realloc(aRealloc_lock_key, __LINE__, __FILE__, CHECKER_FUNCNAME)

#ifdef __cplusplus
	extern "C"
		{
#endif /* __cplusplus */

/* uzivatelske funkce */
/* vypise aktualni alokovanou pamet a otevrene soubory */
CHECKER_API void checker_stat(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* vypise aktualni alokovanou pamet */
CHECKER_API void checker_memory_stat(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* vypise aktualni otevrene soubory */
CHECKER_API void checker_file_stat(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);

/* vraci nejvyssi vrchol alokovane pameti (od spusteni programu az po soucasnost) */
CHECKER_API size_t checker_get_alloc_max_peak_size(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* vraci sumu celkove naalokovane pameti */
CHECKER_API size_t checker_get_alloc_total_size(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* vraci sumu celkove dealokovane pameti */
CHECKER_API size_t checker_get_dealloc_total_size(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* vraci velikost aktualne alokovane pameti */
CHECKER_API size_t checker_get_alloc_actual_size(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* vraci celkovy pocet pozadavku na alokaci pameti */
CHECKER_API unsigned int checker_get_alloc_count(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* vraci celkovy pocet pozadavku na realokaci pameti */
CHECKER_API unsigned int checker_get_realloc_count(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* vraci celkovy pocet pozadavku na dealokaci pameti */
CHECKER_API unsigned int checker_get_dealloc_count(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* vraci celkovy pocet pozadavku na otevreni souboru */
CHECKER_API unsigned int checker_get_fopen_count(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* vraci celkovy pocet pozadavku na zavreni souboru */
CHECKER_API unsigned int checker_get_fclose_count(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);

/* Aktivace zamku na funkcnich alokaci, vsechny alokace vetsi nez aAfterLimit budou hlasit chybu alokace, funkce vraci klic pomoci ktereho je mozne zamek opet deaktivovat */
CHECKER_API size_t checker_lock_alloc(unsigned int aAfterLimit, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* Odemknuti zamku alokaci, parametrem je hodnota klice vracena pri volani funkce pro zakmuti */
CHECKER_API size_t checker_unlock_alloc(size_t aAlloc_lock_key, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* Aktivace zamku na funkcnich realokaci, vsechny realokace vetsi nez aAfterLimit budou hlasit chybu realokace, (mimo volani funkce realloc ktery funguji jako free), funkce vraci klic pomoci ktereho je mozne zamek opet deaktivovat */
CHECKER_API size_t checker_lock_realloc(unsigned int aAfterLimit, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
/* Odemknuti zamku realokaci, parametrem je hodnota klice vracena pri volani funkce pro zakmuti */
CHECKER_API size_t checker_unlock_realloc(size_t aRealloc_lock_key, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);

/*  kontrolni funkce */
CHECKER_API void* checker_malloc(size_t aSize, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
CHECKER_API void* checker_calloc(size_t aCount, size_t aSize, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
CHECKER_API void* checker_realloc(void *aReallocPtr, size_t aSize, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
CHECKER_API void checker_free(void *aFreePtr, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);

CHECKER_API FILE* checker_fopen(const char aPath[CHECKER_PATHNAME_MAXLEN], const char aMode[CHECKER_FILEMODE_MAXLEN], unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);
CHECKER_API int checker_fclose(FILE *aStreamPtr, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);

/* Push checker's hooked functions to the std namespace */
#ifdef __cplusplus
	namespace std
		{
		using ::checker_malloc;
		using ::checker_calloc;
		using ::checker_realloc;
		using ::checker_free;
		using ::checker_fopen;
		using ::checker_fclose;
		} /* namespace std */
#endif /* __cplusplus */

#ifdef __cplusplus
		} /* extern "C" */
#endif /* __cplusplus */

/* KONTROLA PAMETI PRO C++ */
#ifdef __cplusplus

#if (__cplusplus >= 201103L) || (defined(_MSC_VER) && (_MSC_VER >= 1900))		/* C++11 and newer or Visual Studio 2015 and newer */
	#define CHECKER_BAD_ALLOC_EXCEPTION
	#define CHECKER_NO_EXCEPTION			noexcept

#elif (__cplusplus < 201103L) && (__cplusplus >= 199711L) && !defined(_MSC_VER) || ((__GNUC__ == 4) && (__GNUC_MINOR__ < 7)) /* C++98 and not Visual Studio or gcc (4.0-4.6) */
	#define CHECKER_BAD_ALLOC_EXCEPTION		throw(std::bad_alloc)
	#define CHECKER_NO_EXCEPTION			throw()

#elif (defined(_MSC_VER) && (_MSC_VER <= 1800)) || defined(WIN32) || defined(_WIN64)	/* up to Visual Studio 2013 not newer */
	#define CHECKER_BAD_ALLOC_EXCEPTION			/* Must be empty, otherwise MSVC report: warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow) */
	#define CHECKER_NO_EXCEPTION			throw()

#else										/* Pre C++98 */
	#define CHECKER_BAD_ALLOC_EXCEPTION
	#define CHECKER_NO_EXCEPTION
#endif

	/* kontrolni operatory */
	CHECKER_API void* operator new(size_t aSize) CHECKER_BAD_ALLOC_EXCEPTION;
	CHECKER_API void* operator new[](size_t aSize) CHECKER_BAD_ALLOC_EXCEPTION;
	CHECKER_API void* operator new(size_t aSize, const std::nothrow_t& aNoThrow) CHECKER_NO_EXCEPTION;
	CHECKER_API void* operator new[](size_t aSize, const std::nothrow_t& aNoThrow) CHECKER_NO_EXCEPTION;

	CHECKER_API void operator delete(void *aPtr) CHECKER_NO_EXCEPTION;
	CHECKER_API void operator delete[](void *aPtr) CHECKER_NO_EXCEPTION;

	#if (__cplusplus >= 201402L) || (_MSC_FULL_VER >= 190023918L)		/* C++14 global sized dealocation feature */
		CHECKER_API void operator delete(void *aPtr, size_t aSize) CHECKER_NO_EXCEPTION;
		CHECKER_API void operator delete[](void *aPtr, size_t aSize) CHECKER_NO_EXCEPTION;
	#endif

	CHECKER_API bool checker_new_recorder(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN]);

	/* pomocna trida pro ulozeni zaznamu pri chybe v delete */
	class CCheckerDeleteRecorder
		{
		const char * const iSrcFileName;
		const char * const iSrcFuncName;
		const unsigned iSrcLineNum;
		
	public:
		CCheckerDeleteRecorder(const unsigned aSrcLineNum, const char * const aSrcFileName, const char * const aSrcFuncName): iSrcFileName(aSrcFileName), iSrcFuncName(aSrcFuncName), iSrcLineNum(aSrcLineNum)
			{}
		~CCheckerDeleteRecorder();
		operator bool() const
			{return (!iSrcFileName) && (!iSrcFuncName);} /* Always return false! No so easy to understand for compiler optimizer. I hope. */
		};

	/* definice maker pro operatory */
	#define CHECKER_NEW_OPER		checker_new_recorder(__LINE__, __FILE__, CHECKER_FUNCNAME) ? 0 : new
	#define new CHECKER_NEW_OPER

	#define CHECKER_DELETE_OPER		CCheckerDeleteRecorder(__LINE__, __FILE__, CHECKER_FUNCNAME) ? (void)0 : (void) delete
	#define delete CHECKER_DELETE_OPER

#endif /* __cplusplus */

#endif /* CHECK_H */
