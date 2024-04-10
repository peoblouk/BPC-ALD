# Zadání cvičení 10

Cvičení je věnováno návrhu a implementaci dalších dvou výkonnějších třídicích algoritmů:

  1. **Třídění s využitím haldy - Heap sort** - (*varianta top-down*) - viz přednáška 9.
  2. **Třídění slučováním - Merge sort** - (*varianta top-down s pomocí rekurzivního rozkladu*) - viz přednáška 9.

Obě metody mají několik variant řešení. I když je princip stejný, nejedná se o drobné 
modifikace, ale spíše o rozdílný přístup k řešení a použití pomocných programových technik.

---

## Zadání úlohy
Do dodaného projektu si zkopírujte soubory s již vytvořenými algoritmy třídění z minulých cvičení 
`basic_sorts.c`, `basic_sorts.h`, `advanced_sorts.c` a `advanced_sorts.h` pro srovnání 
rychlosti třídění.

Navrhněte, naimplementujte a na přiložených souborech odzkoušejte algoritmy **Heap sort** a **Merge sort**. Zvolte si jedno z možných řešení pro každý z algoritmů.

Pro obě metody jsou dostupné dvě možnosti implementace (dokončete alespoň jednu z možných implementací pro každou z metod):

 1. **Heap sort**:   
    a) přístup k prvku pomocí API `THeap` využívající k přístupu do uzlu haldy indexaci - realizováno funkcí `heap_sort()`.   
    b) přístup k prvku pomocí API `THeap` využívající k přístupu do uzlu haldy iterátor - realizováno funkcí `heap_sort_with_iterators()`.

 2. **Merge sort**:   
    a) přístup k prvku pomocí indexu pozice ve vektoru - realizováno funkcí `merge_sort()`.   
    b) přístup k prvku pomocí API `TSpan` zajišťující omezený pohled na původně celý vektor - 
    realizováno funkcí `merge_sort_with_span()`.


Algoritmy vyzkoušejte (pokud možno) na všech testovacích posloupnostech uložených v souborech, které jsou součástí projektu.


### Soubory projektu

Soubory `basic_sorts.c` a `basic_sorts.h` - zkopírujte si obsah z již Vámi realizované úlohy `basic_sorts` z minulých cvičení.

Soubory `advanced_sorts.c` a `advanced_sorts.h` - zkopírujte si obsah z již Vámi realizované úlohy `advanced_sorts` z minulých cvičení.

Soubory `check.c` a `check.h` jsou soubory pro kontrolu manipulace s pamětí a soubory.

Soubory `TVectorElement.h`, `TVector.h` a `TVector.c` jsou funkčně totožné jen aktualizované verze z minulých projektů. 
Všimněte si např. ve `struct TVector` změnu struktury pro statistiku na ukazatel - z důvodu sdílení 
dat více funkcemi/proměnnými.

Soubor `main.c` - vychází z minulé verze. Je doplněn o načítání nových testovacích dat ze souborů. 
Dále je doplněn o testování nových třídicích metod. 
Srovnání se provádí s algoritmy funkcí `insert_sort()`, `shell_sort()` a `quick_sort()`.

Soubor `powerful_sorts.h` - slouží jako společný hlavičkový soubor pro nové funkce.

Soubory pro realizaci **Heap sortu**:

1. Pro první variantu soubor `heap_sort.h` jako hlavičkový soubor pro funkci `heap_sort()`. K němu příslušný
    zdrojový soubor `heap_sort.c` pro vlastní realizaci funkce a pro funkce pomocné.

2. Pro druhou variantu soubor `heap_sort_with_iterators.h` jako hlavičkový soubor pro 
    funkci `heap_sort_with_iterators()`. K němu příslušný
    zdrojový soubor `heap_sort_with_iterators.c` pro vlastní realizaci funkce a pro funkce pomocné.

Funkce pro obě varianty mají jako vstupní parametr `struct TVector` s daty pro třídění. Rozdíl je v tom,
že pro další činnost používá první varianta `struct THeap`, zatímco druhá varianta
využívá `struct THeapIterator`. Obě varianty používají pro přesuny/úpravu datových členů pomocné funkce:
 `sift_down()` a `heapify()` (s parametry `struct THeap` resp. `struct THeapIterator`).

Struktury `struct THeap` a `struct THeapIterator` a funkce pro manipulaci s nimi jsou
v souborech `THeap.c` a `THeap.h`.

`struct THeapIterator` je iterátor navázaný na strukturu `struct THeap`. Struktura 
`struct THeap` realizuje haldu, ale pro data využívá strukturu `struct TVector`. Pro tuto strukturu jsou
připraveny funkce, které pracují s touto strukturou jako s haldou (implementují funkce
používané pro práci s daty, tak, že respektují postupy a 
pravidla, která platí pro haldu). Znamená to, že skutečná/vnitřní realizace je vektor, ale
halda je strom. Tato skutečnost je například vidět ve funkcích: `heap_pos_to_parent()`, `heap_pos_to_left()` a
`heap_pos_to_right()`, kdy získáme index na rodičovský uzel případně na levého resp. pravého následníka aktuálního uzlu 
(prvky jsou odkazovány přes indexy ve vektoru).


Soubory pro realizaci **Merge sortu**:

1. Pro první variantu, která používá pro manipulaci s daty struktury `struct TVector`,
    je přichystán soubor `merge_sort.h` jako hlavičkový soubor pro funkce `merge_sort()`. K němu příslušný
    zdrojový soubor `merge_sort.c` pro vlastní realizaci funkce a pro funkce pomocné:

- `merge_sort()` - wrapper funkce pro předání volání do funkce `merge_sort_worker()`.
- `merge_sort_worker()` - funkce realizuje vlastní algoritmus obsahující rekurzivní volání.
- `mergify()` - pomocná funkce pro zatřiďování (sloučování) dvou již setříděných podposloupností.

2. Pro druhou variantu (with_span), která používá pro manipulaci s daty strukturu
    `struct TSpan`, je přichystán hlavičkový soubor `merge_sort_with_span.h` s deklarací rozhraní 
    pro funkci `merge_sort_with_span()`. K němu příslušný
    zdrojový soubor `merge_sort_with_span.c` pro vlastní realizaci funkce a pro funkce pomocné:

- `merge_sort_with_span()` - wrapper funkce pro předání volání do funkce `merge_sort_worker()`.
- `merge_sort_worker()` - funkce realizuje vlastní algoritmus obsahující rekurzivní volání.
- `mergify()` -  pomocná funkce pro zatřiďování (sloučování) dvou již setříděných podposloupností (spanů).

Soubor `TSpan.h` obsahuje pomocnou strukturu `struct TSpan` a funkce pro práci s ní.
Vnitřní reprezentace dat je opět vytvořena pomocí `struct TVector`. 
Struktura `TSpan` a funkce pro práci s ní tvoří interface/mezivrstvu mezi uživatelem a daty vektoru. 
Struktura `TSpan` je navržena tak, aby umožňovala pohled na data vekoru jiným způsobem, aniž je bylo nutné alokovat v paměti 
jejich novou kopii.

V souboru `TSpan.h` jsou tyto hlavní funkce (pro zkrácení zde nebudeme uvádět všechny funkce, obzvlášť ne ty, jejichž činnost je zřejmá
z názvu, popřípadě funkce volající stejně pojmenované funkce pro vnitřní proměnnou \ref TSpan::iVector "\c iVector ":

- `span_create()` slouží k inicializaci struktury `struct TSpan` pomocí `struct TVector`.
- `span_create_left()` a varianty `span_create_right()`, `span_create_range()` slouží k rozdělení dat 
    na jednotlivé části, kdy vrátí strukturu obsahující (odkaz) na požadovanou část dat. 
    Tyto funkce jsou určeny pro vstupní parametr typu `struct TVector` a vrací typ `struct TSpan`.   
    Pro vstupní parametr typu `struct TSpan` jsou určeny funkce `span_create_left_subspan()` a varianty `span_create_right_subspan()` případně `span_create_range_subspan()`,  vracející opět typ `struct TSpan`.
- pro datový typ `struct TSpan` je realizován i iterátor prostřednictvím struktury `struct TSpanIterator` 
    a funkcí, které ji využívají (pro realizaci zadání, ale není potřeba).


Testovací soubory jsou připraveny pro testování algoritmů se zaměřením na specifické vstupní posloupnosti hodnot.
Například posloupnost hodnot v souboru: `quick_sort_data2.txt` by měla představovat nejhorší případ pro třídění metodou *Quick sort*. Metoda *Quick sort* by 
měla posloupnost zpracovat za maximální časovou složitost, porovnejte časovou složitost třídění této posloupnosti pomocí metod: **Heap sort** a **Merge sort**.


### Úkoly

  1. Prostudujte si dodaný projekt a jeho soubory.
  2. Promyslete si jednotlivé implementace třídění.
  3. Realizujte jednotlivé (vámi vybrané varianty) implementace třídění **Heap sort** a **Merge sort**.
  4. Odstraňte komentáře pro ladicí tisk a odlaďte zobrazení údajů o průběhu třídění.
  5. Realizované algoritmy otestujte na přiložených souborech.
     Srovnejte výsledné průběžné hodnoty z vašeho programu s hodnotami v souborech s výsledky, které mají název začínající [`cv10_reports_*.lst`](examples.html). Zároveň si prohlédněte zobrazené hodnoty z vašich metod a ujasněte si průběh třídění pro jednotlivé vstupní soubory dat.   
     Pokuste se dosáhnout stejných výsledků jako v přiložených souborech, aniž byste museli modifikovat obsah souboru: 
     `main.c`. Výpočetní časy se budou samozřejmě na Vašem HW lišit.

### Rozbor úlohy

Viz přednáška č.9.

---
`$Id: cv10_zadani_powerful_sorts.md 2686 2024-04-05 14:16:02Z petyovsky $`
