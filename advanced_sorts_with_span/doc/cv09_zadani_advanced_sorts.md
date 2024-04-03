# Zadání cvičení 9

Cvičení je věnováno návrhu a implementaci dvou účinnějších třídicích algoritmů (teorii naleznete v přednášce 8):

  1. **Shell sort** (s Hibbardovou posloupností).
  2. **Quick sort** (s volbou prostředního prvku jako tzv. [pivota](https://cs.wikipedia.org/wiki/Pivot)).

---

## Zadání úlohy
Do dodaného projektu `advanced_sorts` si zkopírujte soubory s již vytvořenými algoritmy třídění z minulých cvičení 
`basic_sorts.c` a `basic_sorts.h` pro srovnání rychlosti třídění.

Napište tyto funkce realizující třídicí algoritmy:

 1. Funkce `shell_sort()` - implementace využívající [Hibbardovu posloupnost](https://en.wikipedia.org/wiki/Shellsort#Gap_sequences) k určení délky kroku - viz přednáška 8,
 2. Funkce `quick_sort()` - implementace s volbou prostředního prvku jako pivota, s pomocnou
    funkcí `quick_sort_worker()`.

Algoritmy vyzkoušejte na všech testovacích posloupnostech uložených v souborech, které jsou součástí projektu.


### Soubory projektu

Soubory `basic_sorts.c` a `basic_sorts.h` - zkopírujte si obsah z již Vámi realizované úlohy `basic_sorts` z minulých cvičení.

Soubory `check.c` a `check.h` jsou soubory pro kontrolu manipulace s pamětí a soubory.

Soubory `TVectorElement.h`, `TVector.h` a `TVector.c` jsou funkčně totožné jen aktualizované verze z minulého projektu: `basic_sorts`.

Soubor `main.c` - vychází z minulých projektů věnovaných metodám třídění. Je doplněn o načítání nových testovacích dat ze souborů. 
Dále je doplněn o testování nových třídicích metod. Při testu se z přímých metod třídění volá už jen 
funkce `insert_sort()`, ostatní přímé metody třídění jsou zakomentovány.
Pro srovnání rychlosti nám postačí nejrychlejší z metod přímého třídění tj.: `insert_sort()`.

Soubor `advanced_sorts.h` - hlavičkový soubor s deklaracemi pro nové třídicí funkce.

Soubor `advanced_sorts.c` - Obsahuje následující funkce, do kterých dopište kód pro jejich tělo. Ve funkcích třídicích 
algoritmů je předpřipraven (zakomentovaný) kód pro ladicí textový výpis na standardní výstup demonstrující postup algoritmu.
Názvy proměnných v předpřipraveném kódu předpokládají použití názvů proměnných tak, jak byly uvedeny v přednáškách.

  - `print_element_if()` - funkce pro tisk hodnoty na základě podmínky. Pokud má první parametr 
     (typu `bool`) hodnotu: `true`, potom se vytiskne předaná hodnota. V opačném případě: `false`, 
     se vytisknou na danou pozici namísto hodnoty pouze zástupné znaky (tj. tečka a mezera). 
     Tento způsob je vhodný právě pro ladicí výpisy z metod vytvářející podposloupnosti nebo využívající princip: [Rozděl a panuj](https://cs.wikipedia.org/wiki/Rozd%C4%9Bl_a_panuj_(algoritmus)).

  - `shell_sort()` - První výpis tiskne vzdálenost (krok `h`) mezi zpracovávanými prvky v dané iteraci (tento výpis tedy bude na začátku těla cyklu, který určuje aktuální velikost kroku). 
     Druhý tisk vytiskne aktuální stav tříděného pole (bude na začátku cyklu pro třídění prvků a 
     bude tisknout při změně kroku nebo posunu počáteční pozice pro stejný krok). 
     Třetí tisk zobrazí prvky, se kterými se v daném kroku manipulovalo a na místě ostatních 
     zobrazí tečky (bude tedy na konci vnitřního cyklu po manipulaci s každou pozicí/prvkem). 
     Vzorový výstup tisku je přiložen ve výsledkových tiskových souborech: [`doc/cv09_reports*.lst`](examples.html).

  - `quick_sort_worker()` - slouží jako funkce připravená pro rekurzivní volání. 
     Tisk se provede při vstupu do funkce, kdy se vytisknou hodnoty v té části, se kterou se bude v daném kroku pracovat.

  - `quick_sort()` - [wrapper funkce](https://en.wikipedia.org/wiki/Wrapper_function) pro předání volání do funkce `quick_sort_worker()`.


Testovací soubory [`reversed_data.txt`](pages.html), [`select_sort_data.txt`](pages.html), [`sorted_data.txt`](pages.html), [`quick_sort_data1.txt`](pages.html) 
(a další tři) jsou připraveny pro testování algoritmů se zaměřením na specifické vstupní posloupnosti hodnot.
Například posloupnost hodnot v souboru: [`quick_sort_data2.txt`](pages.html) by měl představovat nejhorší případ pro třídění metodou *Quick sort*. Metoda by 
měla tuto posloupnost zpracovat s maximální možnou (tj. kvadratickou) mírou časové složitosti. 
Porovnejte také u jednotlivých metod celkové počty operací nutné pro setřídění posloupnosti [`sorted_data.txt`](pages.html), která představuje již setříděnou posloupnost.


### Úkoly

  1. Prostudujte si dodaný projekt a jeho soubory.
  2. Promyslete si jednotlivé implementace obou třídicích algoritmů.
  3. Realizujte jednotlivé implementace obou třídicích algoritmů.
  4. Odstraňte komentáře pro ladicí tisk a odlaďte zobrazení údajů o průběhu třídění.
  5. Realizované algoritmy otestujte na přiložených souborech. 
     Srovnejte výsledné průběžné hodnoty z vašeho programu s hodnotami v souborech s příklady výstupů v souborech: [`doc/cv09_reports*.lst`](examples.html).   
     Zároveň si prohlédněte zobrazené hodnoty z vašich metod a ujasněte si průběh třídění pro jednotlivé vstupní soubory dat.
  6. Pokuste se dosáhnout stejných výstupů programu jako v [přiložených souborech](examples.html), aniž byste museli modifikovat obsah souboru: 
     `main.c`. Výpočetní časy se budou samozřejmě na Vašem HW lišit.

### Rozbor úlohy

Viz přednáška 8.

---

## Zadání bonusové úlohy
Do dodaného projektu `advanced_sorts_with_span` zkopírujte soubory s již vytvořenými algoritmy třídění z minulých úloh 
`basic_sorts.c`, `basic_sorts.h` a `advanced_sorts.c` pro srovnání rychlosti třídění.

Napište znovu funkce realizující třídicí algoritmus *Quick Sort* využívající nový datový typ `struct TSpan`, který umožňuje omezený pohled na elementy jiného vektoru a to včetně definice omezení intervalu pozic do původního vektoru. Je dobré si uvědomit, že typ Span představuje pouze nový pohled na elementy původního vektoru a je proto implementován tak, aby data původního vektoru znovu nealokoval, nekopíroval ani si nepřevzal jejich vlastnictví:

 1. Prostudujte si rozhraní datového typu `struct TSpan` ve zdrojovém souboru `TSpan.h`.
 2. Prostudujte si tělo funkce `quick_sort_with_span()` a reimplementujte metodu *Quick sort* s využitím typu `struct TSpan` v pomocné
    funkci `quick_sort_with_span_worker()`.
 3. Pravděpodobně zjistíte, že tato reimplementace algoritmu *Quick Sort* představuje čitelnější zdrojový text.
 4. Prakticky ověřte, zda je tato nová implementace využívající datový typ *Span* časově efektivnější, než implementace původní.
 5. Diskutujte zjištěné poznatky.

Algoritmus opět vyzkoušejte na všech testovacích posloupnostech uložených v souborech, které jsou součástí projektu. Poté zakomentujte ve funkci `main()`
volání metody `insert_sort()` a změřte dobu třídění vektoru o velikosti 5 000 000 elementů, které obsahují náhodné hodnoty.

---

`$Id: cv09_zadani_advanced_sorts.md 2675 2024-03-25 15:16:28Z petyovsky $`
