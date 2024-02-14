# Zadání cvičení 2

Dokončete implementaci níže uvedených funkcí realizujících operace pro ADT Zásobník.
Na dodaném příkladu si také vyzkoušejte přístup k elementům datových struktury zásobník pomocí [iterátoru](https://en.wikipedia.org/wiki/Iterator_pattern "Návrhový vzor iterátor představuje jednoduchý abstraktní datový typ, jehož účelem je obecná reprezentace pozice v kontejneru. Umožňuje zobecnit algoritmus procházení všemi elementy kontejneru tak, aby nebyl závislý na konkrétní implementaci kontejneru.") a společného rozhraní. Realizujte zásobník ve dvou variantách:
1. pomocí pole (projekt `TStack_array`),
2. pomocí [lineárního jednosměrně vázaného seznamu](https://cs.wikipedia.org/wiki/Line%C3%A1rn%C3%AD_seznam) (projekt `TStack_list`).

Pro obě varianty napište (stejně pojmenované) funkce rozhraní a iterátory tak, aby bylo možné obě varianty řešení (bez dalších úprav) použít v dodané vzorové funkci `main()`, kde je zásobník využíván v úloze tisku znaků v opačném pořadí, než byly načteny.

Pokuste se odpovědět si na tyto otázky:
 - Co je to zásobník?
 - Jaké má zásobník vlastnosti?
 - Co je to rozhraní?
 - Jaké jsou základní funkce pro práci se zásobníkem?
 - Proč je zásobník vhodný pro realizaci úlohy tisku načtených znaků pozpátku?
 - Jak funguje zásobník realizovaný pomocí pole?
 - Jak funguje zásobník realizovaný pomocí lineárního jednosměrně vázaného seznamu? 
 - Co je to lineární vázaný seznam a jaké varianty takovýchto seznamů existují?
 - Co je to iterátor?
 - Proč iterátory používáme? 
 - Jaká je výhoda iterátorů a jejich jednotného rozhraní?
 - Pro realizaci zásobníku pomocí pole/seznamu bude výhodnější nový element přidávat vždy na začátek/konec a odebírat ze začátku/konce?

Pro řešení jsou připraveny zdrojové texty, z nichž soubor `TStack.c` je nutné doplnit o kód těl funkcí.
Pro vlastní testování je připravena funkce `main()`, která je stejná v obou variantách realizace. Tato funkce provede inicializaci (a na konci "úklid") proměnné reprezentující zásobník. Z důvodu testování jednotlivých funkcí provede uložení a výběr elementů ze zásobníku i jejich tisk využitím iterátoru. Následně zavolá funkci pro tisk řetězce pozpátku `demo_tiskni_slova_pozpatku()`.

Funkce `demo_tiskni_slova_pozpatku()` načte z konzoly znaky, které ukládá po slovech do zásobníku. Slovo obsahuje pouze písmena a číslice. Po ukončení slova, následně znaky ze zásobníku vybírá, ruší a tiskne.

## 1. Realizace zásobníku pomocí pole `TStack_array`
V souboru `TStack.h` je definováno rozhraní pro použití ADT Zásobník.
Dále je zde definován datový typ, jehož hodnoty bude možné do zásobníku ukládat a to pomocí klíčového slova `typedef`. Název typu `::TStackElement` je nutné používat všude tam, kde se pracuje s typem element zásobníku.
Struktura `TStack`, reprezentující vlastní zásobník obsahuje složky: `iValues` - "nedynamické" pole o pevné velikosti definované pomocí `STACK_MAXCOUNT` a `iCount` - počet elementů uložených v zásobníku.
V souboru `TStack.c` dokončete implementaci jednotlivých funkcí, které reprezentují operace ADT Zásobník.

Popis funkcí, které je nutné realizovat v projektu `TStack_array`:

    void stack_init(struct TStack *aStack);
        // Prvotní nastavení vnitřních proměnných zásobníku.
        // Pokud parametr aStack není NULL,
        // nastav počet elementů v zásobníku na nulu.

    bool stack_is_empty(const struct TStack *aStack);
        // Test, zda je zásobník prázdný (tj. neobsahuje žádné elementy).
        // Pokud parametr aStack není NULL a pokud zásobník obsahuje alespoň jeden element, vrať false,
        // jinak vrať true.

    bool /* TStackIterator */ stack_top(const struct TStack *aStack, TStackElement *aValue);
        // Do paměti na kterou ukazuje parametr aValue, ulož (hodnotovou) kopii elementu z vrcholu zásobníku.
        // Pokud parametr aStack není NULL, zásobník není prázdný a parametr aValue typu ukazatel není NULL,
        // zkopíruj hodnotu elementu z vrcholu zásobníku do paměti, na kterou ukazuje parametr aValue, a vrať true,
        // jinak vrať false.

    bool stack_push(struct TStack *aStack, TStackElement aValue);
        // Vložení hodnoty do zásobníku na další volnou pozici.
        // Pokud parametr aStack není NULL a pokud zásobník není plný,
        // umísti hodnotu elementu aValue na vrchol zásobníku,
        // inkrementuj počet elementů v zásobníku a vrať true,
        // jinak vrať false.

    bool stack_pop(struct TStack *aStack);
        // Odstraní element na vrcholu zásobníku (bez vracení jeho hodnoty).
        // Pokud parametr aStack není NULL a zásobník není prázdný,
        // zruš hodnotu na vrcholu zásobníku (tj. dekrementuj počet elementů zásobníku o jeden) a vrať true,
        // jinak vrať false.

    void stack_destroy(struct TStack *aStack);
        // Zrušení obsahu zásobníku.
        // Jelikož tato varianta zásobníku nevyužívá dynamickou alokaci,
        // stačí ke zrušení zásobníku pouze nastavení počtu vložených elementů na nulu (tj. lze zde využít volání funkce stack_init).

Struktura `TStackIterator` obsahuje ukazatel `iStack` odkazující na zásobník, se kterým byl iterátor při svém vzniku propojen (asociován) a také pozici elementu `iPos` na který iterátor aktuálně odkazuje. Iterátor prochází elementy od vrcholu zásobníku k jeho dnu, bez možnosti jejich odstranění - pouze vrací jejich hodnoty.   
Otázkou návrhu, je nutnost si stanovit, jaké by mělo být chování iterátoru v případě, že byl do zásobníku v mezičase přidán nebo ze zásobníku odebrán element (nejlépe je skutečnost patrná v případě smazání elementu, na který právě iterátor "ukazuje").   
**V našem případě předpokládáme, že pokud uživatel změní zásobník, přestane využívat stávající iterátory, které s ním byly asociovány. A vytvoří si případně iterátory zcela nové.**

API pro práci s iterátory:

    struct TStackIterator stack_iterator_begin(const struct TStack *aStack);
        // Slouží pro inicializaci a asociaci/propojení iterátoru se zásobníkem a nastavení na pozici prvního elementu v zásobníku.
        // Pokud předaný zásobník existuje (tj. aStack není NULL) a zásobník není prázdný,
        // ulož do iterátoru adresu asociovaného zásobníku.
        // Dále nastav v iterátoru složku iPos na počátek zásobníku
        // (počátkem zásobníku je element, který se bude první vybírat, tedy jeho vrchol),
        // vrať hodnotu vytvořeného iterátoru.
        // Jinak vrať iterátor s vynulovanými vnitřními složkami.

    bool stack_iterator_is_valid(const struct TStackIterator *aIter);
        // Zjistí, zda iterátor odkazuje na platná data zásobníku.
        // Pokud parametr aIter není NULL a
        // pokud je iterátor asociován s platným zásobníkem a
        // pokud iterátor odkazuje do oblasti platných dat zásobníku, vrať true,
        // jinak vrať false.

    bool stack_iterator_to_next(struct TStackIterator *aIter);
        // Posun na další element v zásobníku.
        // Pokud již další element v zásobníku neexistuje a iterátor ukazuje mimo zásobník,
        // funkce vrací false, jinak vrací true.
        // Pokud je ukazatel aIter platný a iterátor je asociován s platným zásobníkem, pokračuj, jinak vrať false.
        // Pokud iterátor neukazuje složkou iPos na element umístěný na dně zásobníku a také
        // odkazovaná pozice není mimo rozsah daný počtem elementů zásobníku, 
        // posuň iterátor na další pozici v zásobníku a vrať true.
        // Jinak nastav iterátor jako nevalidní (zrušte propojení iterátoru s asociovaným zásobníkem) a vrať false.

    TStackElement stack_iterator_value(const struct TStackIterator *aIter);
        // Vrátí kopii hodnoty elementu zásobníku, na který iterátor aktuálně odkazuje.
        // Pokud je iterátor validní, vrať hodnotu elementu, na který iterátor aktuálně odkazuje,
        // jinak vrať nulový element.

## 2. Realizace zásobníku pomocí lineárního jednosměrně vázaného seznamu `TStack_list`
Oproti realizaci zásobníku pomocí pole, se implementace pomocí lineárního jednosměrně vázaného seznamu (dále jen seznamu) liší pouze vnitřní realizací dat (ve struktuře `TStack`) a vnitřní implementací funkcí ve zdrojovém souboru `TStack.c`. Popis kódu funkcí je podobný - pouze je přizpůsoben realizaci pomocí seznamu. Rozhraní všech funkcí zůstane stejné.
Soubor `TStack.h` opět udává rozhraní ADT Zásobník a soubor `TStack.c` obsahuje implementaci jednotlivých operací.

Datový typ, jehož hodnoty bude možné do zásobníku ukládat, je opětovně definován pomocí klíčového slova `typedef`. Název typu `::TStackElement` je nutné používat všude tam, kde se pracuje s typem element zásobníku.
Struktura `TStack`, určená pro vlastní zásobník obsahuje tentokrát pouze odkaz na první uzel seznamu.

Uzel seznamu je struktura `TStackNode`, která obsahuje data: `iValue` a ukazatel na další uzel seznamu: `iNext`. Poslední uzel seznamu má ukazatel `iNext` nastaven na hodnotu `NULL`.
Pro ADT zásobník s implementací pomocí lineárního jednosměrně vázaného seznamu je z důvodů časové složitosti výhodná strategie přidávání (i odebírání) elementu ze začátku seznamu.

Všimněte si, že definice typu `TStackNode` není **záměrně** uvedena v souboru `TStack.h`, ale je zapsána až v souboru `TStack.c`. Přístup ke složkám struktury `TStackNode` je tak omezen pouze na funkce uvedené v souboru `TStack.c`. Tato skutečnost nám umožňuje skrýt vnitřní implementaci vázaného seznamu před ostatními zdrojovými texty programu a zabránit tak náhodné nebo i úmyslné modifikaci těchto složek. V definici struktury `TStack` v souboru `TStack.h` je potom pro složku `iTop` uveden neúplně definovaný (tzv. neprůhledný / *opaque*) ukazatel na typ `TStackNode`, tento programátorský [idiom](https://en.wikipedia.org/wiki/Programming_idiom) se nazývá *Pointer to IMPLementation - [PIMPL](https://en.wikipedia.org/wiki/Opaque_pointer "Tato technika je popsána v knize Návrhové vzory jako vzor Bridge. Neprůhledné ukazatele jsou způsobem, jak skrýt implementační detaily rozhraní před klienty, kteří využívají jeho funkce. To je výhodné i pro programátora, protože lze vytvořit jednoduché veřejné rozhraní a většinu implementačních detailů skrýt v jiném zdrojovém souboru.")* a budeme ho využívat právě k rozdělení na:
 - **vnější / veřejné** - obecně dostupné rozhraní (*public interface*),
 - **vnitřní / soukromé** - rozhraní nedostupné z ostatních zdrojových souborů (*private interface*).

Popis funkcí, které je nutné realizovat v projektu `TStack_list`:

    void stack_init(struct TStack *aStack);
        // Prvotní nastavení vnitřních proměnných zásobníku.
        // Pokud parametr aStack není NULL,
        // nastav vnitřní složku iTop odkazující na vrcholový uzel seznamu na hodnotu NULL.

    bool stack_is_empty(const struct TStack *aStack);
        // Test, zda je zásobník prázdný (tj. neobsahuje žádné elementy).
        // Pokud parametr aStack není NULL a pokud má vnitřní složka iTop odkazující na
        // vrcholový uzel hodnotu různou od NULL (tj. zásobník není prázdný) vrať false,
        // jinak vrať true.

    bool /* TStackIterator */ stack_top(const struct TStack *aStack, TStackElement *aValue);
        // Do paměti na kterou ukazuje parametr aValue, ulož (hodnotovou) kopii elementu z vrcholu zásobníku.
        // Pokud parametr aStack není NULL, zásobník není prázdný a parametr aValue typu ukazatel není NULL,
        // zkopíruj hodnotu elementu z vrcholu zásobníku do paměti, na kterou ukazuje parametr aValue, a vrať true,
        // jinak vrať false.

    bool stack_push(struct TStack *aStack, TStackElement aValue);
        // Vložení hodnoty do zásobníku na další volnou pozici.
        // Pokud parametr aStack není NULL,
        // vytvoř nový uzel a zapiš do něj předanou hodnotu elementu aValue,
        // umísti nový uzel na vrchol zásobníku (tj. do nově vytvořeného uzlu zapiš odkaz na počáteční uzel seznamu a
        // v zásobníku do odkazu na počáteční uzel iTop vlož adresu nově vytvořeného uzlu).
        // Pokud operace skončila úspěšně, vrať true,
        // jinak vrať false.

    bool stack_pop(struct TStack *aStack);
        // Odstraní element na vrcholu zásobníku (bez vracení jeho hodnoty).
        // Pokud parametr aStack je roven NULL a nebo je zásobník prázdný, vrať false.
        // Jinak zruš uzel na vrcholu zásobníku (tj. nejprve si zapamatuj odkaz na počáteční uzel seznamu,
        // ve složce zásobníku iTop změň odkaz z počátečního uzlu až na druhý uzel; dealokuj první uzel pomocí
        // zapamatovaného odkazu), vrať true.

    void stack_destroy(struct TStack *aStack);
        // Zrušení obsahu zásobníku. Odstraní (dealokuje) všechny uzly, které zásobník obsahuje.
        // Pokud parametr aStack není NULL,
        // nastav a zapamatuj si odkaz na aktuální uzel (tj. počáteční uzel seznamu),
        // nastav odkaz na první uzel zásobníku iTop na NULL.
        // Pro všechny uzly seznamu, opakuj:
        // - zapamatuj si odkaz na rušený uzel,
        // - nastav aktuální uzel na následující uzel,
        // - dealokuj rušený uzel pomocí zapamatovaného odkazu.

Struktura `TStackIterator` obsahuje ukazatel odkazující na zásobník `iStack`, se kterým byl iterátor při svém vzniku asociován a pozici `iActual` (ukazatel na aktuální uzel). Iterátor prochází uzly od vrcholu zásobníku k jeho dnu, bez možnosti jejich odstranění - pouze vrací jejich hodnoty. Procházíme tedy od prvního k poslednímu uzlu seznamu.
V průběhu využívání iterátorů opět předpokládáme neměnnost odkazovaného zásobníku.

API pro práci s iterátory:

    struct TStackIterator stack_iterator_begin(const struct TStack *aStack);
        // Slouží pro inicializaci a asociaci/propojení iterátoru se zásobníkem a nastavení na pozici prvního elementu v zásobníku.
        // Pokud předaný zásobník existuje (tj. aStack není NULL) a zásobník není prázdný,
        // ulož do iterátoru adresu asociovaného zásobníku.
        // Dále nastav iterátor na počátek zásobníku
        // (počátkem zásobníku je element, který se bude první odebírat, tedy jeho vrchol),
        // vrať hodnotu vytvořeného iterátoru.
        // Jinak vrať iterátor s vynulovanými vnitřními složkami.

    bool stack_iterator_is_valid(const struct TStackIterator *aIter);
        // Zjistí, zda iterátor odkazuje na platná data zásobníku.
        // Pokud parametr aIter není NULL a
        // pokud je iterátor asociován s platným zásobníkem (tj. má platnou adresu iStack) a
        // pokud v zásobníku jsou platná data (tj. v odkazu zásobníku je platný ukazatel iTop) a
        // pokud obsahuje iterátor platný ukazatel na aktuální uzel iActual (tj. není NULL), vrať true,
        // jinak vrať false.

    bool stack_iterator_to_next(struct TStackIterator *aIter);
        // Posun na další element v zásobníku.
        // Pokud již další element v zásobníku neexistuje (tj. byl již vyčten
        // poslední element zásobníku a iterátor ukazuje mimo zásobník), funkce vrací false, jinak vrací true.
        // Pokud je ukazatel aIter platný a iterátor je asociován s platným zásobníkem, pokračuj, jinak vrať false.
        // Pokud je iterátor validní (tj. ukazuje na platný uzel seznamu),
        // posuň iterátor na další uzel seznamu,
        // pokud nyní iterátor neodkazuje na uzel s adresou NULL, vrať true, jinak vrať false.

    TStackElement stack_iterator_value(const struct TStackIterator *aIter);
        // Vrátí kopii hodnoty elementu zásobníku, na který iterátor aktuálně odkazuje.
        // Pokud je iterátor validní, vrať hodnotu elementu, na který iterátor aktuálně odkazuje,
        // jinak vrať nulový element.

---
`$Id: cv02_zadani_TStack.md 2588 2024-02-09 17:29:02Z petyovsky $`
