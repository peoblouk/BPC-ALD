# Zadání cvičení 11

Cvičení je věnováno návrhu a implementaci ADT množina (v anglickém jazyce *Set*) (viz přednáška č.5) ukládající prvky do setříděného pole a využívající lineární a binární vyhledávání (viz přednáška č.10). Zadání obsahuje dva projekty:

  1. **Hotovou implementaci ADT Množina využívající flexibilní dynamicky alokované pole s nesetříděnými prvky** - `TSet_unsorted_dynamic_array`.   
     Varianta implementace archivující postupně vkládané elementy v *nesetříděném* [flexibilním](https://en.wikipedia.org/wiki/Flexible_array_member) dynamicky alokovaném poli.   
     K vyhledání elementu s danou hodnotou v poli využívá **algoritmus lineárního vyhledání bez jakýchkoliv optimalizací**, 
     (viz přednáška č.10).

  2. **Nedokončenou implementaci ADT Množina využívající flexibilní dynamicky alokované pole se setříděnými prvky** - `TSet_sorted_dynamic_array`.   
     Varianta implementace archivující postupně vkládané elementy v *setříděném* [flexibilním](https://en.wikipedia.org/wiki/Flexible_array_member) dynamicky alokovaném poli.    
     K vyhledání elementu s danou hodnotou v poli využívá buď: **algoritmus lineárního vyhledávání s optimalizací dřívějšího ukončení vyhledávání** nebo **algoritmus binárního vyhledání**, (viz přednáška č.10).

Obě implementace využívají amortizaci časové složitosti operace vkládání prvku tím, že vždy alokují 
pole o dvojnásobné délce, díky čemuž následné operace přidání prvku probíhají s konstantní amortizovanou časovou 
složitostí.   
Amortizace časové složitosti při operaci odebrání prvku je realizována tak, že teprve při odebrání 75% prvků se provede zmenšení alokované kapacity pole na polovinu (tj. při 25% zaplnění se pole zkrátí na polovinu, aby i po zkrácení bylo pole zaplněno pouze z 50%), (viz přednáška č.2).

---

## Zadání úlohy
V dodaném projektu `TSet_sorted_dynamic_array` dokončete implementaci operací pro vkládání: `set_flex_array_insert()` a odebrání: `set_flex_array_erase()` prvků množiny tak, aby využívaly principu práce s flexibilním dynamicky alokovaným pole umožňujícím amortizaci časové složitosti těchto operací.

Dokončete implementaci servisní funkce `set_flex_array_search()` tak, aby realizovala proces vyhledání prvků v setříděném poli pomocí algoritmu:

  1. lineárního vyhledávání s optimalizací dřívějšího ukončení (při definici makra: `FLEX_ARRAY_SEARCH_VARIANT == LINEAR_SEARCH_OPTIMIZED`),
  2. binárního vyhledávání (při definici makra: `FLEX_ARRAY_SEARCH_VARIANT == BINARY_SEARCH`).

Operace vkládání a odebírání prvků realizujte tak, aby ke své funkci využívaly k vyhledání prvků pouze servisní funkci: `set_flex_array_search()`.

Algoritmy vyzkoušejte na testovací posloupnosti ve funkci `main()`, která je součástí projektu.

Srovnejte výsledné počty porovnání prvků množiny z vašeho programu s hodnotami v souborech s výsledky, které mají název začínající: [`cv11_reports_*.lst`](examples.html).

### Soubory projektu

Oba projekty se liší pouze v obsahu souborů `TSetUnsortedFlexArray.c` a `TSetSortedFlexArray.c`. V projektu `TSet_unsorted_dynamic_array`
jsou realizovány všechna těla funkcí a to včetně: `set_flex_array_search()`, `set_flex_array_insert()` a `set_flex_array_erase()`.

Naopak v projektu `TSet_sorted_dynamic_array` jsou těla funkcí: `set_flex_array_search()`, `set_flex_array_insert()` a `set_flex_array_erase()` připravená pro doplnění zdrojových kódu příslušných algoritmů.
V tomto projektu je také na začátku souboru `TSetUnsortedFlexArray.c` možné pomocí `#define` přepínání jednotlivých variant vyhledávání (lineární s optimalizací, binární).

Soubor `TSetElement.h` obsahuje funkce pro porovnání prvků množiny a mechanismus určení počtu těchto porovnání.
Pro evidenci počtu porovnání hodnot elementů množiny slouží globální proměnná `::gSetElementComparisons`, aktualizovaná vždy při volání funkce `set_element_comparator()`.

Soubory `check.c` a `check.h` představují zdrojové soubory pro kontrolu správné manipulace s dynamicky alokovanou pamětí a se soubory.

Soubor `main.c` obsahuje připravenou základní testovací sekvenci ve formě programu uvnitř funkce `testset2_hardcoded()`. 
Testovací sekvence začíná voláním funkce `set_init()` a končí voláním funkce `set_destroy()`.
Dále jsou zde volány funkce pro vkládání, výběr, tisk a zjišťování přítomnosti 
prvku v množině. Tisk je prováděn pomocí iterátorů `struct TSetIterator`. Takto implementovaná funkce představuje sice jednoduchý způsob, jak
ověřit správnost implementace jednotlivých operací abstraktního datového typu, ale její vytvoření je časově náročné, proto budeme pro testování používat univerzálnější způsob.

Soubory `TSet.c` a `TSet.h` platí pro oba projekty: 

 - Soubor `TSet.h` je hlavičkový soubor, který obsahuje definici
    struktury množiny `struct TSet` a struktury iterátoru pro množinu `struct TSetIterator`.
    Rozhraní funkcí je podobné jako v minulých projektech a názvy funkcí dostatečně popisují 
    jejich činnost.

 - Vlastní množinu reprezentuje struktura `struct TSet` pomocí flexibilního dynamického pole
    (proměnná *iValues*). Proměnná *iCapacity* udává množství alokované/využitelné paměti a proměnná
    *iSize* udává aktuální počet prvků uložených v tomto poli. 

 - Soubor `TSet.c` obsahuje všechny základní funkce požadované pro modifikovatelnou (*mutable*) množinu:
    - `set_init()` - inicializace struktury množiny,
    - `set_size()` - zjištění počtu elementů množiny (neboli velikost resp. kardinalita množiny),
    - `set_is_empty()` - zjištění zda je množina prázdná,
    - `set_is_element_of()` - zjištění přítomnosti dané hodnoty v množině,
    - `set_find()` - vytvoření iterátoru ukazujícího na prvek množiny o dané hodnotě,
    - `set_insert()` - realizující vložení elementu o dané hodnotě do množiny,
    - `set_erase()` - realizující odebrání elementu o dané hodnotě z množiny,
    - `set_destroy()` - ukončení života proměnné typu množina.
   
   A také základní operace pro práci s iterátory této množiny:
    - `set_iterator_begin()` - vytvoření iterátoru směřujícího na první element množiny,
    - `set_iterator_is_valid()` - predikátová funkce vracející `true` , pokud iterátor směřuje na platný element množiny,
    - `set_iterator_to_next()` - posun iterátoru na další element množiny,
    - `set_iterator_value()` - získání hodnoty elementu množiny, na který směřuje iterátor.

V projektu `TSet_unsorted_dynamic_array` obsahují soubory `TSetUnsortedFlexArray.h` a `TSetUnsortedFlexArray.c` dokončenou a fungující implementaci ADT Množina využívající 
 pole s nesetříděnými prvky. V těchto souborech je definován datový typ `TSetUnsortedFlexArray` a jsou zde implementovány servisní funkce pro práci s tímto typem:

 - Soubor `TSetUnsortedFlexArray.c` obsahuje tyto základní funkce pro práci s polem představujícím modifikovatelnou (*mutable*) množinu:
     - `set_flex_array_insert()` - realizuje operaci vložení prvku do pole. Funkce nejprve zjistí, zda prvek již 
       v poli není přítomen. Pokud přítomen není, funkce dále ověří, zda je v poli ještě volné místo pro uložení nového prvku.
       Pokud volné místo není, provede alokaci paměti pro dvakrát větší pole a přesune do něj stávající
       prvky. Následně vloží na konec nově alokovaného pole i hodnotu vkládaného prvku.
       Uvědomte si skutečnost, že tato funkce musí správně pracovat i při operaci prvotního vložení prvku do prázdné množiny.
     - `set_flex_array_erase()` - realizuje operaci odebrání prvku z pole. Funkce nejprve zjistí, zda je prvek v poli přítomen.
       V případě vyprázdnění pole, nebo při poklesu zaplnění na 25% se provede zmenšení délky pole na polovinu
       tak, že se alokuje pole s menším rozměrem a do něj se nakopírují stávající hodnoty. Během kopírování se také provede
       odstranění odstraňovaného prvku tak, že se všechny následující prvky při kopírování do nově alokovaného pole posunou o jednu pozici.
       Po odstranění posledního prvku bude dealokována paměť pro dynamické pole a struktura bude vynulována.
     - `set_flex_array_search()` - realizuje operaci nalezení pozice daného prvku v nesetříděném poli. Funkce postupně
       prochází všechny prvky pole a hledá výskyt prvku s požadovanou hodnotou. Nalezne-li tuto hodnotu, poskytne její pozici
       pomocí argumentu `aPosPtr` a vrátí jako návratovou hodnotu `0`. Nenalezne-li v poli prvek, vrátí `-1` nebo `+1` (dle výsledku posledního porovnání) a pomocí argumentu `aPosPtr` poskytne index v poli, kde bylo vyhledávání ukončeno. Pokud je množina prázdná vrací funkce hodnotu `-2` a pomocí argumentu `aPosPtr` poskytne hodnotu `0`.
 - Výsledky počtu porovnání této varianty implementace jsou dostupné v souboru: [`cv11_reports_TSet_unsorted_dynamic_array_set*.lst`](examples.html).

Projekt `TSet_sorted_dynamic_array` obsahuje soubor `TSetSortedFlexArray.h` s deklaraci typu `TSetSortedFlexArray` a deklarací hlaviček servisních funkcemi pro práci s tímto typem.
Zdrojový soubor `TSetSortedFlexArray.c` obsahuje **nedokončenou** implementaci ADT Množina využívající pole se setříděnými prvky.

 - Soubor `TSetSortedFlexArray.c` obsahuje tyto nedokončené funkce (pro modifikovatelnou / *mutable* množinu). **Pro porovnání prvků nezapomeňte používat předdefinovanou funkci `set_element_comparator()` ze souboru `TSetElement.h`**. Všechny funkce realizujte tak, aby měly shodné vlastnosti jako stejné funkce v projektu: `TSet_unsorted_dynamic_array`:
     - `set_flex_array_insert()` - realizuje amortizované vložení prvku na správnou pozici v poli.
       V případě neúspěchu vrací návratovou hodnotu `false`. Pro nalezení pozice a zařazení využijte funkci: `set_flex_array_search()`. 
     - `set_flex_array_erase()` - realizuje amortizované odebrání prvku ze správného místa v poli.
       V případě neúspěchu vrací návratovou hodnotu `false`. Pro nalezení pozice odebíraného prvku využijte opětovně funkci: `set_flex_array_search()`
     - `set_flex_array_search()` - realizuje nalezení pozice daného prvku v setříděném poli pro obě varianty 
       vyhledávání: lineární s optimalizací `FLEX_ARRAY_SEARCH_VARIANT == LINEAR_SEARCH_OPTIMIZED` i binární `FLEX_ARRAY_SEARCH_VARIANT == BINARY_SEARCH`.
       Nalezne-li prvek s požadovanou hodnotou, předá jeho pozici v proměnné `*aPosPtr` a vrátí hodnotu `0`.
       Nenalezne-li v poli prvek s danou hodnotou, vrátí `-1` nebo `+1` (dle výsledku posledního porovnání) a pomocí argumentu `aPosPtr` poskytne index v poli, kde bylo vyhledávání ukončeno. Pokud je množina prázdná, vrací funkce hodnotu `-2` a pomocí argumentu `aPosPtr` poskytne hodnotu `0`.
 - Vzorové výsledky testování s požadovanými počty operací porovnání prvků jsou pro vaši kontrolu dostupné:
     - pro lineární vyhledávání s optimalizací v souboru: [`cv11_reports_TSet_sorted_dynamic_array_set*_linear_search_optimized.lst`](examples.html),
     - pro binární vyhledávání v souboru: [`cv11_reports_TSet_sorted_dynamic_array_set*_binary_search.lst`](examples.html).


### Úkoly

 1. Prostudujte si oba dodané projekty a jejich zdrojové i ostatní soubory.
 2. Projekt `TSet_unsorted_dynamic_array` je plně funkční, využijte ho tedy k nastudování principu práce s dynamicky alokovaným flexibilním polem využívající amortizaci časové
    složitosti při vkládání a odebírání prvků množiny.
 3. Všimněte si, že nyní již množina na rozdíl od vektoru neumožnuje vnější přímou modifikaci vložených elementů, tzv. [PIMPL](https://en.wikipedia.org/wiki/Opaque_pointer). 
    Není možný ani přímý přístup k prvkům vnitřního pole pomocí funkce, ani změna hodnoty elementu pomocí iterátoru. Zamyslete se nad nutnými důvody takového zapouzdření elementů množiny.
 4. Projekt obsahuje automaticky konfigurovatelné testy v souborech [`testplan_set*.h`](files.html), prostudujte si výhody jeho
    využívání namísto ručně napsaného zdrojového textu ve funkci`testset2_hardcoded()`.
    Uvědomte si, proč je automatický tester realizovaný ve zdrojových souborech `testbench.c` a `testbench.h` výhodnější a naučte se vytvářet vlastní testplán.
 5. Promyslete si jednotlivé implementace operací pro: vkládání, odebrání a vyhledávání elementů množiny pro projekt: `TSet_sorted_dynamic_array`.
 6. Ve funkci `set_flex_array_search()` v projektu `TSet_sorted_dynamic_array` realizujte jednotlivé varianty vyhledávání prvku pomocí **lineárního** i **binárního vyhledávání**.
 7. Implementujte operace pro vkládání: `set_flex_array_insert()`, odebrání: `set_flex_array_erase()` prvků v projektu: `TSet_sorted_dynamic_array` tak, aby využívaly
    principu práce s flexibilním dynamicky alokovaným polem a využívající amortizaci časové složitosti při vkládání a odebírání prvků.
    Vnitřní proces vyhledání prvků realizujte výhradně pomocí servisní funkce: `set_flex_array_search()`.
 8. Realizované algoritmy otestujte na připravených souborech [`testplan_set*.h`](files.html) pomocí funkce `main()`.
    Srovnejte výsledné hodnoty z vašeho programu s hodnotami v souborech s výsledky: [`cv11_reports_*.lst`](examples.html). Zároveň si prohlédněte zobrazené hodnoty z vašich metod a ujasněte si průběh procesu vyhledávání pro lineární a binární vyhledávání.   
    Pokuste se dosáhnout stejných výsledků pro počet porovnání i velikosti alokované paměti jako je uvedeno v přiložených souborech, aniž byste museli modifikovat obsah souboru: `main.c`.

### Rozbor úlohy

Viz přednášky č.2 (Amortizace časové složitosti), č.5 (ADT Množina) a č.10 (Lineární a binární vyhledávání).

---
`$Id: cv11_zadani_TSet_dynamic_array.md 2698 2024-04-12 14:20:47Z petyovsky $`
