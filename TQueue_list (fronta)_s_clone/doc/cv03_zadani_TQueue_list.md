# Zadání cvičení 3

Dokončete implementaci níže uvedených funkcí realizujících operace pro ADT fronta (Queue). V dodaném příkladu také implementujte operace pro mutable iterátor ADT fronta a následně při jeho použití ověřte správnou funkci tohoto rozhraní.
Frontu realizujte pomocí [lineárního jednosměrně vázaného seznamu](https://cs.wikipedia.org/wiki/Line%C3%A1rn%C3%AD_seznam) (datový typ: `struct TQueue`; datový typ uzlu seznamu `struct TQueueNode`).
Po odladění varianty pro stávající `::TQueueElement` můžete zkusit implementaci upravit pro typ `double`, včetně definice tiskového řetězce pro tento typ (viz minulá cvičení).

Pokuste se odpovědět si na tyto otázky:
 - Co je ADT Queue/fronta?
 - Jak tento abstraktní datový typ funguje?
 - Pro jaký typ úloh je ADT fronta vhodný?
 - Je vhodnější vkládat uzly *na začátek* nebo *na konec* lineárního jednosměrně vázaného seznamu (a vybírat z opačného konce)? Proč?

Pro řešení jsou připraveny zdrojové texty v souborech.
Všimněte si, že definice typu `TQueueNode` opětovně a **záměrně** není uvedena v souboru `TQueue.h`, ale je zapsána až v souboru `TQueue.c`. Přístup ke složkám struktury `TQueueNode` je tak omezen pouze na funkce uvedené v souboru `TQueue.c`. Tato skutečnost nám umožňuje skrýt vnitřní implementaci vázaného seznamu před ostatními zdrojovými texty programu a zabránit tak náhodné nebo i úmyslné modifikaci těchto složek. V definici struktury `TQueue` v souboru `TQueue.h` je potom pro složky `iFront` a `iBack` uveden neúplně definovaný ukazatel na typ `TQueueNode`, tento programátorský [idiom](https://en.wikipedia.org/wiki/Programming_idiom) *Pointer to IMPLementation - [PIMPL](https://en.wikipedia.org/wiki/Opaque_pointer "Tato technika je popsána v knize Návrhové vzory jako vzor Bridge. Neprůhledné ukazatele jsou způsobem, jak skrýt implementační detaily rozhraní před klienty, kteří využívají jeho funkce. To je výhodné i pro programátora, protože lze vytvořit jednoduché veřejné rozhraní a většinu implementačních detailů skrýt v jiném zdrojovém souboru.")* jsme již využili v předchozích cvičeních.

Soubor `main.c` obsahuje volání jednotlivých funkcí tak, jak byly používány i v předchozích projektech. Funkce `print_element()` vytiskne hodnotu elementu, na který aktuálně ukazuje iterátor předaný jako parametr.

Funkce `is_element_with_value_43()` je definice predikátu (funkce vracející `bool`, která je volaná na místě výrazu v podmínce) používaného v programu. Tento predikát testuje, zda hodnota v elementu na který odkazuje iterátor, je rovna číslu 43.
Povšimněte si jeho použití - do dalších funkcí se předává pouze název tohoto predikátu (název funkce je totiž jejím vstupním bodem - adresou, na kterou se předá řízení při provádění jejího těla).

Dále se podívejte do hlavičkového souboru `TQueue.h`, jak vypadá definice parametru, který očekává (ukazatel na) funkci, a jak se s ním dále pracuje, např.:`queue_find_if()`. V souboru `TQueue.h` jsou dále definice struktur a funkcí. Pro datový element, který je obsahem fronty, je opět definován zobecněný typ `::TQueueElement`.

Vlastní fronta je zapouzdřena ve struktuře `TQueue`, která obsahuje dvě složky `iFront` a `iBack` typu ukazatel, které směřují vždy na první (resp. na poslední) uzel seznamu. Na čele fronty je přítomen nejdříve vložený element. Na konci fronty je přítomen naposledy vložený element. Čelo fronty představuje počátek seznamu, konec fronty představuje konec seznamu.

Elementy fronty jsou uloženy v uzlech seznamu reprezentovaných strukturou `TQueueNode`, která obsahuje složky: hodnota elementu fronty: `iValue` a ukazatel na další uzel seznamu: `iNext`. Poslední uzel seznamu má ukazatel `iNext` nastaven na hodnotu `NULL`.

## Funkce pro implementaci `TQueue_list` a popis jejich činnosti:
V následující kapitole je popsána stručně činnost - správné návratové hodnoty si již doplňte sami.

	void queue_init(struct TQueue *aQueue);
		// Prvotní nastavení vnitřních proměnných fronty.
		// Pokud parametr aQueue není NULL,
		// nastav vnitřní složky odkazující na počáteční a koncový uzel na hodnotu NULL.

	bool queue_is_empty(const struct TQueue *aQueue);
		// Test, zda je fronta prázdná - (tj. fronta neobsahuje žádné elementy).
		// Pokud parametr aQueue není NULL,
		// assercí otestuj konzistenci ukazatelů na poč. a konc. uzel (ukazatele musí být buď: oba platné nebo oba neplatné),
		// pokud má vnitřní složka iFront odkazující na počáteční uzel seznamu hodnotu různou od NULL (tj. fronta není prázdná), vrať false,
		// jinak vrať true.

	bool /* TQueueIterator */ queue_front(const struct TQueue *aQueue, TQueueElement *aValue);
		// Do paměti na kterou ukazuje parametr aValue, ulož (hodnotovou) kopii elementu z čela fronty.
		// Pokud parametr aQueue není NULL, fronta není prázdná a parametr aValue není NULL,
		// zkopíruj hodnotu elementu z čela fronty do paměti, na kterou ukazuje parametr aValue, a vrať true,
		// jinak vrať false.

	bool /* TQueueIterator */ queue_back(const struct TQueue *aQueue, TQueueElement *aValue);
		// Do paměti na kterou ukazuje parametr aValue, ulož (hodnotovou) kopii elementu z konce fronty.
		// Pokud parametr aQueue není NULL, fronta není prázdná a parametr aValue není NULL,
		// zkopíruj hodnotu elementu z konce fronty do paměti, na kterou ukazuje parametr aValue, a vrať true,
		// jinak vrať false.

	bool queue_push(struct TQueue *aQueue, TQueueElement aValue);
		// Vkládá element na konec fronty (tj. na konec seznamu).
		// Pokud parametr aQueue není NULL,
		// alokuj nový uzel TQueueNode, pokud se alokace nepovedla, vrať false.
		// Do nového TQueueNode vlož předanou hodnotu elementu aValue a ukazatel na další (iNext) nastav na NULL.
		// Pokud je fronta prázdná, zapiš adresu nového uzlu do obou složek (proměnných) fronty (iFront a iBack),
		// jinak, zařaď nový uzel za současný konec seznamu.
		// Nastav ukazatel iBack na nově přidaný uzel.
		// Pokud operace skončila úspěšně, vrať true,
		// jinak vrať false.

	bool queue_pop(struct TQueue *aQueue);
		// Odebere element z čela fronty (tj. ze začátku seznamu) bez vracení jeho hodnoty.
		// Pokud parametr aQueue není NULL a fronta není prázdná,
		// zapamatuj si v dočasné proměnné odkaz na počáteční uzel seznamu,
		// jako nový počátek fronty zapiš do proměnné iFront druhý uzel seznamu.
		// Pokud fronta obsahovala pouze seznam s jedním uzlem (počátek je nyní NULL), nastav na NULL i ukazatel konce seznamu,
		// dealokuj paměť bývalého prvního uzlu (adresa z dočasné proměnné).
		// Pokud operace skončila úspěšně, vrať true,
		// jinak vrať false.

	void queue_destroy(struct TQueue *aQueue);
		// Zrušení obsahu fronty. Odstraní (odalokuje) všechny uzly, které fronta obsahuje.
		// Pokud parametr aQueue není NULL,
		// zapamatuj si v dočasné proměnné odkaz na počáteční uzel seznamu,
		// nastav ukazatele na počáteční a koncový uzel seznamu na hodnotu NULL.
		// Pro všechny uzly seznamu, opakuj:
		// zapamatuj si odkaz na rušený uzel,
		// nastav aktuální uzel na následující uzel,
		// odalokuj rušený uzel pomocí zapamatovaného odkazu.

## API pro práci s iterátory:
Struktura pro iterátor `TQueueIterator` obsahuje ukazatel na asociovanou frontu a ukazatel na aktuální uzel seznamu `TQueueNode` uvnitř fronty. 
Koncept správného používání iterátoru musí opět zajistit programátor, který iterátor využívá, viz předchozí cvičení (tj. iterátor garantuje svoji správnou funkci pouze dokud, se nezmění počet elementů v asociované frontě).
V hlavičkovém souboru jsou i funkce rozšiřující možnosti iterátoru - jejich činnost je popsána na konci tohoto textu.

	struct TQueueIterator queue_iterator_begin(const struct TQueue *aQueue);
		// Inicializace a asociace/propojení iterátoru s frontou - zapíše odkaz na frontu a nastaví pozici v iterátoru na počátek fronty.
		// Pokud předaná fronta existuje (tj. aQueue není NULL) a není prázdná, zapiš do složky iterátoru iQueue adresu asociované fronty,
		// nastav iterátor na element na čele fronty (na čele je element, který se bude první odebírat),
		// vrať hodnotu vytvořeného iterátoru.
		// Jinak vrať iterátor s vynulovanými vnitřními složkami.

	bool queue_iterator_is_valid(const struct TQueueIterator *aIter);
		// Zjistí, zda iterátor odkazuje na platný element asociované fronty.
		// Pokud parametr aIter není NULL a
		// pokud je iterátor asociován s platnou frontou (tj. má platnou adresu iQueue) a 
		// tato fronta není prázdná, pokračuj.
		// Vrať true, pokud odkaz v iterátoru na aktuální uzel je platný (tj. nebyl dosažen konec seznamu),
		// jinak vrať false.

	bool queue_iterator_to_next(struct TQueueIterator *aIter);
		// Přesune odkaz v iterátoru z aktuálního elementu na následující element fronty.
		// Pokud je iterátor platný, pokračuj, jinak zruš propojení iterátoru s frontou a vrať false.
		// Posuň aktuální odkaz na další uzel seznamu.
		// Vrať true, když nově odkazovaný uzel existuje (neodkazuje na uzel s adresou NULL),
		// jinak zruš propojení iterátoru s frontou a vrať false.

	TQueueElement queue_iterator_value(const struct TQueueIterator *aIter);
		// Vrátí hodnotu elementu, na kterou odkazuje iterátor.
		// Pokud je iterátor validní, vrať hodnotu aktuálního elementu,
		// jinak vrať nulový element.

	bool queue_iterator_set_value(const struct TQueueIterator *aIter, TQueueElement aValue);
		// Nastaví element, na který odkazuje iterátor, na novou hodnotu.
		// Pokud je iterátor validní,
		// zapiš do aktuálního elementu hodnotu předanou pomocí druhého parametru a vrať true,
		// jinak vrať false.

## Univerzální funkce pro práci s iterátory - cykly/algoritmy:
Funkce `queue_for_each()` s prototypem: \code{.c}

	static inline void queue_for_each(struct TQueueIterator aIter, void(*aOperation)(const struct TQueueIterator *aIter))
\endcode   
Prvním parametrem funkce je iterátor `aIter`, jenž je předem asociovaný se zvolenou frontou a který tak definuje počáteční element pro zvolenou operaci.   
Druhým parametrem funkce je ukazatel `aOperation` definující funkci (např. `print_element()`), jenž je volána na každý z elementů fronty, od elementu na který směřuje iterátor `aIter`, až po konec fronty.   
Funkce pomocí předaného iterátoru `aIter` prochází elementy fronty a pro každý aktuální element zavolá funkci `aOperation`. Funkce tedy umožňuje **provést** se všemi elementy fronty **stejnou operaci**, kterou definuje parametr `aOperation`.   
Tato univerzální funkce je využitelná např. pro tisk všech elementů fronty.   

Funkce `queue_find_if()` s prototypem: \code{.c}

	static inline struct TQueueIterator queue_find_if(struct TQueueIterator aIter, bool(*aPredicate)(const struct TQueueIterator *aIter))
\endcode   
Prvním parametrem funkce je iterátor `aIter`, jenž je předem asociovaný se zvolenou frontou a který tak definuje počáteční element pro zvolenou podmínku.   
Druhým parametrem funkce je ukazatel `aPredicate` určující predikátovou funkci (např. `is_element_with_value_43()`), která je použita jako podmínka pro ukončení cyklu po nalezení elementu, který tuto podmínku splňuje.   
Funkce pomocí předaného iterátoru `aIter` postupně prochází elementy fronty a pro každý element zavolá funkci, na kterou směřuje ukazatel `aPredicate`. Pokud `aPredicate` vrátí `true` (aktuální element splňuje podmínky definované v těle predikátu), cyklus se ukončí a vrátí se iterátor ukazující na aktuální element **splňující** podmínku definovanou predikátovou funkcí `aPredicate`.   

Funkce `queue_find_if_not()` s prototypem: \code{.c}

	static inline struct TQueueIterator queue_find_if_not(struct TQueueIterator aIter, bool(*aPredicate)(const struct TQueueIterator *aIter))
\endcode   
Prvním parametrem funkce je iterátor `aIter`, jenž je předem asociovaný se zvolenou frontou a který tak definuje počáteční element pro zvolenou podmínku.   
Druhým parametrem funkce je ukazatel `aPredicate` na predikátovou funkci (např. `is_element_with_value_43()`), která je použita jako negativní podmínka pro ukončení cyklu.   
Funkce pomocí předaného iterátoru `aIter` postupně prochází elementy fronty a pro každý element zavolá funkci, na kterou směřuje ukazatel `aPredicate`. Pokud `aPredicate` vrátí `false` (aktuální element nesplňuje podmínky definované v těle predikátu), cyklus se ukončí a vrátí se iterátor ukazující na aktuální element **nesplňující** podmínku definovanou predikátovou funkcí `aPredicate`.

Demonstrativní příklad použití těchto funkcí:

\code{.c}
// Precondition: queue1: {41 42 43 43 44 45}

	// Tisk všech elementů fronty
	queue_for_each(queue_iterator_begin(&queue1), print_element);
	putchar('\n');

	// Nalezení prvního elementu s hodnotou 43 od počátku až do konce fronty
	struct TQueueIterator it = queue_find_if(queue_iterator_begin(&queue1), is_element_with_value_43);

	// Přepsání hodnoty nalezeného elementu na hodnotu 128, pokud byl nalezen a posun na další element fronty
	queue_iterator_set_value(&it, 128);
	queue_iterator_to_next(&it);

	// Tisk všech zbývajících elementů fronty
	queue_for_each(it, print_element);
	putchar('\n');

	// Nalezení prvního elementu s jinou hodnotou než 43 za pozicí elementu s novou hodnotou 128 až do konce fronty
	it = queue_find_if_not(it, is_element_with_value_43);

	// Přepsání hodnoty nalezeného elementu na hodnotu 64, pokud byl nalezen
	queue_iterator_set_value(&it, 64);

	// Opětovný tisk všech elementů fronty
	queue_for_each(queue_iterator_begin(&queue1), print_element);
	putchar('\n');

// Postcondition: queue1: {41 42 128 43 64 45}
\endcode

Další příklady použití těchto funkcí naleznete v souboru `main.c`.

---
`$Id: cv03_zadani_TQueue_list.md 2599 2024-02-16 13:50:42Z petyovsky $`
