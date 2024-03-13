# Zadání cvičení 6
Rozšiřte implementaci vektoru (s dynamickým polem) o algoritmy přímého třídění (basic_sorts). Zhodnoťte časovou složitost jednotlivých metod přímého třídění. Časovou složitost každého z algoritmů třídění vyhodnoťte pomocí určení počtu elementárních operací.
Použijte dodaný projekt a dopište kód do prázdných těl připravených funkcí.

## Soubory projektu
`TVectorElement.h` obsahuje definici základního datového typu (důsledně používejte typ `::TVectorElement` všude, kde se pracuje s datovým typem, který představuje elementy `TVector`). Dále jsou zde podpůrné funkce pro tento datový typ (porovnání hodnot dvou elementů, načtení i uložení hodnoty elementu do souboru, vytvoření elementu s náhodnou hodnotou).

Zdrojový soubor `TVector.h` obsahuje struktury definující ADT Vektor a jeho iterátor. ADT Vektor byl pro účely třídění doplněn o další funkce. Struktura vektoru je rozšířena o složku [iStats](\ref TVector::iStats) obsahující statistiky počtu provedených operací s danou proměnnou. V této statistice je evidován (přičtením jedničky k počítadlu dané operace) celkový počet provedených operací s každým elementem vektoru. Dále jsou v hlavičkovém souboru prototypy nových funkcí pro manipulaci s vektorem a také pro práci s iterátorem vektoru.

Soubor `basic_sorts.c` je určen k vlastní implementaci třídicích metod - obsahuje připravené funkce s prázdnými těly pro doplnění kódu třídicích metod. Příslušný soubor `basic_sorts.h` obsahuje připravené deklarace těchto funkcí.

Pro kontrolu správné manipulace s pamětí jsou v projektu soubory: `check.h` a `check.c`.

Soubor `main.c` obsahuje funkci `main()`, která načte data z textového souboru a spustí funkci `sort_testing()`. Tato funkce zavolá vlastní třídění. Přitom zjistí a následně vytiskne dobu trvání třídění a statistiku obsahující počty elementárních operací třídicí metody.

## Úkoly
1. Seznamte s rozšířením struktury `TVector` pro účely implementace třídicích metod. Prostudujte si zdrojové soubory: `TVector.h`, `TVector.c` a `TVectorElement.h`. Zjistěte rozdíly oproti předchozím cvičením a prostudujte implementaci, činnost a význam nového kódu. Počet jakých elementárních operací (v rámci statistik) struktura eviduje  a proč?
2. Prostudujte si činnost funkce `main()`.
3. Prostudujte si připravená data testovacích souborů ve složce projektu: `Resource Files`.
4. Postupně naprogramujte jednotlivé třídicí metody a sledujte jejich výstupy zobrazované funkcí `main()` resp. `sort_testing()`. Zamyslete se u každé z implementovaných metod nad očekávaným výsledky počtu provedených operací a také nad výsledky měření doby běhu algoritmu.
    - `insert_sort()` - třídění přímým vkládáním,
    - `select_sort()` - třídění přímým výběrem,
    - `bubble_sort()` - třídění přímou výměnou,
    - `bubble_sort_optimized()` - modifikace třídění přímou výměnou - optimalizace s pomocným příznakem ukončení třídění.
5. Pro každou z třídicích metod porovnejte, s hodnotami z tabulek uvedených dále, počty provedených elementárních operací. Ověřte, zda odpovídají teorii prezentované v přednáškách.
6. Na následujícím cvičení budeme implementovat dalším třídicí metody. Opět se zamyslete u každé z implementovaných metod nad očekávanými výsledky ohledně počtu provedených operací a měření doby běhu algoritmu.
    - `bubble_sort_optimized2()` - modifikace třídění přímou výměnou - optimalizace s určením indexu poslední výměny,
    - `shaker_sort()` - modifikace třídění přímou výměnou - koktejlové třídění,
    - třídění s rozšířením `::TVectorElement` o strukturované elementy a pojem [stabilita](https://cs.wikipedia.org/wiki/Stabiln%C3%AD_%C5%99azen%C3%AD) třídicí metody.

## Rozbor jednotlivých úkolů
1. Rozbor struktury statistiky provedených operací:
    - Proč je statistika provedených operací důležitá pro srovnání algoritmů.
    - Proč jsou evidovány u třídicích algoritmů pouze počty operací: porovnání (*compare*), výměny (*swap*), přesunu (*move*)? Bude to pro ověření míry časové složitosti jednotlivých algoritmů postačovat? 
    - Projděte si jejich použití v API `TVector` - které funkce je využívají?

    Chybějící API (pro třídicí metody nepodstatné):
    \code{.c}

        vector_resize()
            // není implementováno
    \endcode

    Nové API:
    \code{.c}

        void vector_move_positions(struct TVector *aToVector, size_t aToPos, const struct TVector *aFromVector, size_t aFromPos);
            // zkopíruje hodnotu elementu z pozice v jednom vektoru na pozici druhého vektoru
            // Pozor ověřte si, co se stane, použijeme-li pro zdrojový a cílový vektor stejnou proměnnou?
            // !!! Tuto otázku si položte i u dalších funkcí majících dva parametry stejného typu (tj. předané odkazem) !!!

        int vector_compare_positions(const struct TVector *aLeftVector, size_t aLeftPos, const struct TVector *aRightVector, size_t aRightPos);
            // porovná hodnoty na dvou pozicích dvou vektorů

        int vector_compare_position_value(const struct TVector *aVector, size_t aPos, TVectorElement aValue);
            // porovná hodnotu na dané pozici s předanou hodnotou

        void vector_swap_positions(struct TVector *aLeftVector, size_t aLeftPos, struct TVector *aRightVector, size_t aRightPos);
            // vymění mezi sebou dvě hodnoty ve dvou vektorech na daných pozicích

        void vector_swap_position_value(struct TVector *aVector, size_t aPos, TVectorElement *aValue);
            // vymění hodnotu na dané pozici s předanou hodnotou (odkazem)

        bool vector_is_ascendantly_sorted(const struct TVector *aUnsortedVector, const struct TVector *aSortedVector);
            // zjistí, zda druhý předaný parametr odpovídá správně setříděnému prvnímu parametru
            // tj. první parametr obsahuje originální nesetříděná data, druhý parametr stejná data setříděná
            // prostudujte si způsob jakým je proveden test správnosti setřídění

        bool vector_print_sortstats(const struct TVector *aVector);
            // tisk statistik
    \endcode

    Další nové API pro operace s iterátory (prozatím nepotřebné):
    \code{.c}

        void vector_move_with_iterators(const struct TVectorIterator *aToIter, const struct TVectorIterator *aFromIter);
            // jako move pro vektory, ale pomocí iterátorů

        int vector_compare_with_iterators(const struct TVectorIterator *aLeftIter, const struct TVectorIterator *aRightIter);
            // jako compare pro vektory ale pomocí iterátorů

        int vector_compare_with_iterator_value(const struct TVectorIterator *aIter, TVectorElement aValue);
            // jako compare pro vektor ale pomocí iterátoru

        void vector_swap_with_iterators(const struct TVectorIterator *aLeftIter, const struct TVectorIterator *aRightIter);
            // jako swap pro vektory ale pomocí iterátorů

        void vector_swap_with_iterator_value(const struct TVectorIterator *aIter, TVectorElement *aValue);
            // jako swap pro vektor ale pomocí iterátoru
    \endcode

2. Soubor `main.c` obsahuje dvě funkce zajišťující automatické testování třídicích metod. Prostudujte si jejich obsah. Funkce `main()` také obsahuje prozatím zakomentované části, promyslete si kdy a k čemu je budeme potřebovat.

3. Pro testování jsou připraveny soubory s daty (najdete je v projektové složce: `Resource files`). V rámci rozboru hledejte odpovědi na otázky:
    - Proč je potřeba pro testování několik souborů?
    - Proč je nutné otestovat metody i na částečně setříděných datech, případně na "opačně" setříděných datech?

    Funkce `main()` otevře soubor s daty a načte ho do vektoru. Postupně načtěte a vyzkoušejte všechny připravené datové soubory a srovnejte výsledky jednotlivých třídění pro tento typ dat.

    Funkce `sort_testing()` - vytvoří si kopii dat, aby bylo možné originální vektor použít i pro další volání. Opět si všimněte způsobu předání testované třídicí funkce. Jako parametr funkce `sort_testing()` je odkaz na volanou testovanou funkci. To umožní napsat kód někdy označovaný jako tzv. [testbench](https://en.wikipedia.org/wiki/Test_bench) (příprava dat, tisk výsledků, uvolnění paměti atd.) pouze jednou a přitom zajistit jeho univerzální použití. Testbench funkci `sort_testing()` lze postupně použít pro různé třídící funkce (volá se ta funkce, jejíž adresa je předána jako parametr). Ve funkci je zkontrolováno, zda jsou data správně setříděna a jsou vytištěny statistiky, včetně času potřebného ke třídění.

4. viz přednášky.
Funkce, které by se vám mohly hodit při implementaci třídicích algoritmů:
    - **InsertSort** - `vector_value()`, `vector_compare_position_value()`, `vector_move_positions()`, `vector_set_value()`.
    - **SelectSort** - `vector_compare_positions()`, `vector_swap_positions()`.
    - **BubbleSort** (včetně optimalizované verze) - `vector_compare_positions()`, `vector_swap_positions()`.

5. Výsledné počty operací: *compare*, *swap*, *move*, *get*, *set* musí odpovídat hodnotám uvedeným v tabulkách pro jednotlivé soubory dat.   
**Pokud počty operací ve vaší implementaci neodpovídají, není vaše implementace korektní a je nutné ji opravit.**

    `reversed_data.txt`:
    |             | InsertSort | SelectSort | BubbleSort |BS optimized |
    |-------------|------------|------------|------------|-------------|
    |Comparisons: | 45         | 45         | 45         | 45          |
    |Swaps:       |  0         |  9         | 45         | 45          |
    |Moves:       | 45         |  0         |  0         |  0          |
    |Value Gets:  |  9         |  0         |  0         |  0          |
    |Value Sets:  |  9         |  0         |  0         |  0          |

    `select_sort_data.txt`:
    |             | InsertSort | SelectSort | BubbleSort |BS optimized |
    |-------------|------------|------------|------------|-------------|
    |Comparisons: | 29         | 45         | 45         | 39          |
    |Swaps:       |  0         |  9         | 25         | 25          |
    |Moves:       | 25         |  0         |  0         |  0          |
    |Value Gets:  |  9         |  0         |  0         |  0          |
    |Value Sets:  |  9         |  0         |  0         |  0          |

    `bubble_sort_optimized_data.txt`:
    |             | InsertSort | SelectSort | BubbleSort |BS optimized |
    |-------------|------------|------------|------------|-------------|
    |Comparisons: | 21         | 45         | 45         | 24          |
    |Swaps:       |  0         |  9         | 13         | 13          |
    |Moves:       | 13         |  0         |  0         |  0          |
    |Value Gets:  |  9         |  0         |  0         |  0          |
    |Value Sets:  |  9         |  0         |  0         |  0          |

    `shaker_sort_data.txt`:
    |             | InsertSort | SelectSort | BubbleSort |BS optimized |
    |-------------|------------|------------|------------|-------------|
    |Comparisons: | 24         | 45         | 45         | 45          |
    |Swaps:       |  0         |  9         | 17         | 17          |
    |Moves:       | 17         |  0         |  0         |  0          |
    |Value Gets:  |  9         |  0         |  0         |  0          |
    |Value Sets:  |  9         |  0         |  0         |  0          |

    `sorted_data.txt`:
    |             | InsertSort | SelectSort | BubbleSort |BS optimized |
    |-------------|------------|------------|------------|-------------|
    |Comparisons: |  9         |  45        | 45         |  9          |
    |Swaps:       |  0         |  9         |  0         |  0          |
    |Moves:       |  0         |  0         |  0         |  0          |
    |Value Gets:  |  9         |  0         |  0         |  0          |
    |Value Sets:  |  9         |  0         |  0         |  0          |

---
`$Id: cv06_zadani_basic_sorts.md 2625 2024-03-08 16:06:15Z petyovsky $`
