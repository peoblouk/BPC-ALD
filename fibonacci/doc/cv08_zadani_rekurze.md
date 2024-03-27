# Zadání cvičení 8

Cvičení je věnováno implementaci [rekurzivních funkcí](https://cs.wikipedia.org/wiki/Rekurze#Matematika). Rekurze budou procvičeny na dvou úlohách:

  1. Generátor Fibonacciho posloupnosti,
  2. *Wildcard string matching* - Test shody dvou řetězců, z nichž jeden označovaný jako: [glob](https://cs.wikipedia.org/wiki/Glob) nebo případně [vzor](https://en.wikipedia.org/wiki/Regular_expression#Patterns) (*pattern*), obsahuje zástupné tzv. [žolíkové](https://cs.wikipedia.org/wiki/%C5%BDol%C3%ADkov%C3%BD_znak) (*wildcard*) znaky.

---

## Zadání úlohy 8.1 - Generátor Fibonacciho posloupnosti
Napište funkce realizující výpočet [Fibonacciho posloupnosti](https://cs.wikipedia.org/wiki/Fibonacciho_posloupnost) pomocí následujících metod:

 1. funkce `fibonacci1()` - implementace pomocí stromové rekurze - viz přednáška,
 2. funkce `fibonacci2()` - implementace pomocí iterace, pomocí příkazu cyklu,
 3. funkce `fibonacci3()` - implementace pomocí stromové rekurze s memoizací (knihovna `memoizer.h`),
 4. funkce `fibonacci4()` - implementace pomocí rekurze s koncovým voláním.

### Soubory projektu `fibonacci`

Soubor `fibonacci.h` - obsahuje deklarace funkcí, které se mají realizovat. Navíc je zde, 
pro metodu se stromovou rekurzí a memoizací, deklarace proměnné memoizeru: `::mfibonacci3` a [wrapper funkce](https://en.wikipedia.org/wiki/Wrapper_function): `fibonacci3_wrapper()`.

Soubor `fibonacci.c` - zde se bude dopisovat kód jednotlivých implementací do připravených prázdných funkcí.

Soubor `main.c` - obsahuje funkci `main()`, která definuje vstupní a kontrolní výstupní hodnoty funkce pro výpočet Fibonacciho posloupnosti.
Pro variantu s memoizací provede inicializační fázi pomocí funkce `memoizer_init()`, a na konci data memoizeru zruší voláním funkce `memoizer_destroy()`.
Opakovaně je zde volána funkce `fibonacci_testing()` pro jednotlivé varianty řešení výpočtu.
Funkce: \code{.c}

	bool fibonacci_testing(const char *aFibFuncNameStr, unsigned (*aFibFunc)(unsigned), size_t aTestSetSize, const struct TTestSetItem aTestSet[])
\endcode   

zavolá dodanou funkci (jejíž adresa je předána pomocí parametru `aFibFunc`) pro všechny vstupní parametry (zadané v poli `aTestSet`), porovná získané výsledky s výsledky vzorovými a také změří čas výpočtu. Funkce vrací hodnotu `true` pokud všechny výsledky výpočtené testovanou funkcí jsou shodné se vzorovými.

Popis argumentů funkce:

  - `aFibFuncNameStr` - vstupní řetězec, který se vytiskne na konzolu jako popis,
  - `aFibFunc` - adresu funkce, která se bude testovat,
  - `aTestSetSize` - počet vstupních/výstupních hodnot, se kterými se bude funkce volat a testovat správnost výsledku,
  - `aTestSet` - pole se vstupními hodnotami a správnými výsledky odpovídající vstupním hodnotám (slouží k ověření správné funkčnosti testované funkce).

Soubor `memoizer.h` - obsahuje definice funkcí pro využití mechanizmů [memoizace](https://knihy.nic.cz/files/edice/pruvodce_labyrintem_algoritmu_v2.pdf) (viz str. 301).


### Úkoly

  1. Prostudujte si dodaný projekt a jeho soubory.
  2. Promyslete si jednotlivé implementace generátorů Fibonacciho posloupnosti.
  3. Realizujte jednotlivé implementace generátorů Fibonacciho posloupnosti.
  4. Na základě výstupů z vašeho programu zhodnoťte a relativně srovnejte časové a paměťové složitosti jednotlivých implementací.
  5. Srovnejte hodnoty z vašeho programu s hodnotami v souborech s příklady: `doc/cv08_reports_fibonacci_*_debug.lst` a `doc/cv08_reports_fibonacci_*_release.lst`. Výpočetní časy se budou samozřejmě lišit, ale **důsledně ověřte hodnoty statistik memoizeru** pro funkci: `fibonacci3_wrapper()`. Pokuste se dosáhnout stejných výsledků, aniž byste museli modifikovat obsah souboru: `main.c`.

### Rozbor úlohy
Rozbor obsahuje krátký návod společně s vysvětlením nejdůležitějších pojmů, které budete k vyřešení úlohy potřebovat.

#### Memoizace:
Memoizace (česky tabelace příp. memorování) - je optimalizační technika využívaná ve [funkcionálním programování](https://cs.wikipedia.org/wiki/Funkcion%C3%A1ln%C3%AD_programov%C3%A1n%C3%AD), která spočívá v tom, že se jednou (náročně) počítané
výsledné hodnoty funkce společně s požadovaným vstupem zapamatují (např. do tabulky) pro příští použití.

Je-li při dalším volání požadován výsledek pro již zapamatovaný vstupní argument funkce, potom se jako hodnota funkce vrací zapamatovaná hodnota výsledku, aniž by se požadovaná funkce volala. V ostatních případech se zavolá výpočet hodnoty funkce a výsledek se opět archivuje.
 
Jak je zřejmé tuto optimalizační techniku je možné využít pouze při volání tzv. čistých funkcí *[pure functions](https://en.wikipedia.org/wiki/Pure_function)*, tedy takových funkcí, které kromě výpočtu návratové hodnoty ze zadaných parametrů, nemají žádné postranní efekty, např. tisk na obrazovku, používání globálních proměnných apod. Funkce pro výpočet Fibonacciho čísla je právě příkladem takovéto čisté funkce a proto můžeme tuto optimalizační techniku využít a ověřit si její význam.


Řešení je připraveno v souboru `memoizer.h`. V souboru je nadefinovaný datový typ pro vstupní (`::TMemoizedFuncInput`) a výstupní (`::TMemoizedFuncOutput`) hodnoty.
Pro námi realizovaný generátor Fibonacciho posloupnosti to bude typ `unsigned`. Struktura `struct TMemoizerItem`
slouží pro uložení páru hodnot: vstupní argument funkce a k němu příslušné výstupní hodnoty (tj. výsledek) funkce.


Struktura `struct TMemoizerStats` slouží pro vytvoření statistiky využití tohoto mechanizmu.
Do statistiky jsou započítány dvě situace:

  - Požadavek na výpočet už jednou vypočtené hodnoty, kterou lze v archivu memoizeru doposud nalézt.
  - Požadavek na výpočet nové hodnoty (případně hodnoty, která již není v memoizeru uložena.)

Memoizer je ve funkci `main()` nastaven tak, aby archivoval poslední dvě hodnoty, **toto nastavení neměňte**.

Struktura `struct TMemoizer` je hlavní strukturou memoizeru, která obsahuje informaci o všech archivovaných hodnotách, jejich počtu, 
ukazatel na memoizovanou funkci a hodnoty pro statistiky.

Funkce:

  - `memoizer_init()` - 
     Inicializuje vnitřní složky struktury memoizeru, alokuje paměť archivu pro uložení definovaného počtu párů (vstup a výstup funkce) 
     tj. vstupních a výstupních hodnot. Vstupem je i adresa funkce pro výpočet výstupní hodnoty (tj. memoizovaná funkce).

  - `memoizer_eval()` - 
     Pokud zjistí, že vstupní hodnota již byla memoizována, (tj. najde ji mezi vstupními
     hodnotami uloženými v poli struktur `struct TMemoizerItem`), vrátí příslušnou výstupní hodnotu.
     Jinak vypočte výstupní hodnotu pro zadaný vstup, pomocí funkce, která je uložena ve struktuře
     `struct TMemoizer`. Pár vstupní a výstupní hodnoty uloží do svého archivu již počítaných hodnot. Případně přepíše nejstarší archivované hodnoty.

  - `memoizer_print_stats()` - vytiskne statistiky využití memoizeru.

  - `memoizer_clear()` - vynuluje parametry struktury `struct TMemoizer` včetně statistiky - neodalokuje paměť, pouze nastaví archiv jako prázdný.

  - `memoizer_destroy()` - vynuluje parametry včetně statistiky a dealokuje paměť pro archiv.

#### Jednotlivé implementace funkce `fibonacci`:
Funkce:

  - `fibonacci1()` - 
     Implementace pomocí stromové rekurze - základní vzorec se dvěma voláními téže funkce (viz přednáška 07).

  - `fibonacci2()` - 
     Implementace iterací, tj. pomocí příkazu cyklu, 
     kdy si pamatujeme vypočtené hodnoty a použijeme ji ve dvou následujících krocích výpočtu
     (jednou jako hodnotu `n-1` a v dalším kroku jako hodnotu `n-2`).

  - `fibonacci3()` - 
     Implementace pomocí stromové rekurze s memoizací (knihovna `memoizer.h`).
     Tato funkce slouží k získání i ke generování hodnot pro memoizaci. 
     Algoritmus je podobný jako v úkolu realizace funkce `fibonacci1()`, ale místo rekurentního volání se volá funkce `memoizer_eval()` jejímž parametrem
     je odkaz na strukturu se zapamatovanými prvky (`::mfibonacci3`) a požadovaná hodnota.

  - `fibonacci3_wrapper()` - 
     Je obalová funkce, která pouze zavolá funkci `memoizer_eval()` se stejným parametrem a příslušnou strukturou pro memoizované hodnoty.
     Je zde proto, abychom dodrželi shodné API pro všechny funkce generující Fibonacciho posloupnost.

  - `fibonacci4()` - 
     Implementace pomocí rekurze s koncovým voláním. Implementace s 
     rekurzí v koncovém volání vede k automatické eliminaci rekurze překladačem.
     V tomto případě ovšem musí být volání rekurze posledním voláním v příkazu
     `return`. Proto musíme použít konstrukci s pomocnou funkcí: `fibonacci4_worker()` (viz přednáška 07).



#### Úkol navíc (pro šťouraly, kteří se chtějí naučit něco dalšího):
Pokud si chcete na 100% ověřit v assembleru, že se skutečně provedla optimalizace 
koncové rekurze, zkuste následující postup: 

  - při breakpointu (tj. při spuštěném programu)
    dejte v souboru `fibonacci.c`  pravé tlačítko myši např. na funkci 
    `fibonacci4_worker()` a zvolte volbu:  **Go to disassembly**.

  - Zobrazí se vám kód C i strojový kód. Ve funkcích potom můžete zjistit, 
    zda se v nich vyskytuje jejich opětovné volání pomocí příkazu `call nazev_funkce` nebo pouze instrukce podmíněných
    skoků: např. `je` - *jump equal* a `jne` - *jump not equal* - ekvivalentní C konstrukcím: `if(!n)`, případně: `if(n == 0)`.

  - Aby se tato optimalizace projevila, musíme ji povolit
    nastavením projektu ( *Project properties / C & C++ / Optimization / Optimization - Maximum Optimization* )
    zároveň bude možná nutné (pokud bude hlášena chyba ... /RTC1... )
    nastavit také: ( *Project properties / C & C++ / Code Generation / Basic Runtime Checks - default* ). 

  - Po disassemblování vraťte volby do původního stavu.

  - Jednodušší možností je však vše přeložit a pozorovat výpis programu v režimu: **Release**.

  - Případně využijte online nástroj: [Compiler Explorer](http://www.godbolt.org).


---

## Zadání úlohy 8.2 - (*wildcard string matching*) - Test shody dvou řetězců, z nichž jeden obsahuje žolíkové znaky

Napište tělo funkce `wildcard_match()`, realizující rekurzivní řešení úlohy testu na shodu dvou řetězců,
z nichž jeden obsahuje zástupné (tzv. [žolíkové](https://cs.wikipedia.org/wiki/%C5%BDol%C3%ADkov%C3%BD_znak)) znaky: `*` a `?`. Tyto znaky mají při testování shody jednotlivých znaků řetězce speciální význam.

### Projekt: `wildcard_match`
Tento podprojekt slouží k vlastní realizaci funkce `wildcard_match()` a ukázce jejího použití
ve funkci `main()`. Oba řetězce se načtou z klávesnice a porovnají. Následně je zobrazen výsledek.

### Projekt: `UnitTest`
Projekt obsahuje i připravený [unit test](https://cs.wikipedia.org/wiki/Unit_testing), který umožní rychle ověřovat správnost 
implementace funkce `wildcard_match()`. Projekt `UnitTest` má jediný soubor.
Jeho základem je sice C++, ale testování je provedeno velice jednoduchými mechanizmy,
takže ze struktury kódu by mělo být jasné, jak pracuje.

**Unit test je** (stručně pro VS a XCode): speciální projekt, ve kterém se volají programátorem nadefinované funkce a pomocí předpřipravených
funkcí či maker (a jejich návaznosti na použité prostředí) se otestuje a zobrazí výsledek testu.
V testu se volá funkce a následně se porovnává skutečná a očekávaná hodnota výsledku. 
Testovat budeme výsledek porovnání, tj. `true` nebo `false`. Budeme tedy kontrolovat, zda se výsledek rovná nebo nerovná očekávané hodnotě tj.: (`UT_ASSERT_IS_TRUE(výsledek)`, `UT_ASSERT_IS_FALSE(výsledek)`).

**Unit test funguje**: tak, že využívá zdrojové a přeložené soubory testovaného projektu a ověřuje správnost implementace jednotlivých (jednotek) v našem případě funkcí
(testované soubory tedy nekopírujeme, ale jen mezi oběma projekty sdílíme). Pro testování je připraven kód tak, aby
získal voláním výsledek určité jednotky (v nejjednodušším případě zavoláme testovanou funkci). Pomocí
funkcí ze skupiny `UT_ASSERT_...` porovná shodu či neshodu výsledků s požadavkem. 
V případě, že se funkce neshoduje s předpokládaným výsledkem, 
běh tohoto testu se přeruší a výsledek se zobrazí v prostředí a pokračuje se na další test v pořadí.   
Vlastní realizace je tedy taková, že se zavolá funkce `wildcard_match()` se dvěma řetězci, pro které předem známe, správný výsledek funkce.
Jelikož známe správný výsledek, víme také, jak jej otestovat pomocí maker: `UT_ASSERT_IS_TRUE()`, nebo `UT_ASSERT_IS_FALSE()`.

Výsledek funkce je tedy vždy ověřen a v případě neočekávaného výsledku je zobrazena chyba.   
Tento postup nám umožňuje se zbavit nutnosti, při každé změně zdrojového textu funkce, zadávat opakovaně všechny testové kombinace vstupů ručně.

**Jak se unit testování používá**: Unit testy mají vlastní položku v menu. Okno testů vyvoláme pomocí volby
menu *Test/Test Explorer*. Testy spustíme pomocí tlačítka *Run* nebo *Run All Tests* (jejichž ikony 
jsou zelené trojúhelníky). Před jejich použitím je vhodné celý projekt přeložit. V okně *Test Explorer* se objeví
"adresářová struktura"/strom s výsledky jednotlivých testů (tuto strukturu budete patrně
muset rozvinout). V rozvinuté struktuře má každý test svůj řádek a označení výsledku - červená 
znamená, že v něm došlo k chybě, zelená, že je v pořádku. Poklikáním na položku se dostaneme
ke zdrojovému kódu testu a v okně *Test Explorer* uvidíme bližší informace o případném neočekávaném chování testované funkce.

### Soubory projektu `wildcard_match`
Kromě již popsaného souboru `main.c` je zde nejdůležitější soubor `wildcard_match.c`
ve kterém je potřeba doplnit tělo funkce realizované rekurzivním algoritmem.


### Úkoly
Pro zjištění správnosti řešení použijte projekt s unit testy (otevřete *Test Explorer* a spusťte testování).
Testy je možné nastavit v prostředí dokonce tak, aby se spouštěly automaticky po každém sestavení programu.
Snažte se za pomocí všech dodaných unit testů a případně níže uvedených řetězců odladit fungující algoritmus pro určení shody dvou řetězců, z nichž jeden obsahující žolíkové znaky.


### Rozbor

Další možné testovací řetězce (kromě těch, které jsou v modulu `UnitTest`):

      "ab*cd", "abcd"       // Ano
      "ab*cd", "abxycd"     // Ano
      "ab*cd", "abxycccd"   // Ano
      "ab*cd", "abccxyccd"  // Ano

      "ab?cd", "abxcd"      // Ano
      "ab?cd", "abxccd"     // Ne

      "*abcd", "abcd"       // Ano
      "*abcd", "xyabcd"     // Ano
      "*abcd", "xyaabcd"    // Ano

      "abcd*", "abcd"       // Ano
      "abcd*", "abcdabb"    // Ano

      "abcd?", "abcd"       // Ne
      "abcd?", "abcdx"      // Ano
      "abcd?", "abcdxy"     // Ne

      "abc*c?d", "abcd"     // Ne

      "*c*d", "abcd"        // Ano

      "*?c*d", "abcd"       // Ano

      "ba*a?","baaabab"     // Ano

      "ba*ab","baaabab"     // Ano
      "ba*ab","baaa*ab"     // Ano

---
`$Id: cv08_zadani_rekurze.md 2648 2024-03-18 14:00:53Z petyovsky $`
