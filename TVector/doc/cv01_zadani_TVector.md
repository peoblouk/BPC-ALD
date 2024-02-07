# Zadání cvičení 1

Dokončete implementaci níže uvedených funkcí a ověřte časovou i paměťovou složitost funkce `vector_resize()`. Tato funkce má za úkol změnit velikost vektoru.
Funkce alokuje novou oblast paměti o velikosti danou počtem prvků a vloží do ní hodnoty z původního vektoru. Původní oblast paměti následně dealokuje.

Pro řešení jsou připraveny zdrojové texty, z nichž některé je nutné doplnit o chybějící kód (viz soubor `TVector.c`).
Pro vlastní testování je v souboru `main.c` připravena funkce `resize_testing()`, která zavolá testovanou funkci `vector_resize()`, změří dobu jejího trvání a její paměťové nároky.
Doba trvání se měří pomocí času získaného funkcí [`timespec_get()`](https://en.cppreference.com/w/c/chrono/timespec_get), která vrací hodnotu typu struktura `timespec` (knihovna [`time.h`](https://en.cppreference.com/w/c/chrono)). V této struktuře jsou dva časové údaje, jeden udává celé sekundy a druhý nanosekundy (od určitého okamžiku v minulosti). Výpočet doby trvání (desetinné číslo v sekundách) se provádí ve funkci `difftimespec()` na základě dvou časových okamžiků.
Testovací funkce se volá z `main()` opakovaně s různými požadavky na množství alokované paměti. Parametrem funkce je počet elementů / prvků vektoru. Promyslete si, jaký je vztah mezi množstvím alokované paměti a počtem požadovaných elementů / prvků vektoru.

Důvod existence souboru `TVectorElement.h` a v něm definovaném typu `::TVectorElement` je v oddělení definice typu element vektoru od realizace [ADT](https://cs.wikipedia.org/wiki/Abstraktn%C3%AD_datov%C3%BD_typ) `TVector` (soubory `TVector.c` a `TVector.h`). Pokud v budoucnu změníme datový typ elementu, bude nutno upravit pouze soubor `TVectorElement.h`. Ostatní zdrojové soubory s definicí vektoru, již zůstanou beze změny. Změna datového typu element představuje pro základní datové typy pouze triviální úpravu alias definice typu `::TVectorElement` (viz klíčové slovo `typedef`) a úpravu hodnoty makra `TVECTOR_ELEMENT_FRMSTR`. Pokud ovšem bude novým typem složený datový typ (např. komplexní číslo, zlomek, jméno a příjmení), bude třeba modifikovat i těla ostatních funkcí v tomto hlavičkovém souboru. Tyto funkce představují tzv. [API](https://cs.wikipedia.org/wiki/API) pro `::TVectorElement`, tedy zajišťují: načtení / uložení hodnot elementu do souboru, způsob jejich porovnávání, a vygenerování náhodné hodnoty elementu. Z toho také plyne, že nadřazený ADT `TVector` bude pro libovolnou realizaci typu `::TVectorElement` vždy schopen všechny své elementy: načíst / uložit do souboru, a také seřadit dle velikosti (viz pozdější cvičení).

Ujasněte si rozdíl mezi knihovními funkcemi [`malloc()`](https://en.cppreference.com/w/c/memory/malloc) a [`calloc()`](https://en.cppreference.com/w/c/memory/calloc). Obě funkce dynamicky alokují paměť o velikosti bajtů. Funkce `calloc(počet, velikost)` si oba parametry (počet a velikost typu) vynásobí sama. [Některé implementace](http://locklessinc.com/articles/calloc/) navíc kontrolují, z bezpečnostních důvodů, případné přetečení výsledku operace násobení. Od normy jazyka [C23](https://en.wikipedia.org/wiki/C23_(C_standard_revision)) je tato kontrola již povinně naimplementována v těle funkce `calloc()`. Tato funkce následně po alokaci (na rozdíl od funkce `malloc()`) zajistí efektivní nulování přidělené paměti s pomocí mechanismu jádra operačního systému. Z toho plyne, že `malloc()` použijeme tam, kde paměť (ihned) plníme svými daty. Funkci `calloc()` použijeme v okamžiku, kdy nemáme data k přiřazení do prvků pole, nebo chceme využít automatickou inicializaci prvků na nuly.

1. Prostudujte dodaný projekt a v něm dodané zdrojové kódy.
   - Projekt si překopírujte do své pracovní složky a otevřete ve VS pomocí souboru `TVector.sln`.
   - Doporučujeme si upravit nastavení formátování zdrojového kódu označované v literatuře jako [Whitesmiths](https://en.wikipedia.org/wiki/Indentation_style#Whitesmiths_style) (`Tools` > `Options` > `TextEditor` > `C & C++` > `Formating` > `Indentation` a zde zaškrtněte první položku `Indent braces`).
   - Nastavení prostředí VS si uložte pro rychlejší načtení v dalších cvičeních (`Tools` > `Import and Export Settings` > `Export selected environment settings` > `Next` > `Next` > uložit soubor na plochu a zvolit `Finish`).
   - Prostudujte si funkce v souboru `main.c`.
   - Nastudujte si, jak pracuje tzv. [aserce](https://cs.wikipedia.org/wiki/Aserce). Jedná se o funkci `assert()`, která je definována ve standardním souboru [`assert.h`](https://en.wikipedia.org/wiki/Assert.h).
   - Prostudujte si soubor `TVectorElement.h`, který obsahuje základní definice dat a pomocných funkcí (snažte se je ve svém kódu použít). Popište a zdůvodněte význam typu `::TVectorElement` a makra `TVECTOR_ELEMENT_FRMSTR` (v případě nutnosti se inspirujte ve funkcích v `TVectorElement.h`).
   - Prostudujte si soubor `TVector.h` (mimo sekci s iterátory) a ujasněte si jeho význam v projektu.
   - Prostudujte soubor `TVector.c`.

2. Funkce `vector_init()` alokuje pole o dané délce s nulovými elementy a inicializuje složky struktury `TVector`, jejíž adresa je předána jako parametr. Předpokládáme, že tato funkce zajišťuje prvotní inicializaci proměnné vektor.

        // test vstupních parametrů - má-li parametr typu ukazatel hodnotu NULL, ukončíme funkci
        // alokujeme pole dané délky a jeho počáteční adresu uložíme do dočasné proměnné
        // pokud došlo při alokaci k chybě, funkci ukončíme
        // k alokaci využijeme funkci calloc, pole tedy již bude naplněno nulami
        // ve struktuře nastavíme složky reprezentující počet prvků pole (iSize) a počáteční adresu pole (iValues)
        // při úspěchu funkce vrátíme hodnotu true, jinak false

3. Funkce `vector_init_random()` vytvoří vektor o dané délce s náhodnými hodnotami - do proměnné, která je předána jako parametr.
 Od funkce `vector_init()` se liší vložením náhodných hodnot do elementů vektoru (použijte připravenou funkci `vector_element_random_value()` z `TVectorElement.h`).

        // test vstupních parametrů - má-li parametr typu ukazatel hodnotu NULL, ukončíme funkci
        // alokujeme pole dané délky a jeho počáteční adresu uložíme do dočasné proměnné
        // pokud došlo při alokaci k chybě, funkci ukončíme
        // pole naplníme hodnotami z funkce: vector_element_random_value()
        // ve struktuře nastavíme složky reprezentující počet prvků pole (iSize) a počáteční adresu pole (iValues)
        // při úspěchu funkce vrátíme hodnotu true, jinak false

4. Funkce `vector_init_file()` vytvoří vektor o dané délce s danými hodnotami - do proměnné, která je předána jako parametr. Druhým parametrem je proměnná typu `FILE*` představující otevřený textový soubor obsahující informaci o počtu elementů vektoru a následným výčtem jednotlivých hodnot vektoru. (K načtení hodnoty elementu použijte připravenou funkci `vector_element_load_file()` ze zdrojového souboru `TVectorElement.h`).

        // test vstupních parametrů - má-li některý z parametrů typu ukazatel hodnotu NULL, ukončíme funkci
        // načteme počet elementů vektoru ze souboru (s kontrolou úspěšnosti načtení)
        // alokujeme pole dané délky a jeho počáteční adresu uložíme do dočasné proměnné
        // pokud došlo při alokaci k chybě, funkci ukončíme
        // postupně načteme hodnoty elementů vektoru ze souboru do paměti (s kontrolou úspěšnosti načtení)
        // ve struktuře nastavíme složky reprezentující počet prvků pole (iSize) a počáteční adresu pole (iValues)
        // při úspěchu funkce vrátíme hodnotu true, jinak false

5. Funkce `vector_destroy()` uvolní získané zdroje (alokovanou paměť) a nastaví vnitřní složky proměnné `TVector` pro budoucí další použití.

        // test vstupních parametrů - má-li parametr typu ukazatel hodnotu NULL, ukončíme funkci
        // uvolnění paměti
        // vynulujeme složky struktury TVector

6. Funkce `vector_value()` vrátí hodnotu elementu na dané pozici vektoru.

        // pomocí aserce ověřte validitu vstupních parametrů a otestujte, zda je požadovaná pozice (index) elementu uvnitř alokovaného pole
        // vrátíme hodnotu elementu

7. Funkce `vector_size()` vrátí počet elementů vektoru.

        // zkontrolujeme validitu vstupních parametrů a existenci naalokované paměti pro pole
        // vrátíme počet elementů vektoru nebo hodnotu 0 v případě chyby

8. Funkce `vector_resize()` změní počet prvků alokovaného pole na novou velikost a uchová hodnoty platných elementů. Předpokládá se, že vektor předaný jako parametr již byl korektně inicializován a může také obsahovat platná data, které musí být případně odalokovány, pokud nebudou využity.

        // test vstupního parametru - má-li parametr typu ukazatel hodnotu NULL, ukončíme funkci
        // pokud je počet prvků původního vektoru shodný s nově požadovaným počtem (aNewSize), ukončíme funkci s hodnotou true
        // pokud je nově požadovaný počet prvků vektoru nulový, odalokujeme paměť pole, vynulujeme jednotlivé složky a ukončím funkci s hodnotou true
        // alokujeme paměť pro pole vektoru o novém počtu prvků (malloc/calloc) a jeho počáteční adresu uložíme do dočasné proměnné
        // pokud došlo při alokaci k chybě, funkci ukončíme
        // postupně zkopírujeme hodnoty prvků z původního pole do nového pole 
        // zvolte správný počet kopírovaných prvků, pokud kopírujete do menšího i pokud kopírujete do většího, zbývající hodnoty pole vynulujte
        // dealokujeme paměť původního pole
        // ve struktuře nastavíme složky reprezentující nový počet prvků pole a počáteční adresu nového pole
        // při úspěchu funkce vrátíme hodnotu true, jinak false

9. Funkce `vector_clone()` vytvoří kopii elementů `aVector` (zdroj) do proměnné `aVectorClone`. Předpokládáme, že proměnná `aVectorClone` může již obsahovat platná data, které musí být případně odalokovány, pokud nebudou využity.

        // test vstupních parametrů - má-li některý z parametrů typu ukazatel hodnotu NULL, ukončíme funkci
        // kontrola, zda originál a kopie jsou dvě různé proměnné typu TVector - pokud jsou stejné, funkci ukončíme
        // pokud není originál alokován - odalokujte i paměť kopie, vynulujte její složky a funkci ukončete
        // alokujeme pole dané délky (pro kopii) a jeho počáteční adresu uložíme do dočasné proměnné
        // pokud došlo při alokaci k chybě, funkci ukončíme
        // překopírujeme hodnoty elementů do nově alokovaného pole
        // pokud kopie obsahovala dříve alokovanou paměť, tuto paměť dealokujeme
        // do složek výstupní proměnné aVectorClone zapíšeme hodnoty reprezentující počet prvků nového pole (iSize) a počáteční adresu nového pole (iValues)
        // při úspěchu funkce vrátíme hodnotu true, jinak false

10. Funkce `vector_store_file()` představuje opak k načítání vektoru ze souboru. Data uloží ve stejném formátu, aby bylo možné jejich opětovně načtení pomocí funkce `vector_init_file()`. (K uložení hodnoty elementu do souboru použijte připravenou funkci `vector_element_store_file()` ze zdrojového souboru `TVectorElement.h`).

        // test vstupních parametrů - má-li některý z parametrů typu ukazatel hodnotu NULL, ukončíme funkci
        // do souboru uložíme počet elementů vektoru (s kontrolou úspěšnosti zápisu)
        // do souboru postupně uložíme hodnoty všech elementů vektoru (s kontrolou úspěšnosti zápisu)
        // při úspěchu funkce vrátíme hodnotu true, jinak false

11. Funkce `vector_set_value()` má za úkol zapsat zadanou hodnotu na danou pozici v poli a provést kontroly platnosti takového operace.

        // pomocí aserce ověřte validitu vstupních parametrů a otestujte, zda je požadovaná pozice (index) elementu uvnitř alokovaného pole
        // zapíšeme hodnotu na danou pozici v poli


---
`$Id: cv01_zadani_TVector.md 2580 2024-02-05 14:23:12Z petyovsky $`
