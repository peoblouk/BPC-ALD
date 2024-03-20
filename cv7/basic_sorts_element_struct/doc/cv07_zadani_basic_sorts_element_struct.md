# Zadání cvičení 7

## 1. Dokončení algoritmů přímého třídění
Dokončete algoritmy přímého třídění v projektu `basic_sorts`. Zhodnoťte časovou složitost jednotlivých metod přímého třídění. Časovou složitost každého z algoritmů třídění vyhodnoťte pomocí určení počtu elementárních operací. Použijte nově dodaný projekt a dopište kód do prázdných těl připravených funkcí.

### Úkoly
1. Ověřte funkčnost přímých metod třídění, které jste realizovali v předchozím cvičení a přeneste jejich zdrojové texty do nového projektu pro toto cvičení. Ověřte jejich výstupy zobrazované funkcí `main()` resp. `sort_testing()`. Zamyslete se u každé z implementovaných metod nad očekávaným výsledky počtu provedených operací a také nad výsledky měření doby běhu algoritmu.
    - `insert_sort()` - třídění přímým vkládáním,
    - `select_sort()` - třídění přímým výběrem,
    - `bubble_sort()` - třídění přímou výměnou,
    - `bubble_sort_optimized()` - modifikace třídění přímou výměnou - optimalizace s pomocným příznakem ukončení třídění.
2. Na tomto cvičení budeme implementovat další třídicí metody. Opět si ověřte u každé z implementovaných metod očekávané výsledky počtu provedených operací a výsledky měření doby běhu algoritmu.
    - `bubble_sort_optimized2()` - modifikace třídění přímou výměnou - optimalizace s určením indexu poslední výměny,
    - `shaker_sort()` - modifikace třídění přímou výměnou - [koktejlové třídění](https://cs.wikipedia.org/wiki/Koktejlov%C3%A9_%C5%99azen%C3%AD).
    
3. Pro každou z třídicích metod ověřte, zda odpovídají hodnotám uvedeným v tabulkách pro jednotlivé soubory dat.

### Rozbor jednotlivých úkolů
1. Pro každou z třídicích metod porovnejte počty provedených elementárních operací. Ověřte, zda odpovídají teorii prezentované v přednáškách.
2. Funkce, které by se vám mohly hodit při implementaci třídicích algoritmů:
    - **InsertSort** - `vector_value()`, `vector_compare_position_value()`, `vector_move_positions()`, `vector_set_value()`.
    - **SelectSort** - `vector_compare_positions()`, `vector_swap_positions()`.
    - **BubbleSort** - `vector_compare_positions()`, `vector_swap_positions()`.
    - **BubbleSort optimized** (s pomocným příznakem) - `vector_compare_positions()`, `vector_swap_positions()`.
    - **BubbleSort optimized2** (s výpočtem indexu poslední změny) - `vector_compare_positions()`, `vector_swap_positions()`.
    - **ShakerSort** - `vector_compare_positions()`, `vector_swap_positions()`.
3. Výsledné počty operací: *compare*, *swap*, *move*, *get*, *set* musí odpovídat hodnotám uvedeným v tabulkách pro jednotlivé soubory dat.   
**Pokud počty operací ve vaší implementaci neodpovídají, není vaše implementace korektní a je nutné ji opravit.**

    `reversed_data.txt`:
    |             | InsertSort | SelectSort | BubbleSort |BS optimized |BS optimized2| ShakerSort |
    |-------------|------------|------------|------------|-------------|-------------|------------|
    |Comparisons: | 45         | 45         | 45         | 45          | 45          | 45         |
    |Swaps:       |  0         |  9         | 45         | 45          | 45          | 45         |
    |Moves:       | 45         |  0         |  0         |  0          |  0          |  0         |
    |Value Gets:  |  9         |  0         |  0         |  0          |  0          |  0         |
    |Value Sets:  |  9         |  0         |  0         |  0          |  0          |  0         |

    `select_sort_data.txt`:
    |             | InsertSort | SelectSort | BubbleSort |BS optimized |BS optimized2| ShakerSort |
    |-------------|------------|------------|------------|-------------|-------------|------------|
    |Comparisons: | 29         | 45         | 45         | 39          | 29          | 36         |
    |Swaps:       |  0         |  9         | 25         | 25          | 25          | 25         |
    |Moves:       | 25         |  0         |  0         |  0          |  0          |  0         |
    |Value Gets:  |  9         |  0         |  0         |  0          |  0          |  0         |
    |Value Sets:  |  9         |  0         |  0         |  0          |  0          |  0         |

    `bubble_sort_optimized_data.txt`:
    |             | InsertSort | SelectSort | BubbleSort |BS optimized |BS optimized2| ShakerSort |
    |-------------|------------|------------|------------|-------------|-------------|------------|
    |Comparisons: | 21         | 45         | 45         | 24          | 24          | 22         |
    |Swaps:       |  0         |  9         | 13         | 13          | 13          | 13         |
    |Moves:       | 13         |  0         |  0         |  0          |  0          |  0         |
    |Value Gets:  |  9         |  0         |  0         |  0          |  0          |  0         |
    |Value Sets:  |  9         |  0         |  0         |  0          |  0          |  0         |

    `shaker_sort_data.txt`:
    |             | InsertSort | SelectSort | BubbleSort |BS optimized |BS optimized2| ShakerSort |
    |-------------|------------|------------|------------|-------------|-------------|------------|
    |Comparisons: | 24         | 45         | 45         | 45          | 45          | 24         |
    |Swaps:       |  0         |  9         | 17         | 17          | 17          | 17         |
    |Moves:       | 17         |  0         |  0         |  0          |  0          |  0         |
    |Value Gets:  |  9         |  0         |  0         |  0          |  0          |  0         |
    |Value Sets:  |  9         |  0         |  0         |  0          |  0          |  0         |

    `sorted_data.txt`:
    |             | InsertSort | SelectSort | BubbleSort |BS optimized |BS optimized2| ShakerSort |
    |-------------|------------|------------|------------|-------------|-------------|------------|
    |Comparisons: |  9         |  45        | 45         |  9          |  9          |  9         |
    |Swaps:       |  0         |  9         |  0         |  0          |  0          |  0         |
    |Moves:       |  0         |  0         |  0         |  0          |  0          |  0         |
    |Value Gets:  |  9         |  0         |  0         |  0          |  0          |  0         |
    |Value Sets:  |  9         |  0         |  0         |  0          |  0          |  0         |
    
    


---

## 2. Třídění vektoru s elementy představujícími kompozitní datový typ
V novém projektu `basic_sorts_element_struct` otestujte algoritmy přímého třídění elementů typu struktura dle různých klíčů.

Prostudujte zdrojové texty reprezentující strukturovaný typ `TVectorElement` a odlaďte pro něj všech 6 třídicích metod, které již máte hotové a které by měli být pro tento nový datový typ funkční bez jakékoliv změny zdrojových textů třídicích funkcí.

### Soubory projektu `basic_sorts_element_struct`
Většina souborů je stejná, jako v minulém příkladu. K výraznějším změnám došlo pouze v souboru `TVectorElement.h` a byl přidán soubor `TVectorElement.c`, které tvoří rozhraní pro práci se zvoleným datovým typem.

Soubor `TVectorElement.h` obsahuje definici základního datového typu `::TVectorElement`. Tento datový typ používejte všude, kde se pracuje s datovým typem, který obsahuje `TVector`.

Datový typ `::TVectorElement` je nyní reprezentován strukturou `struct TStudent`, která obsahuje položky:

   - `char iFirstName[21]` - \ref TStudent::iFirstName "Jméno studenta",
   - `char iLastName[31]` - \ref TStudent::iLastName "Příjmení studenta",
   - `size_t iId` - \ref TStudent::iId "ID studenta",
   - `unsigned short iPoints` - \ref TStudent::iPoints "Bodové hodnocení studenta".

Dále soubor `TVectorElement.h` obsahuje podpůrné funkce pro tento složený datový typ.

Oproti minulému projektu přibyl soubor `TVectorElement.c`, který obsahuje definici proměnné výčtového typu `::TVectorElementCompareSelect`, která určuje klíč pro porovnání dvou elementů. Klíč pro porovnání je nastavitelný pomocí funkce: `vector_element_set_comparator()`, na tyto možné hodnoty:

   - `EByFirstName` - porovnání dle klíče: \ref TVectorElementCompareSelect::EByFirstName "Jméno studenta",
   - `EByLastName` - porovnání dle klíče: \ref TVectorElementCompareSelect::EByLastName "Příjmení studenta",
   - `EById` - porovnání dle klíče: \ref TVectorElementCompareSelect::EById "ID studenta",
   - `EByPoints` - porovnání dle klíče: \ref TVectorElementCompareSelect::EByPoints "Bodové hodnocení studenta".

`TVector.h` je shodný s předchozím cvičením a obsahuje strukturu vektoru a iterátoru. Struktura vektoru byla doplněna o strukturu statistiky udržující počet operací s danou proměnnou. V této statistice se bude evidovat (přičtení jedničky k počítadlu dané operace) každá operace s elementem vektoru. Jsou zde i další funkce pro práci s iterátorem vektoru.

Soubor `basic_sorts.c` je určený k vlastní činnosti - jsou v něm funkce s prázdnými těly pro doplnění kódu. K němu příslušný `basic_sorts.h` obsahuje deklarace těchto funkcí.

Pro kontrolu správné manipulace s pamětí jsou v projektu soubory `check.h` a `check.c`.

Soubor `main.c` obsahuje funkci `main()`, která načte data ze souboru a spustí funkci `sort_testing()`. Tato funkce spustí vlastní třídění. Přitom zjistí a následně vytiskne čas trvání třídění a statistiky práce s daty.
Funkce `main()` je doplněna o příklad ověřující [stabilitu](https://cs.wikipedia.org/wiki/Stabiln%C3%AD_%C5%99azen%C3%AD "Stabilita třídicího algoritmu je vlastnost, která zajišťuje, že třídicí algoritmus ve výstupní posloupnosti nemění pořadí položek se stejným klíčem.") výsledného třídění metodou **select sort**, při třídění podle jiných klíčů než v inicializační verzi třídění. (*Pozn.: Stabilita třídicího algoritmu je vlastnost, která zajišťuje, že třídicí algoritmus ve výstupní posloupnosti nemění pořadí položek se stejným klíčem.*)

### Úkoly
1. V projektu `basic_sorts_element_struct` prostudujte si činnost funkcí, jejichž volání bylo doplněno do funkce `main()`.
2. Prostudujte si novou implementaci typu `::TVectorElement` definovaného v souborech `TVectorElement.h` a `TVectorElement.c`.
3. Přesuňte do nového projektu jednotlivé třídicí metody tak, aby fungovaly se složeným datovým typem a sledujte jejich výstupy zobrazované funkcí `main()` resp. `sort_testing()`.
4. Ověřte si praktickou ukázkou, zda algoritmus **select sort** je stabilní třídicí metoda.

### Rozbor jednotlivých úkolů
1. Byla doplněna funkce `vector_element_set_comparator()` (v souboru `TVectorElement.c`), která nastaví (globální) proměnnou určující, podle kterého klíče se budou ve funkci `vector_element_comparator()` elementy porovnávat (a tedy i třídit).
2. Rozbor funkcí v souboru `TVectorElement.h`. Rozdílem oproti minulému projektu je složitější manipulace s prvkem `::TVectorElement`, který je nyní reprezentovaný strukturou. Je zde definován typ `::TVectorElementCompareSelect`, podle kterého volí funkce položku pro porovnání.
3. Pro testování je připraven soubor s daty (najdete ho ve složce projektu `Resource files`).

    Funkce `main()` otevře soubor s daty a načte ho do vektoru. Postupně načtěte a vyzkoušejte všechny připravené datové soubory a srovnejte výsledky jednotlivých třídění pro tento typ dat.

    Funkce `sort_testing()` - vytvoří si kopii dat, aby bylo možné originální vektor použít i pro další volání. Opět si všimněte způsobu předání testované třídicí funkce. Jako parametr funkce `sort_testing()` je odkaz na volanou testovanou funkci. To umožní napsat kód někdy označovaný jako tzv. testbench (příprava dat, tisk výsledků, uvolnění paměti atd.) pouze jednou a přitom zajistit jeho univerzální použití. Testbench funkci `sort_testing()` lze postupně použít pro různé třídicí funkce (volá se ta funkce, jejíž adresa je předána jako parametr). Ve funkci je zkontrolováno, zda jsou data správně setříděna a jsou vytištěny statistiky, včetně času potřebného ke třídění.

    Pozn.: Pokud byly třídicí algoritmy dobře naprogramovány v minulém cvičení (tj. za pomoci funkcí pro `::TVectorElement`), měly by třídicí algoritmy fungovat prakticky bez úprav. To znamená, že pokud budete muset něco upravit v novém projektu, měli byste se zamyslet i nad provedením stejné úpravy v minulém projektu.
4. Zkontrolujte si výsledný výpis vašeho programu projektu `basic_sorts_element_struct` s referenčním výpisem a
případně jej upravte tak, abyste dosáhli shodného výstupu:

        ------ Insert Sort ------
        Tomas Jedno 1111 9
        Pepa Druhy 222 8
        Petr Treti 3 7
        Bozena Ctvrta 4 6
        Pepa Paty 5 5
        Jan Sesty 6 4
        Pepa Sedmy 7 3
        Pepa Osmy 8 2
        Milos Devaty 9 1
        Skluzan Tavic 99 0

        Skluzan Tavic 99 0
        Milos Devaty 9 1
        Pepa Osmy 8 2
        Pepa Sedmy 7 3
        Jan Sesty 6 4
        Pepa Paty 5 5
        Bozena Ctvrta 4 6
        Petr Treti 3 7
        Pepa Druhy 222 8
        Tomas Jedno 1111 9


        Vector is sorted. OK :-)

        Sorting time: 0.000000 seconds.

        Comparisons: 45
        Swaps: 0
        Moves: 45
        Value Gets: 9
        Value Sets: 9

        ------ Select Sort ------
        Tomas Jedno 1111 9
        Pepa Druhy 222 8
        Petr Treti 3 7
        Bozena Ctvrta 4 6
        Pepa Paty 5 5
        Jan Sesty 6 4
        Pepa Sedmy 7 3
        Pepa Osmy 8 2
        Milos Devaty 9 1
        Skluzan Tavic 99 0

        Skluzan Tavic 99 0
        Milos Devaty 9 1
        Pepa Osmy 8 2
        Pepa Sedmy 7 3
        Jan Sesty 6 4
        Pepa Paty 5 5
        Bozena Ctvrta 4 6
        Petr Treti 3 7
        Pepa Druhy 222 8
        Tomas Jedno 1111 9


        Vector is sorted. OK :-)

        Sorting time: 0.000000 seconds.

        Comparisons: 45
        Swaps: 9
        Moves: 0
        Value Gets: 0
        Value Sets: 0

        ------ Bubble Sort ------
        Tomas Jedno 1111 9
        Pepa Druhy 222 8
        Petr Treti 3 7
        Bozena Ctvrta 4 6
        Pepa Paty 5 5
        Jan Sesty 6 4
        Pepa Sedmy 7 3
        Pepa Osmy 8 2
        Milos Devaty 9 1
        Skluzan Tavic 99 0

        Skluzan Tavic 99 0
        Milos Devaty 9 1
        Pepa Osmy 8 2
        Pepa Sedmy 7 3
        Jan Sesty 6 4
        Pepa Paty 5 5
        Bozena Ctvrta 4 6
        Petr Treti 3 7
        Pepa Druhy 222 8
        Tomas Jedno 1111 9


        Vector is sorted. OK :-)

        Sorting time: 0.000000 seconds.

        Comparisons: 45
        Swaps: 45
        Moves: 0
        Value Gets: 0
        Value Sets: 0

        ------ Bubble Sort Optimized ------
        Tomas Jedno 1111 9
        Pepa Druhy 222 8
        Petr Treti 3 7
        Bozena Ctvrta 4 6
        Pepa Paty 5 5
        Jan Sesty 6 4
        Pepa Sedmy 7 3
        Pepa Osmy 8 2
        Milos Devaty 9 1
        Skluzan Tavic 99 0

        Skluzan Tavic 99 0
        Milos Devaty 9 1
        Pepa Osmy 8 2
        Pepa Sedmy 7 3
        Jan Sesty 6 4
        Pepa Paty 5 5
        Bozena Ctvrta 4 6
        Petr Treti 3 7
        Pepa Druhy 222 8
        Tomas Jedno 1111 9


        Vector is sorted. OK :-)

        Sorting time: 0.000000 seconds.

        Comparisons: 45
        Swaps: 45
        Moves: 0
        Value Gets: 0
        Value Sets: 0

        ------ Bubble Sort Optimized2 ------
        Tomas Jedno 1111 9
        Pepa Druhy 222 8
        Petr Treti 3 7
        Bozena Ctvrta 4 6
        Pepa Paty 5 5
        Jan Sesty 6 4
        Pepa Sedmy 7 3
        Pepa Osmy 8 2
        Milos Devaty 9 1
        Skluzan Tavic 99 0

        Skluzan Tavic 99 0
        Milos Devaty 9 1
        Pepa Osmy 8 2
        Pepa Sedmy 7 3
        Jan Sesty 6 4
        Pepa Paty 5 5
        Bozena Ctvrta 4 6
        Petr Treti 3 7
        Pepa Druhy 222 8
        Tomas Jedno 1111 9


        Vector is sorted. OK :-)

        Sorting time: 0.000000 seconds.

        Comparisons: 45
        Swaps: 45
        Moves: 0
        Value Gets: 0
        Value Sets: 0

        ------ Shaker Sort ------
        Tomas Jedno 1111 9
        Pepa Druhy 222 8
        Petr Treti 3 7
        Bozena Ctvrta 4 6
        Pepa Paty 5 5
        Jan Sesty 6 4
        Pepa Sedmy 7 3
        Pepa Osmy 8 2
        Milos Devaty 9 1
        Skluzan Tavic 99 0

        Skluzan Tavic 99 0
        Milos Devaty 9 1
        Pepa Osmy 8 2
        Pepa Sedmy 7 3
        Jan Sesty 6 4
        Pepa Paty 5 5
        Bozena Ctvrta 4 6
        Petr Treti 3 7
        Pepa Druhy 222 8
        Tomas Jedno 1111 9


        Vector is sorted. OK :-)

        Sorting time: 0.000000 seconds.

        Comparisons: 45
        Swaps: 45
        Moves: 0
        Value Gets: 0
        Value Sets: 0

        ------ Test stability vicenasobneho trideni s metodou select_sort:
        Bozena Ctvrta 4 6
        Jan Sesty 6 4
        Milos Devaty 9 1
        Pepa Druhy 222 8
        Pepa Osmy 8 2
        Pepa Paty 5 5
        Pepa Sedmy 7 3
        Petr Treti 3 7
        Skluzan Tavic 99 0
        Tomas Jedno 1111 9


---
`$Id: cv07_zadani_basic_sorts_element_struct.md 2635 2024-03-11 15:12:27Z petyovsky $`
