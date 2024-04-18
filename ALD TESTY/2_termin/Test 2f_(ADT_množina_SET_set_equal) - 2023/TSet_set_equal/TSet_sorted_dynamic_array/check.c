/*******************************************************************
**                                                                **
**  Knihovna pro kontrolu prace s alokovanou pameti a se soubory  **
**  Autor: Pavel Sabatka a Petr Petyovsky                         **
**  Implementovano v ramci bakalarske prace                       **
**  VUT v Brne, UAMT, FEKT v ak. roce 2007, 2008, 2010 - 2023     **
**  rel. 2012.28                                                  **
**                                                                **
*******************************************************************/
/* $Id: check.c 2088 2023-02-06 16:08:37Z petyovsky $ */

#include "check.h"

#undef malloc
#undef calloc
#undef realloc
#undef free
#undef fopen
#undef fclose

#ifdef __cplusplus
	#undef new
	#undef delete
#endif /* __cplusplus */

/* Boolean type support */
#ifdef __cplusplus						/* C++ bool type support (probably also in C23) */
	#define CHECKER_BOOL	bool
	#define CHECKER_TRUE	true
	#define CHECKER_FALSE	false

#elif (__STDC_VERSION__ >= 199901L)		/* C99 bool type support */
	#include <stdbool.h>
	#define CHECKER_BOOL	bool
	#define CHECKER_TRUE	true
	#define CHECKER_FALSE	false

#else									/* C90 bool type replacement */
	#define CHECKER_BOOL	int
	#define CHECKER_TRUE	1
	#define CHECKER_FALSE	0
#endif /*__cplusplus */

/* Empty init list support */
#ifdef __cplusplus						/* C++ empty init list support (probably also in C23) */
	#define CHECKER_EMPTY_INIT_LIST		{}

#else									/* C empty init list replacement */
	#define CHECKER_EMPTY_INIT_LIST		{0,}
#endif /*__cplusplus */

/* nullptr C++11 keyword support */
#if defined(__cplusplus) && ( (__cplusplus >= 201103L) || (_MSC_VER >= 1600) )	/* C++11 or higher, Visual Studio 2010 or higher */
	#define CHECKER_NULL	nullptr
#else
	#define CHECKER_NULL	NULL
#endif /*__cplusplus */

/* libc's size_t support in the printf format */
#if (__STDC_VERSION__ >= 199901L) || (__cplusplus >= 201103L)
	#define CHECKER_SIZE_T_PRIu		"%zu"	/* pro typ size_t v C99 a C++11 jiz definovan modifikator velikosti (%z) */
	#define CHECKER_SIZE_T_PRId		"%zd"
	#define CHECKER_SIZE_T_CONV		size_t
#elif (_MSC_VER >= 1310)
	#define CHECKER_SIZE_T_PRIu		"%Iu"	/* od VS2003 definovan modifikator velikosti (%Iu) pro typ size_t, ktery je bud I32 nebo I64 */
	#define CHECKER_SIZE_T_PRId		"%Id"
	#define CHECKER_SIZE_T_CONV		size_t
#else
	#define CHECKER_SIZE_T_PRIu		"%lu"	/* retro compilers */
	#define CHECKER_SIZE_T_PRId		"%ld"
	#define CHECKER_SIZE_T_CONV		unsigned long int
#endif /* __STDC_VERSION__ */

/* Compiler and libraries support of the C11 Security Annex K (fopen_s, strncpy_s, strnlen_s, fprintf_s, ...) */
#if ((__STDC_LIB_EXT1__ >= 201112L) && (__STDC_WANT_LIB_EXT1__ != 0)) || (_MSC_VER >= 1600) /* C11 Security Annex K or higher, Visual Studio 2010 or higher */
	#define CHECKER_SECURE_SUPPORT
	#define CHECKER_STRNCPY_S(aDest, aDestsz, aSrc, aCount)		strncpy_s((aDest), (aDestsz), (aSrc), (aCount))
	#define CHECKER_STRNLEN_S(aStr, aDestsz)					strnlen_s((aStr), (aDestsz))
/* Problem s PATH_MAX a FILENAME_MAX, proto na nekterych mistech ponechan strlen */
/* FILENAME_MAXSIZE: <stdio.h> define FILENAME_MAX,   linux, unix, windows compatible */

#else
	#undef  CHECKER_SECURE_SUPPORT
	#define CHECKER_STRNCPY_S(aDest, aDestsz, aSrc, aCount)		strncpy((aDest), (aSrc), (aCount))
	#define CHECKER_STRNLEN_S(aStr, aDestsz)					strlen(aStr)
#endif /* __STDC_VERSION__ */

/* A macro for C/C++ compatibility of the unused parameter in the function body.
   C++ say: If a parameter is not used in the function body, it does not need to be named in the function prototype.
   otherwise C compilers emit error: expected formal parameter list, not a type list
*/
#ifdef __cplusplus		// || (__STDC_VERSION__ >= 202300L)		// New feature will be probably implemented in C++17 / C23 standards as a [[maybe_unused]] attribute, so can be enabled for C23 compilers
	#define CHECKER_UNUSED_PARAM(aPar)
#else
	#define CHECKER_UNUSED_PARAM(aPar)	aPar
#endif

/* include knihoven */
#include <string.h>
#include <time.h>

#define CHECKER_HIDDEN_FUNCNAME "##"
#define CHECKER_HIDDEN_SOURCE_FILENAME "#HiddenSource#"

#define CHECKER_PAGE_WIDTH 79

/****************************
*      DATOVE TYPY          *
****************************/
enum TAllocType {EMalloc = 0, ECalloc, ERealloc, ENew, ENewA, ENewNothrow, ENewNothrowA, ECount};
static const char *alloc_type_str[ECount] = {"malloc", "calloc", "realloc", "new", "new[]", "new(nothrow)", "new(nothrow)[]"};
#define CHECKER_MAXLENGTH_ALLOC_TYPE_STR 15		/* sizeof("new(nothrow)[]") = 15 */

/* prvek linearniho seznamu alokovane pameti */
struct TNodeMem
	{
	struct TNodeMem *iNext;			// ukazatel na dalsi prvek
	void *iMemAddress;				// adresa alokovane pameti
	size_t iMemSize;				// alokovana velikost
	unsigned int iSrcLineNum;		// radek zdrojoveho kodu (makro __LINE__)
	unsigned int iAllocNum;			// absolutni poradi alokace
	char iSrcFileName[CHECKER_FILENAME_MAXLEN];	// soubor, ve kterem je volana alokace pameti
	char iSrcFuncName[CHECKER_FUNCNAME_MAXLEN];	// funkce, ve ktere je volana alokace pameti
	enum TAllocType iAllocType;		// typ alokace pameti
	};

/* linearni seznam alokovane pameti */
struct TListMem
	{
	struct TNodeMem *iAct;
	struct TNodeMem *iFirst;

	// statistiky vyuziti pameti
	size_t iStat_alloc_size_max_peak;
	size_t iStat_alloc_size;
	size_t iStat_free_size;

	// pocitadla alokace/dealokace pameti
	unsigned int iCounter_alloc;
	unsigned int iCounter_realloc;
	unsigned int iCounter_dealloc;

	// podpora pro zamezeni alokace/realokace
	size_t iAlloc_lock_key;
	unsigned int iCounter_alloc_lock_limit;
	size_t iRealloc_lock_key;
	unsigned int iCounter_realloc_lock_limit;
	};

/* prvek linearniho seznamu otevrenych souboru */
struct TNodeFile
	{
	struct TNodeFile *iNext;		// ukazatel na dalsi prvek
	FILE *iStreamAddress;			// adresa handleru FILE
	unsigned int iSrcLineNum;		// radek zdrojoveho kodu (makro __LINE__)
	char iSrcFileName[CHECKER_FILENAME_MAXLEN];	// soubor, ve kterem je volano fopen
	char iSrcFuncName[CHECKER_FUNCNAME_MAXLEN];	// funkce, ve ktere je volano fopen
	char iPath[CHECKER_PATHNAME_MAXLEN];		// jmeno souboru
	char iMode[CHECKER_FILEMODE_MAXLEN];		// parametry otevirani souboru
	};

/* linearni seznam otevrenych souboru */
struct TListFile
	{
	struct TNodeFile *iAct;
	struct TNodeFile *iFirst;

	// pocitadla otevreni/zavreni souboru
	unsigned int iCounter_fopen;
	unsigned int iCounter_fclose;
	};

/****************************
*          FUNKCE           *
****************************/
// prace se seznamem - funkce pro kontrolu alokace v C
static void allocc_init_list(struct TListMem *aLm);					// inicializace
static void allocc_insert(											// vlozeni prvku
	struct TListMem *aLm,
	size_t aSize,
	void *aAddress,
	unsigned int aSrcLineNum,
	const char aSrcFileName[CHECKER_FILENAME_MAXLEN],
	const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN],
	enum TAllocType aAllocType);
static void allocc_resize_act(										// zmena parametru pouzite pro realloc()
	struct TListMem *aLm,
	size_t aSize,
	void *aAddress,
	unsigned int aSrcLineNum,
	const char aSrcFileName[CHECKER_FILENAME_MAXLEN],
	const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN],
	enum TAllocType aAllocType);
static void allocc_dispose_act(struct TListMem *aLm);				// zrusi aktualni prvek
static void allocc_set_act(struct TListMem *aLm, void *aAddress);	// nastavi prvek se zadanou adresou jako aktualni
static void allocc_set_last_act(struct TListMem *aLm);				// nastavi posledni prvek jako aktualni
static CHECKER_BOOL allocc_active(const struct TListMem *aLm);		// overi, jestli je seznam aktivni

// prace se seznamem - funkce pro kontrolu souboru v C
static void filec_init_list(struct TListFile *aLf);					// inicializace
static void filec_insert(											// vlozeni prvku
	struct TListFile *aLf,
	const char aPath[CHECKER_PATHNAME_MAXLEN],
	const char aMode[CHECKER_FILEMODE_MAXLEN],
	unsigned int aSrcLineNum,
	const char aSrcFileName[CHECKER_FILENAME_MAXLEN],
	const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN],
	FILE *aStream);
static void filec_dispose_act(struct TListFile *aLf);				// zrusi aktualni prvek
static void filec_set_act(struct TListFile *aLf, FILE *aStream);	// nastavi prvek se zadanou adresou jako aktualni
static void filec_set_last_act(struct TListFile *aLf);				// nastavi posledni prvek jako aktualni
static CHECKER_BOOL filec_active(const struct TListFile *aLf);		// overi, jestli je seznam aktivni

// prace se seznamem - pro oba seznamy
static void dispose_lists(											// zruseni vsech seznamu
	struct TListMem *aLm,
	struct TListFile *aLf);

// dalsi funkce
static void checkdel(void);											// tuto funkci pridavam do funkce exit
static void program_error(void);									// vyvola chybu, korektne uvolni pamet a soubory, ukonci program
static void print_lists(											// vypise statistiky programu
	struct TListMem *aLm,
	struct TListFile *aLf,
	CHECKER_BOOL aPrintAllocs,
	CHECKER_BOOL aPrintFiles);
static void printchar(size_t aFrom, size_t aTo, char aCh);			// opakovane vypisuje znak aCh dokud aFrom != aTo
static unsigned int chars_in_dec(size_t aValue);					// Vraci pocet znaku nutnych k vytisknuti cisla v desitkove soustave
static unsigned int chars_in_hex(ptrdiff_t aValue);					// Vraci pocet znaku nutnych k vytisknuti cisla v sestnactkove soustave

/***************************************
*  IMPLEMENTACE STATICKYCH PROMENNYCH  *
****************************************/
static struct TListMem Lm = CHECKER_EMPTY_INIT_LIST;	// seznam alokovane pameti
static struct TListFile Lf = CHECKER_EMPTY_INIT_LIST;	// seznam otevrenych souboru

static CHECKER_BOOL Used_alloc =  CHECKER_FALSE;		// bool - byla jiz alokovana nejaka pamet?
static CHECKER_BOOL Used_file_c = CHECKER_FALSE;		// bool - byl jiz otevren nejaky soubor?
static CHECKER_BOOL Correct_exit = CHECKER_TRUE;		// bool - byl program ukoncen bez chyby

#ifdef __cplusplus
	// Mailbox pro docasne ulozeni adresy dealokovane pameti pri volani operatoru delete
	static void *Checker_delete_err_ptr = CHECKER_NULL;
#endif /* __cplusplus */

#ifdef __cplusplus
	// Mailbox pro docasne ulozeni _LINE_, _FILE_, CHECKER_FUNCNAME hodnot pri volani operatoru new
	static CHECKER_BOOL Checker_new_record_full = CHECKER_FALSE;	// bool - Je ulozen v temp delete mailboxu nejaky zaznam?
	static unsigned int Checker_new_line = 0;
	static char Checker_new_soubor[CHECKER_FILENAME_MAXLEN] = {'\0'};
	static char Checker_new_func[CHECKER_FUNCNAME_MAXLEN] = {'\0'};
#endif /* __cplusplus */

/********************************************
*  IMPLEMENTACE FUNKCI  - UZIVATELSKYCH     *
********************************************/
/* vypise alokovanou pamet a otevrene soubory */
void checker_stat(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	fprintf(CHECKER_OUT, "%s: %u :%s\n", aSrcFileName, aSrcLineNum, aSrcFuncName);
	print_lists(&Lm, &Lf, CHECKER_TRUE, CHECKER_TRUE);
	}

/* vypise aktualni stav alokovane pameti */
void checker_memory_stat(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	fprintf(CHECKER_OUT, "%s: %u :%s\n", aSrcFileName, aSrcLineNum, aSrcFuncName);
	print_lists(&Lm, &Lf, CHECKER_TRUE, CHECKER_FALSE);
	}

/* vypise aktualni otevrene soubory */
void checker_file_stat(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	fprintf(CHECKER_OUT, "%s: %u: %s\n", aSrcFileName, aSrcLineNum, aSrcFuncName);
	print_lists(&Lm, &Lf, CHECKER_FALSE, CHECKER_TRUE);
	}

/* vraci nejvyssi vrchol alokovane pameti (od spusteni programu az po soucasnost) */
size_t checker_get_alloc_max_peak_size(unsigned int CHECKER_UNUSED_PARAM(aSrcLineNum), const char CHECKER_UNUSED_PARAM(aSrcFileName)[CHECKER_FILENAME_MAXLEN], const char CHECKER_UNUSED_PARAM(aSrcFuncName)[CHECKER_FUNCNAME_MAXLEN])
	{
	return Lm.iStat_alloc_size_max_peak;
	}

/* vraci sumu celkove naalokovane pameti */
size_t checker_get_alloc_total_size(unsigned int CHECKER_UNUSED_PARAM(aSrcLineNum), const char CHECKER_UNUSED_PARAM(aSrcFileName)[CHECKER_FILENAME_MAXLEN], const char CHECKER_UNUSED_PARAM(aSrcFuncName)[CHECKER_FUNCNAME_MAXLEN])
	{
	return Lm.iStat_alloc_size;
	}

/* vraci sumu celkove dealokovane pameti */
size_t checker_get_dealloc_total_size(unsigned int CHECKER_UNUSED_PARAM(aSrcLineNum), const char CHECKER_UNUSED_PARAM(aSrcFileName)[CHECKER_FILENAME_MAXLEN], const char CHECKER_UNUSED_PARAM(aSrcFuncName)[CHECKER_FUNCNAME_MAXLEN])
	{
	return Lm.iStat_free_size;
	}

/* vraci velikost aktualne alokovane pameti */
size_t checker_get_alloc_actual_size(unsigned int CHECKER_UNUSED_PARAM(aSrcLineNum), const char CHECKER_UNUSED_PARAM(aSrcFileName)[CHECKER_FILENAME_MAXLEN], const char CHECKER_UNUSED_PARAM(aSrcFuncName)[CHECKER_FUNCNAME_MAXLEN])
	{
	return Lm.iStat_alloc_size - Lm.iStat_free_size;
	}

/* vraci celkovy pocet pozadavku na alokaci pameti */
unsigned int checker_get_alloc_count(unsigned int CHECKER_UNUSED_PARAM(aSrcLineNum), const char CHECKER_UNUSED_PARAM(aSrcFileName)[CHECKER_FILENAME_MAXLEN], const char CHECKER_UNUSED_PARAM(aSrcFuncName)[CHECKER_FUNCNAME_MAXLEN])
	{
	return Lm.iCounter_alloc;
	}

/* vraci celkovy pocet pozadavku na realokaci pameti */
unsigned int checker_get_realloc_count(unsigned int CHECKER_UNUSED_PARAM(aSrcLineNum), const char CHECKER_UNUSED_PARAM(aSrcFileName)[CHECKER_FILENAME_MAXLEN], const char CHECKER_UNUSED_PARAM(aSrcFuncName)[CHECKER_FUNCNAME_MAXLEN])
	{
	return Lm.iCounter_realloc;
	}

/* vraci celkovy pocet pozadavku na dealokaci pameti */
unsigned int checker_get_dealloc_count(unsigned int CHECKER_UNUSED_PARAM(aSrcLineNum), const char CHECKER_UNUSED_PARAM(aSrcFileName)[CHECKER_FILENAME_MAXLEN], const char CHECKER_UNUSED_PARAM(aSrcFuncName)[CHECKER_FUNCNAME_MAXLEN])
	{
	return Lm.iCounter_dealloc;
	}

/* vraci celkovy pocet pozadavku na otevreni souboru */
unsigned int checker_get_fopen_count(unsigned int CHECKER_UNUSED_PARAM(aSrcLineNum), const char CHECKER_UNUSED_PARAM(aSrcFileName)[CHECKER_FILENAME_MAXLEN], const char CHECKER_UNUSED_PARAM(aSrcFuncName)[CHECKER_FUNCNAME_MAXLEN])
	{
	return Lf.iCounter_fopen;
	}

/* vraci celkovy pocet pozadavku na zavreni souboru */
unsigned int checker_get_fclose_count(unsigned int CHECKER_UNUSED_PARAM(aSrcLineNum), const char CHECKER_UNUSED_PARAM(aSrcFileName)[CHECKER_FILENAME_MAXLEN], const char CHECKER_UNUSED_PARAM(aSrcFuncName)[CHECKER_FUNCNAME_MAXLEN])
	{
	return Lf.iCounter_fclose;
	}

/* Aktivace zamku alokaci, vsechny alokace po poctu vetsim nez aAfterLimit budou hlasit chybu alokace(vcetne volani funkce realloc ktere funguji jako malloc alokace), funkce vraci klic pomoci ktereho je mozne zamek opet deaktivovat */
size_t checker_lock_alloc(unsigned int aAfterLimit, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	if(Lm.iAlloc_lock_key)
		{
		fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\nAlokace jiz byla zamknuta!!!\n"
			"Zdrojovy soubor:radek: %s:%u\n"
			"Volani funkce: %s()\n", aSrcFileName, aSrcLineNum, aSrcFuncName);
		return 0;
		}
	else
		{
		srand((unsigned)time(NULL));
		Lm.iAlloc_lock_key = (size_t) malloc(10);
		free((void*) Lm.iAlloc_lock_key);
		Lm.iAlloc_lock_key ^= rand();
		Lm.iCounter_alloc_lock_limit = Lm.iCounter_alloc + aAfterLimit;
		}
	return Lm.iAlloc_lock_key;
	}

/* Odemknuti zamku alokaci, parametrem je hodnota klice vracena pri volani funkce pro zamknuti */
size_t checker_unlock_alloc(size_t aAlloc_lock_key, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	if(Lm.iAlloc_lock_key != aAlloc_lock_key)
		{
		fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\nPokus o odemknuti zamku alokace spatnym klicem!!!\n"
			"Zdrojovy soubor:radek: %s:%u\n"
			"Volani funkce: %s()\n", aSrcFileName, aSrcLineNum, aSrcFuncName);
		return 0;
		}
	Lm.iAlloc_lock_key = 0;
	Lm.iCounter_alloc_lock_limit = 0;
	return 1;
	}

/* Aktivace zamku realokaci, vsechny realokace po poctu vetsim nez aAfterLimit budou hlasit chybu realokace, (mimo volani funkce realloc ktery funguji jako free nebo jako malloc), funkce vraci klic pomoci ktereho je mozne zamek opet deaktivovat */
size_t checker_lock_realloc(unsigned int aAfterLimit, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	if(Lm.iRealloc_lock_key)
		{
		fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\nRealokace jiz byla zamknuta!!!\n"
			"Zdrojovy soubor:radek: %s:%u\n"
			"Volani funkce: %s()\n", aSrcFileName, aSrcLineNum, aSrcFuncName);
		return 0;
		}
	else
		{
		srand((unsigned)time(NULL));
		Lm.iRealloc_lock_key = (size_t)malloc(10);
		free((void*)Lm.iRealloc_lock_key);
		Lm.iRealloc_lock_key ^= rand();
		Lm.iCounter_realloc_lock_limit = Lm.iCounter_realloc + aAfterLimit;
		}
	return Lm.iRealloc_lock_key;
	}

/* Odemknuti zamku realokaci, parametrem je hodnota klice vracena pri volani funkce pro zamknuti */
size_t checker_unlock_realloc(size_t aRealloc_lock_key, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	if(Lm.iRealloc_lock_key != aRealloc_lock_key)
		{
		fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\nPokus o odemknuti zamku realokace spatnym klicem!!!\n"
			"Zdrojovy soubor:radek: %s:%u\n"
			"Volani funkce: %s()\n", aSrcFileName, aSrcLineNum, aSrcFuncName);
		return 0;
		}
	Lm.iRealloc_lock_key = 0;
	Lm.iCounter_realloc_lock_limit = 0;
	return 1;
	}

/*****************************************************
*  IMPLEMENTACE ALOKACNICH A DEALOKACNICH FUNKCI - C *
*****************************************************/
void* checker_malloc(size_t aSize, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	void *memPtr = CHECKER_NULL;
	if(Used_alloc == CHECKER_FALSE)
		{
		Used_alloc = CHECKER_TRUE;
		allocc_init_list(&Lm);
		
		if(Used_file_c == CHECKER_FALSE)
			atexit(&checkdel);
		}

	if((!Lm.iAlloc_lock_key) || (Lm.iCounter_alloc < Lm.iCounter_alloc_lock_limit))
		memPtr = (void *) malloc(aSize);

	if(memPtr)
		allocc_insert(&Lm, aSize, memPtr, aSrcLineNum, aSrcFileName, aSrcFuncName, EMalloc);
	else
		fprintf(CHECKER_OUT, "Pri alokaci pameti nastala chyba !\n"
								"---------------------------------\n"
								"Chyba pri volani funkce void* malloc(size_t)\n"
				);
	return memPtr;
	}

void* checker_calloc(size_t aCount, size_t aSize, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	void *memPtr = CHECKER_NULL;

	if(Used_alloc == CHECKER_FALSE)
		{
		Used_alloc = CHECKER_TRUE;
		allocc_init_list(&Lm);
		if(Used_file_c == CHECKER_FALSE)
			atexit(&checkdel);
		}

	if((!Lm.iAlloc_lock_key) || (Lm.iCounter_alloc < Lm.iCounter_alloc_lock_limit))
		memPtr = (void *) calloc(aCount, aSize);

	if(memPtr)
		allocc_insert(&Lm, aSize*aCount, memPtr, aSrcLineNum, aSrcFileName, aSrcFuncName, ECalloc);
	else
		fprintf(CHECKER_OUT, "Pri alokaci pameti nastala chyba !\n"
								"------------------------------\n"
								"Chyba pri volani funkce void* calloc(size_t, size_t)\n"
				);

	return memPtr;
	}

void* checker_realloc(void* aReallocPtr, size_t aSize, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	void *newPtr = CHECKER_NULL;

	if(aSize == 0)		// tzn. jiny zapis funkce free
		{
		allocc_set_act(&Lm, aReallocPtr);

		if(allocc_active(&Lm))		// uvolnuji korektne pridelenou pamet
			{
#ifdef __cplusplus
			if( (Lm.iAct->iAllocType == ENew) || (Lm.iAct->iAllocType == ENewA) || (Lm.iAct->iAllocType == ENewNothrow) || (Lm.iAct->iAllocType == ENewNothrowA) )
				{
				fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\nFunkci realloc() kompletne uvolnujete pamet: %p, ktera byla alokovana operatorem %s(). Spravne by mela byt uvolnena operatorem delete%s(). Pozor, pamet nebyla uvolnena.\n\n", aReallocPtr, alloc_type_str[Lm.iAct->iAllocType], ( (Lm.iAct->iAllocType == ENewA) || (Lm.iAct->iAllocType == ENewNothrowA) )? ("[]") : (""));
				fprintf(CHECKER_OUT, "------------------------------\n"
						"Lokace varovani (odalokace adresy: %p):\n"
						"Zdrojovy soubor:radek: %s:%u\n", aReallocPtr, aSrcFileName, aSrcLineNum);
				fprintf(CHECKER_OUT, "Volani funkce: %s()\n", aSrcFuncName);
				fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: %u\n", Lm.iCounter_dealloc);
				return aReallocPtr;
				}
#endif
			allocc_dispose_act(&Lm);
			free(aReallocPtr);
			}
		else					// uvolnuji pamet, ktera mi nebyla pridelena
			{
			fprintf(CHECKER_OUT, "\n\nCHECKER: CHYBA\n------------------------------\n");
			fprintf(CHECKER_OUT, "Volanim funkce\nvoid realloc(void*, size_t)\nse pokousite uvolnit pamet na adrese: %p, ktera Vam nebyla pridelena.\n", aReallocPtr);
			fprintf(CHECKER_OUT, "\n-------------------------------------\n"
					"Lokace chyby:\n"
					"Zdrojovy soubor:radek: %s:%u\n", aSrcFileName, aSrcLineNum);
			fprintf(CHECKER_OUT, "Volani funkce: %s()\n", aSrcFuncName);
			fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: %u\n", Lm.iCounter_dealloc);
			program_error();
			}
		}	// aSize == 0
	else				// budeme pracovat s pameti (alokovat / realokovat)
		{
		if(aReallocPtr == CHECKER_NULL) // tj. jiny zapis funkce malloc
			{
			if(Used_alloc == CHECKER_FALSE)		// jestlize jeste nebylo alokovano
				{
				Used_alloc = CHECKER_TRUE;
				allocc_init_list(&Lm);
				if(Used_file_c == CHECKER_FALSE)
					atexit(&checkdel);
				}

			if((!Lm.iAlloc_lock_key) || (Lm.iCounter_alloc < Lm.iCounter_alloc_lock_limit))
				newPtr = (void *) malloc(aSize);

			if(newPtr)
				allocc_insert(&Lm, aSize, newPtr, aSrcLineNum, aSrcFileName, aSrcFuncName, ERealloc);
			else
				fprintf(CHECKER_OUT, "CHECKER: Pri alokaci pameti nastala chyba !\n------------------------------\nChyba pri volani funkce void* realloc(void *, size_t,)");

			} // if(aReallocPtr == CHECKER_NULL)
		else				// volani funkce realloc
			{
			allocc_set_act(&Lm, aReallocPtr);
#ifdef __cplusplus
			if( (Lm.iAct->iAllocType == ENew) || (Lm.iAct->iAllocType == ENewA) || (Lm.iAct->iAllocType == ENewNothrow) || (Lm.iAct->iAllocType == ENewNothrowA) )
				{
				fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\nFunkci realloc() menite velikost pameti: %p, ktera byla alokovana operatorem %s(). Norma jazyka C++ tuto operaci nepovoluje. Spravne by mela byt uvolnena operatorem delete%s(). Pozor, pamet nebyla uvolnena ani zmenena jeji velikost.\n\n", aReallocPtr, alloc_type_str[Lm.iAct->iAllocType], ( (Lm.iAct->iAllocType == ENewA) || (Lm.iAct->iAllocType == ENewNothrowA) )? ("[]") : (""));
				fprintf(CHECKER_OUT, "------------------------------\n"
						"Lokace varovani (realokace adresy: %p):\n"
						"Zdrojovy soubor:radek: %s:%u\n", aReallocPtr, aSrcFileName, aSrcLineNum);
				fprintf(CHECKER_OUT, "Volani funkce: %s()\n", aSrcFuncName);
				fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: %u\n", Lm.iCounter_dealloc);
				return aReallocPtr;
				}
#endif
			if(!allocc_active(&Lm))
				{
				fprintf(CHECKER_OUT, "\n\nCHECKER: Chyba - prvni parametr funkce\n void* realloc(void*, size_t) \nzadavate ukazatel na misto v pameti, ktere vam nebylo prideleno ! !\n-----------------------------------------------------\n");
				return CHECKER_NULL;
				}

			if((!Lm.iRealloc_lock_key) || (Lm.iCounter_realloc < Lm.iCounter_realloc_lock_limit))
				newPtr = (void *) realloc(aReallocPtr, aSize);

			if(newPtr)
				allocc_resize_act(&Lm, aSize, newPtr, aSrcLineNum, aSrcFileName, aSrcFuncName, ERealloc);
			else
				fprintf(CHECKER_OUT, "CHECKER: Pri alokaci pameti nastala chyba !\n------------------------------\nChyba pri volani funkce void* realloc(void *, size_t,)");

			}	// else- if(aReallocPtr != CHECKER_NULL)
		return newPtr;
		}	// else- if(aSize > 0)
	return CHECKER_NULL;
	}

void checker_free(void *aFreePtr, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	allocc_set_act(&Lm, aFreePtr);

	if(allocc_active(&Lm))
		{
#ifdef __cplusplus
		if( (Lm.iAct->iAllocType == ENew) || (Lm.iAct->iAllocType == ENewA) || (Lm.iAct->iAllocType == ENewNothrow) || (Lm.iAct->iAllocType == ENewNothrowA))
			{
			fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\nFunkci free() uvolnujete pamet: %p, ktera byla alokovana operatorem %s(). Spravne by mela byt uvolnena operatorem delete%s(). Pozor, pamet nebyla uvolnena.\n\n", aFreePtr, alloc_type_str[Lm.iAct->iAllocType], ( (Lm.iAct->iAllocType == ENewA) || (Lm.iAct->iAllocType == ENewNothrowA) )? ("[]") : (""));
			fprintf(CHECKER_OUT, "------------------------------\n"
								  "Lokace varovani (odalokace adresy: %p):\n"
								  "Zdrojovy soubor:radek: %s:%u\n", aFreePtr, aSrcFileName, aSrcLineNum);
			fprintf(CHECKER_OUT, "Volani funkce: %s()\n", aSrcFuncName);
			fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: %u\n", Lm.iCounter_dealloc);
			}
		else
			{
#endif
			allocc_dispose_act(&Lm);
			free(aFreePtr);
#ifdef __cplusplus
			}
#endif
		}
	else
		if(aFreePtr)
			{
			fprintf(CHECKER_OUT, "\n\nCHECKER: CHYBA\n------------------------------\n"
								 "Volanim funkce\nvoid free(void*)\nse pokousite uvolnit pamet na adrese: %p, ktera Vam nebyla pridelena.\n", aFreePtr);

			fprintf(CHECKER_OUT, "------------------------------\n"
								 "Lokace chyby:\n"
								  "Zdrojovy soubor:radek: %s:%u\n", aSrcFileName, aSrcLineNum);
			fprintf(CHECKER_OUT, "Volani funkce: %s()\n", aSrcFuncName);
			fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: %u\n", Lm.iCounter_dealloc);
			program_error();
			}
	}


/*******************************************************
*  IMPLEMENTACE ALOKACNI / DEALOKACNI OPERATORU - C++  *
*******************************************************/
#ifdef __cplusplus

bool checker_new_recorder(unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	size_t len = 0;
	if(Checker_new_record_full == CHECKER_TRUE)
		fprintf(CHECKER_OUT, "New record mailbox isn't empty:\nnew(%s:%u:%s)\n<-\nnew(%s:%u:%s)\n",
				Checker_new_soubor, Checker_new_line, Checker_new_func, aSrcFileName, aSrcLineNum, aSrcFuncName);
	Checker_new_line = aSrcLineNum;
	len = strlen(aSrcFileName) + 1;
	if(len > CHECKER_FILENAME_MAXLEN)
		CHECKER_STRNCPY_S(Checker_new_soubor, CHECKER_FILENAME_MAXLEN, aSrcFileName + len - CHECKER_FILENAME_MAXLEN, CHECKER_FILENAME_MAXLEN);
	else
		CHECKER_STRNCPY_S(Checker_new_soubor, CHECKER_FILENAME_MAXLEN, aSrcFileName, CHECKER_FILENAME_MAXLEN);

	CHECKER_STRNCPY_S(Checker_new_func, CHECKER_FUNCNAME_MAXLEN, aSrcFuncName, CHECKER_FUNCNAME_MAXLEN - 1);
	Checker_new_func[CHECKER_FUNCNAME_MAXLEN - 1] = '\0';
	Checker_new_record_full = CHECKER_TRUE;
	return false;
	}

void* operator new(size_t aSize) CHECKER_BAD_ALLOC_EXCEPTION
	{
	void *addr = CHECKER_NULL;

	if(Used_alloc == CHECKER_FALSE)
		{
		Used_alloc = CHECKER_TRUE;
		allocc_init_list(&Lm);
		if(Used_file_c == CHECKER_FALSE)
			atexit(&checkdel);
		}

	if((!Lm.iAlloc_lock_key) || (Lm.iCounter_alloc < Lm.iCounter_alloc_lock_limit))
		addr = (void *) malloc(aSize);

	if(addr)
		{
		if(Checker_new_record_full)
			allocc_insert(&Lm, aSize, addr, Checker_new_line, Checker_new_soubor, Checker_new_func, ENew);
		else
			allocc_insert(&Lm, aSize, addr, 0, CHECKER_HIDDEN_SOURCE_FILENAME, CHECKER_HIDDEN_FUNCNAME, ENew);
		}
	else
		{
		fprintf(CHECKER_OUT, "Pri alokaci pameti nastala chyba !\n---------------------------------\nChyba pri alokaci pomoci operatoru void* operator new(size_t)");
		throw std::bad_alloc();
		}
	Checker_new_record_full = CHECKER_FALSE;
	return addr;
	}

void* operator new(size_t aSize, const std::nothrow_t& CHECKER_UNUSED_PARAM(aNoThrow)) CHECKER_NO_EXCEPTION
	{
	void *addr = CHECKER_NULL;

	if(Used_alloc == CHECKER_FALSE)
		{
		Used_alloc = CHECKER_TRUE;
		allocc_init_list(&Lm);
		if(Used_file_c == CHECKER_FALSE)
			atexit(&checkdel);
		}

	if((!Lm.iAlloc_lock_key) || (Lm.iCounter_alloc < Lm.iCounter_alloc_lock_limit))
		addr = (void *) malloc(aSize);

	if(addr)
		{
		if(Checker_new_record_full)
			allocc_insert(&Lm, aSize, addr, Checker_new_line, Checker_new_soubor, Checker_new_func, ENewNothrow);
		else
			allocc_insert(&Lm, aSize, addr, 0, CHECKER_HIDDEN_SOURCE_FILENAME, CHECKER_HIDDEN_FUNCNAME, ENewNothrow);
		}
	else
		{
		fprintf(CHECKER_OUT, "Pri alokaci pameti nastala chyba !\n---------------------------------\nChyba pri alokaci pomoci operatoru void* operator new(size_t,nothrow)");
		}
	Checker_new_record_full = CHECKER_FALSE;
	return addr;
	}

void* operator new[](size_t aSize) CHECKER_BAD_ALLOC_EXCEPTION
	{
	void *addr = CHECKER_NULL;

	if(Used_alloc == CHECKER_FALSE)
		{
		Used_alloc = CHECKER_TRUE;
		allocc_init_list(&Lm);
		if(Used_file_c == CHECKER_FALSE)
			atexit(&checkdel);
		}

	if((!Lm.iAlloc_lock_key) || (Lm.iCounter_alloc < Lm.iCounter_alloc_lock_limit))
		addr = (void *) malloc(aSize);

	if(addr)
		{
		if(Checker_new_record_full)
			allocc_insert(&Lm, aSize, addr, Checker_new_line, Checker_new_soubor, Checker_new_func, ENewA);
		else
			allocc_insert(&Lm, aSize, addr, 0, CHECKER_HIDDEN_SOURCE_FILENAME, CHECKER_HIDDEN_FUNCNAME, ENewA);
		}
	else
		{
		fprintf(CHECKER_OUT, "Pri alokaci pameti nastala chyba !\n---------------------------------\nChyba pri alokaci pomoci operatoru void* operator new[](size_t)");
		throw std::bad_alloc();
		}
	Checker_new_record_full = CHECKER_FALSE;
	return addr;
	}

void* operator new[](size_t aSize, const std::nothrow_t& CHECKER_UNUSED_PARAM(aNoThrow)) CHECKER_NO_EXCEPTION
	{
	void *addr = CHECKER_NULL;

	if(Used_alloc == CHECKER_FALSE)
		{
		Used_alloc = CHECKER_TRUE;
		allocc_init_list(&Lm);
		if(Used_file_c == CHECKER_FALSE)
			atexit(&checkdel);
		}

	if((!Lm.iAlloc_lock_key) || (Lm.iCounter_alloc < Lm.iCounter_alloc_lock_limit))
		addr = (void *) malloc(aSize);

	if(addr)
		{
		if(Checker_new_record_full)
			allocc_insert(&Lm, aSize, addr, Checker_new_line, Checker_new_soubor, Checker_new_func, ENewNothrowA);
		else
			allocc_insert(&Lm, aSize, addr, 0, CHECKER_HIDDEN_SOURCE_FILENAME, CHECKER_HIDDEN_FUNCNAME, ENewNothrowA);
		}
	else
		{
		fprintf(CHECKER_OUT, "Pri alokaci pameti nastala chyba !\n---------------------------------\nChyba pri alokaci pomoci operatoru void* operator new[](size_t,nothrow)");
		}
	Checker_new_record_full = CHECKER_FALSE;
	return addr;
	}

CCheckerDeleteRecorder::~CCheckerDeleteRecorder()
	{
	if(Checker_delete_err_ptr)
		{
		fprintf(CHECKER_OUT, "------------------------------\n"
			"Lokace chyby (odalokace adresy: %p):\n"
			"Zdrojovy soubor:radek: %s:%u\n", Checker_delete_err_ptr, iSrcFileName, iSrcLineNum);
		fprintf(CHECKER_OUT, "Volani funkce: %s()\n", iSrcFuncName);
		fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: %u\n", Lm.iCounter_dealloc);
		}
	Checker_delete_err_ptr = CHECKER_NULL;
	}

void operator delete(void *aPtr) CHECKER_NO_EXCEPTION
	{
	if(Checker_delete_err_ptr)
		{
		fprintf(CHECKER_OUT, "------------------------------\n"
				"Lokace chyby (odalokace adresy: %p):\n"
				"Zdrojovy soubor:radek: %s:%u\n", Checker_delete_err_ptr, CHECKER_HIDDEN_SOURCE_FILENAME, 0);
		fprintf(CHECKER_OUT, "Volani funkce: %s()\n", CHECKER_HIDDEN_FUNCNAME);
		fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: %u\n", Lm.iCounter_dealloc);
		}
	Checker_delete_err_ptr = CHECKER_NULL;

	allocc_set_act(&Lm, aPtr);

	if(allocc_active(&Lm))
		{
		if( (Lm.iAct->iAllocType != ENew) && (Lm.iAct->iAllocType != ENewNothrow) )
			{
			fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\nOperatorem delete() uvolnujete pamet: %p, ktera byla alokovana pomoci %s(). Spravne by mela byt uvolnena pomoci %s().\nPozor, pamet nebyla uvolnena.\n",
				aPtr, alloc_type_str[Lm.iAct->iAllocType], ((Lm.iAct->iAllocType == ENewA) || (Lm.iAct->iAllocType == ENewNothrowA)) ? ("delete[]") : ("free") );
			Checker_delete_err_ptr = aPtr;
			}
		else
			{
			allocc_dispose_act(&Lm);
			free(aPtr);
			}
		}
	else
		if(aPtr)
			{
			fprintf(CHECKER_OUT, "\n\nCHECKER: CHYBA\n------------------------------\n"
					"Volanim operatoru \ndelete(void*)\nse pokousite uvolnit pamet na adrese: %p, ktera Vam nebyla pridelena.\n", aPtr);
			Checker_delete_err_ptr = aPtr;
			program_error();
			}
	}

void operator delete[](void *aPtr) CHECKER_NO_EXCEPTION
	{
	if(Checker_delete_err_ptr)
		{
		fprintf(CHECKER_OUT, "------------------------------\n"
				"Lokace chyby (odalokace adresy: %p):\n"
				"Zdrojovy soubor:radek: %s:%u\n", Checker_delete_err_ptr, CHECKER_HIDDEN_SOURCE_FILENAME, 0);
		fprintf(CHECKER_OUT, "Volani funkce: %s()\n", CHECKER_HIDDEN_FUNCNAME);
		fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: %u\n", Lm.iCounter_dealloc);
		}
	Checker_delete_err_ptr = CHECKER_NULL;

	allocc_set_act(&Lm, aPtr);

	if(allocc_active(&Lm))
		{
		if( (Lm.iAct->iAllocType != ENewA) && (Lm.iAct->iAllocType != ENewNothrowA) )
			{
			fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\nOperatorem delete[]() uvolnujete pamet: %p, ktera byla alokovana pomoci %s(). Spravne by mela byt uvolnena pomoci %s().\n",
				aPtr, alloc_type_str[Lm.iAct->iAllocType], ( (Lm.iAct->iAllocType == ENew) || (Lm.iAct->iAllocType == ENewNothrow)) ? ("delete") : ("free") );
			fprintf(CHECKER_OUT, "Alokace pameti byla puvodne provedena pomoci: %s() v souboru: %s:%u, ve funkci: %s().\nPozor, pamet nebyla uvolnena.\n",
				alloc_type_str[Lm.iAct->iAllocType], Lm.iAct->iSrcFileName, Lm.iAct->iSrcLineNum, Lm.iAct->iSrcFuncName);
			Checker_delete_err_ptr = aPtr;
			}
		else
			{
			allocc_dispose_act(&Lm);
			free(aPtr);
			}
		}
	else
		if(aPtr)
			{
			fprintf(CHECKER_OUT, "\n\nCHECKER: CHYBA\n------------------------------\n"
					"Volanim operatoru \ndelete[](void*)\nse pokousite uvolnit pamet na adrese: %p, ktera Vam nebyla pridelena.\n", aPtr);
			Checker_delete_err_ptr = aPtr;
			program_error();
			}
	}

#if (__cplusplus >= 201402L) || (_MSC_FULL_VER >= 190023918L)		/* C++14 global sized dealocation feature */

void operator delete(void *aPtr, size_t aSize) CHECKER_NO_EXCEPTION
	{
	if(Checker_delete_err_ptr)
		{
		fprintf(CHECKER_OUT, "------------------------------\n"
			"Lokace chyby (odalokace adresy: %p) (velikosti: " CHECKER_SIZE_T_PRIu " :\n"
			"Zdrojovy soubor:radek: %s:%u\n", Checker_delete_err_ptr, (CHECKER_SIZE_T_CONV)aSize, CHECKER_HIDDEN_SOURCE_FILENAME, 0);
		fprintf(CHECKER_OUT, "Volani funkce: %s()\n", CHECKER_HIDDEN_FUNCNAME);
		fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: %u\n", Lm.iCounter_dealloc);
		}
	Checker_delete_err_ptr = CHECKER_NULL;

	allocc_set_act(&Lm, aPtr);

	if(allocc_active(&Lm))
		{
		CHECKER_BOOL is_warning = CHECKER_FALSE;
		if((Lm.iAct->iAllocType != ENew) && (Lm.iAct->iAllocType != ENewNothrow))
			{
			fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\nOperatorem sized delete() uvolnujete na adrese: %p pamet, ktera byla alokovana pomoci %s(). Spravne by mela byt uvolnena pomoci %s().\n",
				aPtr, alloc_type_str[Lm.iAct->iAllocType], ((Lm.iAct->iAllocType == ENewA) || (Lm.iAct->iAllocType == ENewNothrowA)) ? ("delete[]") : ("free"));
			Checker_delete_err_ptr = aPtr;
			is_warning = CHECKER_TRUE;
			}
		if(Lm.iAct->iMemSize != aSize)
			{
			if(!is_warning)
				fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\n");
			fprintf(CHECKER_OUT, "Operatorem sized delete() uvolnujete na adrese: %p, blok pameti o velikosti: " CHECKER_SIZE_T_PRIu ". Ovsem puvodni alokovana velikost bloku byla: " CHECKER_SIZE_T_PRIu "!\n",
				aPtr, (CHECKER_SIZE_T_CONV)aSize, (CHECKER_SIZE_T_CONV)Lm.iAct->iMemSize);
			Checker_delete_err_ptr = aPtr;
			is_warning = CHECKER_TRUE;
			}
		if(is_warning)
			fprintf(CHECKER_OUT, "Alokace pameti byla puvodne provedena pomoci: %s() v souboru: %s:%u, ve funkci: %s().\n" "Pozor, pamet nebyla uvolnena.\n",
				alloc_type_str[Lm.iAct->iAllocType], Lm.iAct->iSrcFileName, Lm.iAct->iSrcLineNum, Lm.iAct->iSrcFuncName);
		else
			{
			allocc_dispose_act(&Lm);
			free(aPtr);
			}
		}
	else
		if(aPtr)
			{
			fprintf(CHECKER_OUT, "\n\nCHECKER: CHYBA\n------------------------------\n"
				"Volanim operatoru\nsized delete(void*)\nse pokousite uvolnit pamet na adrese: %p, ktera Vam nebyla pridelena.\n", aPtr);
			Checker_delete_err_ptr = aPtr;
			program_error();
			}
	}

void operator delete[](void *aPtr, size_t aSize) CHECKER_NO_EXCEPTION
	{
	if(Checker_delete_err_ptr)
		{
		fprintf(CHECKER_OUT, "------------------------------\n"
				"Lokace chyby (odalokace adresy: %p) (velikosti: " CHECKER_SIZE_T_PRIu " :\n"
				"Zdrojovy soubor:radek: %s:%u\n", Checker_delete_err_ptr, (CHECKER_SIZE_T_CONV)aSize, CHECKER_HIDDEN_SOURCE_FILENAME, 0);
		fprintf(CHECKER_OUT, "Volani funkce: %s()\n", CHECKER_HIDDEN_FUNCNAME);
		fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: %u\n", Lm.iCounter_dealloc);
		}
	Checker_delete_err_ptr = CHECKER_NULL;

	allocc_set_act(&Lm, aPtr);

	if(allocc_active(&Lm))
		{
		CHECKER_BOOL is_warning = CHECKER_FALSE;
		if( (Lm.iAct->iAllocType != ENewA) && (Lm.iAct->iAllocType != ENewNothrowA) )
			{
			fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\nOperatorem sized delete[]() uvolnujete na adrese: %p pamet, ktera byla alokovana pomoci %s(). Spravne by mela byt uvolnena pomoci %s().\n",
				aPtr, alloc_type_str[Lm.iAct->iAllocType], ( (Lm.iAct->iAllocType == ENew) || (Lm.iAct->iAllocType == ENewNothrow)) ? ("delete") : ("free") );
			Checker_delete_err_ptr = aPtr;
			is_warning = CHECKER_TRUE;
			}
		if(Lm.iAct->iMemSize != aSize)
			{
			if(!is_warning)
				fprintf(CHECKER_OUT, "\n\nCHECKER: VAROVANI\n");
			fprintf(CHECKER_OUT, "Operatorem sized delete[]() uvolnujete na adrese: %p, blok pameti o velikosti: " CHECKER_SIZE_T_PRIu ". Ovsem puvodni alokovana velikost bloku byla: " CHECKER_SIZE_T_PRIu "!\n",
				aPtr, (CHECKER_SIZE_T_CONV)aSize, (CHECKER_SIZE_T_CONV)Lm.iAct->iMemSize);
			if(!is_warning)
				fprintf(CHECKER_OUT, "Alokace pameti byla puvodne provedena pomoci: %s() v souboru: %s:%u, ve funkci: %s().\n",
					alloc_type_str[Lm.iAct->iAllocType], Lm.iAct->iSrcFileName, Lm.iAct->iSrcLineNum, Lm.iAct->iSrcFuncName);
			Checker_delete_err_ptr = aPtr;
			is_warning = CHECKER_TRUE;
			}
		if(is_warning)
			fprintf(CHECKER_OUT, "Alokace pameti byla puvodne provedena pomoci: %s() v souboru: %s:%u, ve funkci: %s().\n" "Pozor, pamet nebyla uvolnena.\n",
				alloc_type_str[Lm.iAct->iAllocType], Lm.iAct->iSrcFileName, Lm.iAct->iSrcLineNum, Lm.iAct->iSrcFuncName);
		else
			{
			allocc_dispose_act(&Lm);
			free(aPtr);
			}
		}
	else
		if(aPtr)
			{
			fprintf(CHECKER_OUT, "\n\nCHECKER: CHYBA\n------------------------------\n"
					"Volanim operatoru \nsized delete[](void*)\nse pokousite uvolnit pamet na adrese: %p, ktera Vam nebyla pridelena.\n", aPtr);
			Checker_delete_err_ptr = aPtr;
			program_error();
			}
	}

#endif /* C++14 */

#endif /* __cplusplus */

/***************************************
*  IMPLEMENTACE SOUBOROVYCH FUNKCI - C *
****************************************/
FILE* checker_fopen(const char aPath[CHECKER_PATHNAME_MAXLEN], const char aMode[CHECKER_FILEMODE_MAXLEN], unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	FILE *filePtr = CHECKER_NULL;
	if(Used_file_c == CHECKER_FALSE)
		{
		Used_file_c = CHECKER_TRUE;
		filec_init_list(&Lf);
		if(Used_alloc == CHECKER_FALSE)
			atexit(&checkdel);
		}

	if((aPath == CHECKER_NULL) || (aMode == CHECKER_NULL))
		{
		fprintf(CHECKER_OUT, "CHECKER: CHYBA\n------------------------------\n"
							 "Pri volanim funkce: fopen() vznikly tyto problemy:\n");
		if(aPath == CHECKER_NULL)
			fprintf(CHECKER_OUT, "Ukazatel na nazev souboru obsahuje NULL !\n");
		if(aMode == CHECKER_NULL)
			fprintf(CHECKER_OUT, "Ukazatel na mod otevreni souboru obsahuje NULL !\n");
		fprintf(CHECKER_OUT, "Soubor nemohl byt otevren\n\n");
		printchar(0, CHECKER_PAGE_WIDTH, '-');
		fprintf(CHECKER_OUT, "\nLokace chyby:\n"
							 "Zdrojovy soubor:radek: %s:%u\n", aSrcFileName, aSrcLineNum);
		fprintf(CHECKER_OUT, "Volani funkce: %s()\n", aSrcFuncName);
		program_error();
		}

#ifdef CHECKER_SECURE_SUPPORT
	/* When using fopen_s or freopen_s, file access permissions for any file created with "w" or "a" prevents other users from accessing it.
	 * File access mode flag "u" can optionally be prepended to any specifier that begins with "w" or "a", to enable the default fopen permissions. (C11) */
	errno_t err = fopen_s(&filePtr, aPath, aMode);
	if((!err) && (filePtr != CHECKER_NULL))
#else
	if((filePtr = fopen(aPath, aMode)) != CHECKER_NULL)
#endif
		{
		filec_insert(&Lf, aPath, aMode, aSrcLineNum, aSrcFileName, aSrcFuncName, filePtr);
		Lf.iCounter_fopen++;
		}
	else
		{
		fprintf(CHECKER_OUT, "CHECKER: VAROVANI\n------------------------------\n"
							 "Soubor %s se nepodarilo otevrit\n\n", aPath);
		printchar(0, CHECKER_PAGE_WIDTH, '-');
		fprintf(CHECKER_OUT, "\nLokace varovani:\n"
							 "Zdrojovy soubor:radek: %s:%u\n", aSrcFileName, aSrcLineNum);
		fprintf(CHECKER_OUT, "Volani funkce: %s()\n", aSrcFuncName);
		}
	return filePtr;
	}

int checker_fclose(FILE *aStreamPtr, unsigned int aSrcLineNum, const char aSrcFileName[CHECKER_FILENAME_MAXLEN], const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN])
	{
	int succ = 0;

	filec_set_act(&Lf, aStreamPtr);

	if(filec_active(&Lf))
		{
		Lf.iCounter_fclose++;
		succ = fclose(aStreamPtr);
		filec_dispose_act(&Lf);
		}
	else
		{
		fprintf(CHECKER_OUT, "\n\nCHECKER: CHYBA\n------------------------------\n"
							 "Volanim funkce\nint fclose(FILE*)\nse pokousite uzavrit soubor, ktery nebyl otevren\n\n");
		printchar(0, CHECKER_PAGE_WIDTH, '-');
		fprintf(CHECKER_OUT, "\nLokace chyby:\n"
							 "Zdrojovy soubor:radek: %s:%u\n", aSrcFileName, aSrcLineNum);
		fprintf(CHECKER_OUT, "Volani funkce: %s()\n", aSrcFuncName);
		program_error();
		}
	return succ;
	}


/*********************************************************/
/*                  PRACE SE SEZNAMEM                    */
/*********************************************************/

void checkdel(void)
	{
	/*
	tato funkce je pridavana do funkce exit
	v pripade, ze v programu nenastane fatalni chyba (uvolneni nealokovane pameti apod.),
	vypise statistiky programu pomoci print_list() a nasledne uvolni neuvolnenou pamet
	a uzavre neuzavrene soubory pomoci funkce dispose_lists()
	*/
#ifdef __cplusplus
	if(Checker_delete_err_ptr)
		{
		fprintf(CHECKER_OUT, "------------------------------\n"
				"Lokace chyby (odalokace adresy: %p):\n"
				"Zdrojovy soubor:radek: %s:%u\n", Checker_delete_err_ptr, CHECKER_HIDDEN_SOURCE_FILENAME, 0);
		fprintf(CHECKER_OUT, "Volani funkce: %s()\n", CHECKER_HIDDEN_FUNCNAME);
		fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: %u\n", Lm.iCounter_dealloc);
		}
	Checker_delete_err_ptr = CHECKER_NULL;
#endif /* __cplusplus */

	if(Correct_exit == CHECKER_TRUE)
		print_lists(&Lm, &Lf, CHECKER_TRUE, CHECKER_TRUE);
	dispose_lists(&Lm, &Lf);
	}

void allocc_init_list(struct TListMem *aLm)
	{
	/* inicializuje seznam pro alokace*/

	aLm->iAct = CHECKER_NULL;
	aLm->iFirst = CHECKER_NULL;
	aLm->iCounter_alloc = 0;
	aLm->iCounter_realloc = 0;
	aLm->iCounter_dealloc = 0;
	aLm->iStat_alloc_size_max_peak = 0;
	aLm->iStat_alloc_size = 0;
	aLm->iStat_free_size = 0;
	}

void filec_init_list(struct TListFile *aLf)
	{
	/* inicializuje seznam pro soubory*/

	aLf->iAct = CHECKER_NULL;
	aLf->iFirst = CHECKER_NULL;
	aLf->iCounter_fopen = 0;
	aLf->iCounter_fclose = 0;
	}

void allocc_insert(
		struct TListMem *aLm,
		size_t aSize,
		void *aAddress,
		unsigned int aSrcLineNum,
		const char aSrcFileName[CHECKER_FILENAME_MAXLEN],
		const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN],
		enum TAllocType aAllocType)
	{
	/* vlozi na konec seznamu prvek se zadanymi parametry
	   nastavi vlozeny prvek (tzn. posledni) jako aktualni
	*/

	size_t len;
	struct TNodeMem *insPtr = (struct TNodeMem *) malloc(sizeof(struct TNodeMem));

	if(!insPtr)
		program_error();
	else
		{
		insPtr->iMemSize = aSize;			// alokovana velikost
		insPtr->iSrcLineNum = aSrcLineNum;			// radek zdrojoveho kodu (makro __LINE__)

		len = strlen(aSrcFileName) + 1;
		if(len > CHECKER_FILENAME_MAXLEN )
			CHECKER_STRNCPY_S(insPtr->iSrcFileName, CHECKER_FILENAME_MAXLEN, aSrcFileName + len - CHECKER_FILENAME_MAXLEN, CHECKER_FILENAME_MAXLEN);
		else
			CHECKER_STRNCPY_S(insPtr->iSrcFileName, CHECKER_FILENAME_MAXLEN, aSrcFileName, CHECKER_FILENAME_MAXLEN);

		insPtr->iAllocNum = aLm->iCounter_alloc + 1;	// kolikate volani mallocu to je
		insPtr->iMemAddress = aAddress;	// adresa alokovane pameti
		CHECKER_STRNCPY_S(insPtr->iSrcFuncName, CHECKER_FUNCNAME_MAXLEN, aSrcFuncName, CHECKER_FUNCNAME_MAXLEN - 1);		// jmeno nadrazene funkce (makro CHECKER_FUNCNAME)
		insPtr->iSrcFuncName[CHECKER_FUNCNAME_MAXLEN - 1] = '\0';

		insPtr->iAllocType = aAllocType;

		insPtr->iNext = CHECKER_NULL;

		// vlozeni do seznamu
		if(aLm->iFirst)
			{
			// vlozeni na konec
			allocc_set_last_act(aLm);
			aLm->iAct->iNext = insPtr;
			aLm->iAct = insPtr;
			}
		else
			{
			aLm->iFirst = insPtr;
			aLm->iAct = insPtr;
			}
		}

	/* aktualizace statistik */
	aLm->iCounter_alloc++;
	aLm->iStat_alloc_size += aSize;
	if(aLm->iStat_alloc_size_max_peak < (aLm->iStat_alloc_size - aLm->iStat_free_size))
		aLm->iStat_alloc_size_max_peak = (aLm->iStat_alloc_size - aLm->iStat_free_size);
	}

void filec_insert(
		struct TListFile *aLf,
		const char aPath[CHECKER_PATHNAME_MAXLEN],
		const char aMode[CHECKER_FILEMODE_MAXLEN],
		unsigned int aSrcLineNum,
		const char aSrcFileName[CHECKER_FILENAME_MAXLEN],
		const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN],
		FILE *aStream)
	{
	/*
	vlozi na konec seznamu prvek se zadanymi parametry
	nastavi vlozeny prvek (tzn. posledni) jako aktualni
	*/
	struct TNodeFile *insPtr = (struct TNodeFile *) malloc(sizeof(struct TNodeFile));

	if(!insPtr)
		program_error();
	else
		{
		size_t len = strlen(aSrcFileName) + 1;		// jmeno souboru (makro __FILE__)
		if(len > CHECKER_FILENAME_MAXLEN )
			CHECKER_STRNCPY_S(insPtr->iSrcFileName, CHECKER_FILENAME_MAXLEN, aSrcFileName + len - CHECKER_FILENAME_MAXLEN, CHECKER_FILENAME_MAXLEN);
		else
			CHECKER_STRNCPY_S(insPtr->iSrcFileName, CHECKER_FILENAME_MAXLEN, aSrcFileName, CHECKER_FILENAME_MAXLEN);

		CHECKER_STRNCPY_S(insPtr->iSrcFuncName, CHECKER_FUNCNAME_MAXLEN, aSrcFuncName, CHECKER_FUNCNAME_MAXLEN - 1); // jmeno nadrazene funkce (makro CHECKER_FUNCNAME)
		insPtr->iSrcFuncName[CHECKER_FUNCNAME_MAXLEN - 1] = '\0';

		insPtr->iSrcLineNum = aSrcLineNum;				// radek zdrojoveho kodu (makro __LINE__)

		len = strlen(aPath) + 1; 		// jmeno oteviraneho souboru
		if(len > CHECKER_PATHNAME_MAXLEN )
			CHECKER_STRNCPY_S(insPtr->iPath, CHECKER_PATHNAME_MAXLEN, aPath + len - CHECKER_PATHNAME_MAXLEN, CHECKER_PATHNAME_MAXLEN);
		else
			CHECKER_STRNCPY_S(insPtr->iPath, CHECKER_PATHNAME_MAXLEN, aPath, CHECKER_PATHNAME_MAXLEN);

		CHECKER_STRNCPY_S(insPtr->iMode, CHECKER_FILEMODE_MAXLEN, aMode, CHECKER_FILEMODE_MAXLEN - 1);	// mod otvirani souboru
		insPtr->iMode[CHECKER_FILEMODE_MAXLEN - 1] = '\0';

		insPtr->iStreamAddress = aStream;				// adresa streamu

		insPtr->iNext = CHECKER_NULL;

		filec_set_last_act(aLf);			// vlozeni do seznamu
		if(aLf->iAct != CHECKER_NULL)
			{
			aLf->iAct->iNext = insPtr;
			aLf->iAct = insPtr;
			}
		else
			{
			aLf->iFirst = insPtr;
			aLf->iAct = insPtr;
			}
		}
	}

void allocc_resize_act(
		struct TListMem *aLm,
		size_t aSize,
		void *aAddress,
		unsigned int aSrcLineNum,
		const char aSrcFileName[CHECKER_FILENAME_MAXLEN],
		const char aSrcFuncName[CHECKER_FUNCNAME_MAXLEN],
		enum TAllocType aAllocType)
	{
	/*
	Aktualni prvek je naplnen novymi parametry vyuzivano pro funkci realloc()
	ponecha aktivitu na zvolenem prvku
	*/
	size_t len;
	struct TNodeMem *insPtr = aLm->iAct;

	if(allocc_active(aLm))
		{
		/* aktualizace statistik */
		aLm->iCounter_realloc++;
		aLm->iStat_free_size  += aLm->iAct->iMemSize;
		aLm->iStat_alloc_size += aSize;
		if(aLm->iStat_alloc_size_max_peak < (aLm->iStat_alloc_size - aLm->iStat_free_size))
			aLm->iStat_alloc_size_max_peak = (aLm->iStat_alloc_size - aLm->iStat_free_size);

		insPtr->iMemSize = aSize;			// nove alokovana velikost
		insPtr->iSrcLineNum = aSrcLineNum;			// radek zdrojoveho kodu (makro __LINE__)

		len = strlen(aSrcFileName) + 1;								// jmeno souboru (makro __FILE__)
		if(len > CHECKER_FILENAME_MAXLEN)
			CHECKER_STRNCPY_S(insPtr->iSrcFileName, CHECKER_FILENAME_MAXLEN, aSrcFileName + len - CHECKER_FILENAME_MAXLEN, CHECKER_FILENAME_MAXLEN);
		else
			CHECKER_STRNCPY_S(insPtr->iSrcFileName, CHECKER_FILENAME_MAXLEN, aSrcFileName, CHECKER_FILENAME_MAXLEN);

		insPtr->iAllocNum = aLm->iCounter_alloc + 1;	// kolikate volani mallocu to je
		insPtr->iMemAddress = aAddress;	// adresa alokovane pameti
		CHECKER_STRNCPY_S(insPtr->iSrcFuncName, CHECKER_FUNCNAME_MAXLEN, aSrcFuncName, CHECKER_FUNCNAME_MAXLEN - 1);		// jmeno nadrazene funkce (makro CHECKER_FUNCNAME)
		insPtr->iSrcFuncName[CHECKER_FUNCNAME_MAXLEN - 1] = '\0';

		insPtr->iAllocType = aAllocType;
		}
	else
		program_error();

	}

void allocc_dispose_act(struct TListMem *aLm)
	{
	/*
	zrusi aktivni prvek.
	Vazba z predchoziho se navaze na nasledujici prvek seznamu
	nastavi aktivitu na prvni prvek
	*/
	if(allocc_active(aLm))
		{
		/* aktualizace statistik */
		aLm->iCounter_dealloc++;
		aLm->iStat_free_size += aLm->iAct->iMemSize;

		if(aLm->iAct == aLm->iFirst)	// jestli-ze je prvni
			aLm->iFirst = aLm->iAct->iNext;
		else
			{						// jestli neni prvni, resim vazby
			struct TNodeMem *tmp = aLm->iFirst;

			while(tmp->iNext != aLm->iAct)
				tmp = tmp->iNext;

			tmp->iNext = aLm->iAct->iNext;
			}

		free(aLm->iAct);
		aLm->iAct = aLm->iFirst;
		}
	else
		program_error();

	}

void filec_dispose_act(struct TListFile *aLf)
	{
	/*
	zrusi aktivni prvek.
	Vazba z predchoziho se navaze na nasledujici prvek seznamu
	nastavi aktivitu na prvni prvek
	*/
	if(filec_active(aLf))
		{
		if(aLf->iAct == aLf->iFirst)  // jestli-ze je prvni
			aLf->iFirst = aLf->iAct->iNext;
		else
			{						// jestli neni prvni, resim vazby
			struct TNodeFile *tmp = aLf->iFirst;

			while(tmp->iNext != aLf->iAct)
				tmp = tmp->iNext;

			tmp->iNext = aLf->iAct->iNext;
			}

		free(aLf->iAct);
		aLf->iAct = aLf->iFirst;
		}
	}

void allocc_set_last_act(struct TListMem *aLm)
	{
	/*
	nastavi jako aktualni prvek posledni prvek
	jestli-ze nastane chyba, nastavi CHECKER_NULL
	*/
	if(aLm->iFirst)
		{
		if(!allocc_active(aLm)) aLm->iAct = aLm->iFirst;
		while(aLm->iAct->iNext) aLm->iAct = aLm->iAct->iNext;
		}
	else
		aLm->iAct = CHECKER_NULL;
	}

void filec_set_last_act(struct TListFile *aLf)
	{
	/*
	nastavi jako aktualni prvek posledni prvek
	jestli-ze nastane chyba, nastavi CHECKER_NULL
	*/
	if(aLf->iFirst)
		{
		if(!filec_active(aLf))
			aLf->iAct = aLf->iFirst;
		while(aLf->iAct->iNext)
			aLf->iAct = aLf->iAct->iNext;
		}
	else
		aLf->iAct = CHECKER_NULL;
	}

void allocc_set_act(struct TListMem *aLm, void *aAddress)
	{
	/*
	najde prvek se zadanou adresou. V pripade,
	ze takovy prvek nenajde, nastavi aLm->iAct = CHECKER_NULL
	*/
	for(aLm->iAct = aLm->iFirst; (aLm->iAct) && (aLm->iAct->iMemAddress != aAddress); aLm->iAct = aLm->iAct->iNext);
	}

void filec_set_act(struct TListFile *aLf, FILE *aStream)
	{
	/*
	najde prvek se zadanou adresou. V pripade,
	ze takovy prvek nenajde, nastavi aLf->iAct = CHECKER_NULL
	*/
	for(aLf->iAct = aLf->iFirst; (aLf->iAct) && (aLf->iAct->iStreamAddress != aStream); aLf->iAct = aLf->iAct->iNext);
	}

CHECKER_BOOL allocc_active(const struct TListMem *aLm)
	{
	/* jestli-ze je seznam aktivni, vraci 1, jinak 0 */
	return (aLm->iAct) ? (CHECKER_TRUE) : (CHECKER_FALSE);
	}

CHECKER_BOOL filec_active(const struct TListFile *aLf)
	{
	/* jestli-ze je seznam aktivni, vraci 1, jinak 0 */
	return (aLf->iAct) ? (CHECKER_TRUE) : (CHECKER_FALSE);
	}

void dispose_lists(struct TListMem* aLm, struct TListFile *aLf)
	{
	struct TNodeMem  *tmp_m = CHECKER_NULL;
	struct TNodeFile *tmp_f = CHECKER_NULL;

	while(aLm->iFirst)
		{
		tmp_m = aLm->iFirst;
		aLm->iFirst = aLm->iFirst->iNext;
		free(tmp_m->iMemAddress); 	// uvolnim pamet alokovanou v programu
		free(tmp_m);			// zrusim zaznam ze seznamu
		}
	aLm->iFirst = CHECKER_NULL;
	aLm->iAct = CHECKER_NULL;

	while(aLf->iFirst)
		{
		tmp_f = aLf->iFirst;
		aLf->iFirst = aLf->iFirst->iNext;
		fclose(tmp_f->iStreamAddress);	// uzavru soubor
		free(tmp_f);			// zrusim zaznam ze seznamu
		}
	aLf->iFirst = CHECKER_NULL;
	aLf->iAct = CHECKER_NULL;
	}

void program_error(void)
	{
	fprintf(CHECKER_OUT, "\nProgram musel byt ukoncen\n");
	fflush(CHECKER_OUT);
	Correct_exit = CHECKER_FALSE;
	exit(EXIT_FAILURE);
	}

void allocc_print_list(struct TListMem *aLm)
	{
	size_t a = 0, b = 0, c = 0, e = 0, g = 0, h = 0;
	size_t mezer_mezi_sloupci, x = 0;
	size_t pflen = 0; /* printf length */

	fprintf(CHECKER_OUT, "\n\nCHECKER: KONTROLNI VYPIS \n");
	printchar(0, CHECKER_PAGE_WIDTH, '=');
	putc('\n', CHECKER_OUT);

	if(!Used_alloc)
		{
		fprintf(CHECKER_OUT, "\nZadna pamet nebyla dynamicky alokovana\n\n");
		return;
		}

	fprintf(CHECKER_OUT, "VYPIS ALOKOVANE PAMETI\n");
	fprintf(CHECKER_OUT, "Celkovy pocet alokaci pameti v programu: \t%u\n", aLm->iCounter_alloc);
	if(aLm->iCounter_realloc)
		fprintf(CHECKER_OUT, "Celkovy pocet realokaci pameti v programu: \t%u\n", aLm->iCounter_realloc);

	fprintf(CHECKER_OUT, "Celkovy pocet uspesnych uvolneni pameti v programu: \t%u\n", aLm->iCounter_dealloc);

	fprintf(CHECKER_OUT, "Spickove mnozstvi alokovane pameti pri behu programu:  " CHECKER_SIZE_T_PRIu "  bytu.\n", (CHECKER_SIZE_T_CONV)aLm->iStat_alloc_size_max_peak);
	fprintf(CHECKER_OUT, "Alokovano celkem  " CHECKER_SIZE_T_PRIu "  bytu, uvolneno  " CHECKER_SIZE_T_PRIu "  bytu.\n\n", (CHECKER_SIZE_T_CONV)aLm->iStat_alloc_size, (CHECKER_SIZE_T_CONV)aLm->iStat_free_size);

	if(!aLm->iFirst)
		{
		fprintf(CHECKER_OUT, "Vsechna dynamicka pamet programu byla bez chyby uvolnena. :-)\n\n");
		return;
		}

	fprintf(CHECKER_OUT, "NEODALOKOVANO ZUSTAVA  " CHECKER_SIZE_T_PRId "  BYTU !!!\n\n", (CHECKER_SIZE_T_CONV)aLm->iStat_alloc_size - (CHECKER_SIZE_T_CONV)aLm->iStat_free_size);

	/* ziskam delky nejdelsich udaju */
	aLm->iAct = aLm->iFirst;
	while(aLm->iAct)
		{
		#ifdef CHECKER_TAB_INDEX
		/* poradi alokace */
			x = chars_in_dec(aLm->iAct->iAllocNum);
			if(x > a) a = x;
		#endif

		#ifdef CHECKER_TAB_FILE_LINENUM
		/* filename */
			x = CHECKER_STRNLEN_S(aLm->iAct->iSrcFileName, CHECKER_FILENAME_MAXLEN);
			if(x > b) b = x;
		/* line number starting from 0!!!!*/
			x = chars_in_dec(aLm->iAct->iSrcLineNum);
			if(x > c) c = x;
		#endif

		#ifdef CHECKER_TAB_FUNCTION
		/* function name */
			x = CHECKER_STRNLEN_S(aLm->iAct->iSrcFuncName, CHECKER_FUNCNAME_MAXLEN);
		/* allocation type (malloc/new/realloc) */
			x += CHECKER_STRNLEN_S(alloc_type_str[aLm->iAct->iAllocType % ECount], CHECKER_MAXLENGTH_ALLOC_TYPE_STR);
			if(x > h) h = x;
		#endif

		#ifdef CHECKER_TAB_SIZE
		/* allocated memory block size */
			x = chars_in_dec(aLm->iAct->iMemSize);
			if(x > e) e = x;
		#endif

		#ifdef CHECKER_TAB_MEM_ADDR
		/* Address can be 0!!!! (So, addr + 1) */
			x = 1 + 4; /* pro pomlcku a dvakrat znaky 0x */
			x += chars_in_hex((ptrdiff_t) aLm->iAct->iMemAddress);
			x += chars_in_hex((ptrdiff_t) ((char*)aLm->iAct->iMemAddress + aLm->iAct->iMemSize) );
			if(x > g) g = x;
		#endif

		aLm->iAct = aLm->iAct->iNext;
		} /* while(aLm->iAct) */

	x = a + b + c + e + h + g + 6;				/* prizpusobeni sirky tabulky */
	if(x < 35) mezer_mezi_sloupci = 8;
	else if(x < 40) mezer_mezi_sloupci = 7;
	else if(x < 45) mezer_mezi_sloupci = 6;
	else if(x < 50) mezer_mezi_sloupci = 5;
	else if(x < 55) mezer_mezi_sloupci = 4;
	else if(x < 60) mezer_mezi_sloupci = 3;
	else mezer_mezi_sloupci = 2;


	/* Zahlavi tabulky */
	fprintf(CHECKER_OUT, "VYPIS ALOKOVANE PAMETI\n");

	#ifdef CHECKER_TAB_INDEX
	/* Poradi alokace */
		pflen = fprintf(CHECKER_OUT, "#");
		if(a < pflen)
			a = pflen;
		a+= mezer_mezi_sloupci;
		printchar(pflen, a, ' ');
	#endif

	#ifdef CHECKER_TAB_FILE_LINENUM
	/* Soubor:Radek */
		pflen = fprintf(CHECKER_OUT, "Soubor:Radek");
		if(b + c + 1 < pflen)
			b = pflen;
		else
			b += c + 1;

		b += mezer_mezi_sloupci;
		printchar(pflen, b, ' ');
	#endif

	#ifdef CHECKER_TAB_FUNCTION
	/* Funkce */
		pflen = fprintf(CHECKER_OUT, "Funkce");

		if(h + 3 + 2 < pflen)
			h = pflen;
		else
			h += 3 + 2;

		h += mezer_mezi_sloupci;
		printchar(pflen, h, ' ');
	#endif

	#ifdef CHECKER_TAB_SIZE
	/* allocated memblock size */
		pflen = fprintf(CHECKER_OUT, "B");
		if(e < pflen)
			e = pflen;
		e += mezer_mezi_sloupci;
		printchar(pflen, e, ' ');
	#endif

	#ifdef CHECKER_TAB_MEM_ADDR
		pflen = fprintf(CHECKER_OUT, "Adresa pameti");
		if(g < pflen)
			g = pflen;
		printchar(pflen, g, ' ');
	#endif

	/* Title break line */
	putc('\n', CHECKER_OUT);
	#ifdef CHECKER_TAB_INDEX
		printchar(mezer_mezi_sloupci, a, '-');
		printchar(0, mezer_mezi_sloupci, ' ');
	#endif
	#ifdef CHECKER_TAB_FILE_LINENUM
		printchar(mezer_mezi_sloupci, b, '-');
		printchar(0, mezer_mezi_sloupci, ' ');
	#endif
	#ifdef CHECKER_TAB_FUNCTION
		printchar(mezer_mezi_sloupci, h, '-');
		printchar(0, mezer_mezi_sloupci, ' ');
	#endif
	#ifdef CHECKER_TAB_SIZE
		printchar(mezer_mezi_sloupci, e, '-');
		printchar(0, mezer_mezi_sloupci, ' ');
	#endif
	#ifdef CHECKER_TAB_MEM_ADDR
		printchar(0, g, '-');
	#endif	
	putc('\n', CHECKER_OUT);

	/* Telo tabulky */
	aLm->iAct = aLm->iFirst;
	while(aLm->iAct)
		{
		#ifdef CHECKER_TAB_INDEX
		/* poradi alokace */
			pflen = fprintf(CHECKER_OUT, "%u", aLm->iAct->iAllocNum);
			printchar(pflen, a, ' ');
		#endif

		#ifdef CHECKER_TAB_FILE_LINENUM
		/* file: line */
			pflen = fprintf(CHECKER_OUT, "%s:%u", aLm->iAct->iSrcFileName, aLm->iAct->iSrcLineNum);
			printchar(pflen, b, ' ');
		#endif

		#ifdef CHECKER_TAB_FUNCTION
		/* function name and allocation type */
			pflen = 0;
			if(CHECKER_STRNLEN_S(aLm->iAct->iSrcFuncName, CHECKER_FUNCNAME_MAXLEN))
				{
				pflen = fprintf(CHECKER_OUT, "%s()", aLm->iAct->iSrcFuncName);
				}
			pflen += fprintf(CHECKER_OUT, ":%s()", alloc_type_str[aLm->iAct->iAllocType]);

			printchar(pflen, h, ' ');
		#endif

		#ifdef CHECKER_TAB_SIZE
			/* velikost */
			pflen = fprintf(CHECKER_OUT, CHECKER_SIZE_T_PRIu, (CHECKER_SIZE_T_CONV)aLm->iAct->iMemSize);
			printchar(pflen, e, ' ');
		#endif

		#ifdef CHECKER_TAB_MEM_ADDR
			#if(_MSC_VER >= 1310)  /* od VS2003 definovan modifikator velikosti (%Iu), ktery je bud I32 nebo I64 */
				pflen = fprintf(CHECKER_OUT, "0x%Ix-0x%Ix", (ptrdiff_t)aLm->iAct->iMemAddress, (ptrdiff_t)((char *)aLm->iAct->iMemAddress + aLm->iAct->iMemSize));
			#else
				pflen = fprintf(CHECKER_OUT, "%p-%p", aLm->iAct->iMemAddress, (void *) ((char *)aLm->iAct->iMemAddress + aLm->iAct->iMemSize));
			#endif
			printchar(pflen, g, ' ');
		#endif
		
		putc('\n', CHECKER_OUT);
		aLm->iAct = aLm->iAct->iNext;
		} /* while(aLm->iAct) */

	putc('\n', CHECKER_OUT);
	} /* allocc_print_lists() */


void filec_print_list(struct TListFile *aLf)
	{
	size_t b = 0, c = 0, d = 0, e = 0, f = 0;
	size_t mezer_mezi_sloupci, x = 0;
	size_t pflen = 0; /* printf length */

	if(!Used_file_c)
		{
		fprintf(CHECKER_OUT, "\nZadne soubory nebyly v programu zpracovavany.\n\n");
		return;
		}

	fprintf(CHECKER_OUT, "PREHLED OTEVRENYCH SOUBORU\n");
	fprintf(CHECKER_OUT, "Pocet otevrenych souboru: \t%u\n", aLf->iCounter_fopen);
	fprintf(CHECKER_OUT, "Pocet uzavrenych souboru: \t%u\n\n", aLf->iCounter_fclose);


	if(!aLf->iFirst)
		{
		fprintf(CHECKER_OUT, "Vsechny soubory byly bez chyby uzavreny. :-)\n\n");
		return;
		}

	fprintf(CHECKER_OUT, "NEUZAVRENYCH ZUSTAVA  %d  SOUBORU  !!!\n\n", aLf->iCounter_fopen - aLf->iCounter_fclose);

	/* ziskam delky nejdelsich udaju */
	aLf->iAct = aLf->iFirst;

	while(aLf->iAct)
		{
		#ifdef CHECKER_TAB_FILE_LINENUM
		/* soubor se zdrojovym kodem */
			x = CHECKER_STRNLEN_S(aLf->iAct->iSrcFileName, CHECKER_FILENAME_MAXLEN);
			if(x > b) b = x;
		/* cislo radku */
			x = chars_in_dec(aLf->iAct->iSrcLineNum);
			if(x > c) c = x;
		#endif

		#ifdef CHECKER_TAB_FUNCTION
		/* funkce */
			x = CHECKER_STRNLEN_S(aLf->iAct->iSrcFuncName, CHECKER_FUNCNAME_MAXLEN);
			if(x > d) d = x;
		#endif

		/* otevirany soubor */
		x = CHECKER_STRNLEN_S(aLf->iAct->iPath, CHECKER_PATHNAME_MAXLEN);
		if(x > e) e = x;

		/* mod otevreni */
		x = CHECKER_STRNLEN_S(aLf->iAct->iMode, CHECKER_FILEMODE_MAXLEN);
		if(x > f) f = x;

		aLf->iAct = aLf->iAct->iNext;
		} /* while(aLf->iAct) */

	x = b + c + d + e + f;						/* prizpusobeni sirky tabulky */
	if(x < 25) mezer_mezi_sloupci = 8;
	else if(x < 30) mezer_mezi_sloupci = 7;
	else if(x < 35) mezer_mezi_sloupci = 6;
	else if(x < 40) mezer_mezi_sloupci = 5;
	else if(x < 45) mezer_mezi_sloupci = 4;
	else if(x < 50) mezer_mezi_sloupci = 3;
	else mezer_mezi_sloupci = 2;

	/* zahlavi tabulky */
	fprintf(CHECKER_OUT, "VYPIS OTEVRENYCH SOUBORU\n");

	#ifdef CHECKER_TAB_FILE_LINENUM
		pflen = fprintf(CHECKER_OUT, "Misto otevreni");		/* soubor: radek */
		if(b + c + 1 < pflen)
			b = pflen;
		else
			b += c + 1;
		b += mezer_mezi_sloupci;
		printchar(pflen, b, ' ');
	#endif

	#ifdef CHECKER_TAB_FUNCTION
		if(d)
			{
			pflen = fprintf(CHECKER_OUT, "Funkce");			/* funkce */
			d += 2 + mezer_mezi_sloupci;
			printchar(pflen, d, ' ');
			}
	#endif

	pflen = fprintf(CHECKER_OUT, "Otevreny soubor");		/* Jmeno souboru */
	if(e < pflen)
		e = pflen;
	e += mezer_mezi_sloupci;
	printchar(pflen, e, ' ');

	pflen = fprintf(CHECKER_OUT, "Mod otevreni");		/* Mod otevreni */
	if(f < pflen)
		f = pflen;
	printchar(pflen, f, ' ');

	/* Title break line */
	putc('\n', CHECKER_OUT);
	#ifdef CHECKER_TAB_FILE_LINENUM
		printchar(mezer_mezi_sloupci, b, '-');
		printchar(0, mezer_mezi_sloupci, ' ');
	#endif
	#ifdef CHECKER_TAB_FUNCTION
		if(d)
			{
			printchar(mezer_mezi_sloupci, d, '-');
			printchar(0, mezer_mezi_sloupci, ' ');
			}
	#endif
	printchar(mezer_mezi_sloupci, e, '-');
	printchar(0, mezer_mezi_sloupci, ' ');

	printchar(0, f, '-');
	putc('\n', CHECKER_OUT);

	/* Telo tabulky */
	aLf->iAct = aLf->iFirst;

	while(aLf->iAct)
		{
		#ifdef CHECKER_TAB_FILE_LINENUM
		/* file: line */
			pflen = fprintf(CHECKER_OUT, "%s:%u", aLf->iAct->iSrcFileName, aLf->iAct->iSrcLineNum);
			printchar(pflen, b, ' ');
		#endif

		#ifdef CHECKER_TAB_FUNCTION
		/* funkce */
		if(d)
			{
			pflen = fprintf(CHECKER_OUT, "%s()", aLf->iAct->iSrcFuncName);
			printchar(pflen, d, ' ');
			}
		#endif

		/* nazev souboru */
		pflen = fprintf(CHECKER_OUT, "%s", aLf->iAct->iPath);
		printchar(pflen, e, ' ');

		/* mod otevreni */
		pflen = fprintf(CHECKER_OUT, "%s", aLf->iAct->iMode);
		printchar(pflen, f, ' ');

		putc('\n', CHECKER_OUT);
		aLf->iAct = aLf->iAct->iNext;
		} /* while(aLf->iAct) */

	putc('\n', CHECKER_OUT);
	} /* filec_print_list() */


void print_lists(struct TListMem *aLm, struct TListFile *aLf, CHECKER_BOOL aPrintAllocs, CHECKER_BOOL aPrintFiles)
	{
	if(aPrintAllocs)
		allocc_print_list(aLm);

	if(aPrintAllocs && aPrintFiles && Used_file_c)
		{
		putc('\n', CHECKER_OUT);
		printchar(0, CHECKER_PAGE_WIDTH, '-');
		putc('\n', CHECKER_OUT);
		}

	if(aPrintFiles)
		filec_print_list(aLf);

	fflush(CHECKER_OUT);
	} /* print_lists */

void printchar(size_t aFrom, size_t aTo, char aCh)
	{
	for(; aFrom < aTo; ++aFrom)
		putc(aCh, CHECKER_OUT);
	} /* printchar() */

unsigned int chars_in_dec(size_t aValue)
	{
	unsigned int i = 0;
	do
		{
		aValue /= 10;
		i++;
		}
	while(aValue);

	return i;
	} /* chars_in_dec() */

unsigned int chars_in_hex(ptrdiff_t aValue)
	{
	unsigned int i = 0;
	do
		{
		aValue /= 16;	//aValue >>= 4;
		i++;
		}
	while(aValue);

	return i;
	} /* chars_in_hex() */

/* End of file check.cpp */
