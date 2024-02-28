# Zadání cvičení 4

Dokončete implementaci níže uvedených funkcí realizujících operace pro ADT fronta (Queue). V dodaném příkladu také implementujte operace pro mutable iterátor ADT fronta a následně při jeho použití ověřte správnou funkci tohoto rozhraní.
Frontu realizujte pomocí [kruhového pole](https://en.wikipedia.org/wiki/Circular_buffer "Kruhové pole je jeden ze způsobů, jakým bývá datová struktura fronta v praxi často implementována. Její podstatou je zacyklené pole, ve kterém po posledním prvku znovu následuje první, takže pohyb v poli může být nekonečný. Po zápisu na poslední prvek se zapisuje znovu do prvního – za předpokladu, že z prvního už byl mezitím obsah odebrán. Ke správě cyklického pole slouží dva ukazatele – jeden na pozici, kam se má zapisovat nový prvek, druhý na pozici, ze které se má číst nejstarší vložený prvek. Pokud ukazují na stejnou pozici, je fronta prázdná, pokud by se takového stavu mělo dosáhnout změnou ukazatele zapisování, pak naopak dochází k tomu, že do plné fronty se už další prvek nevejde.").

Pokuste se odpovědět si na tyto otázky:
 - Co je ADT Queue/fronta?
 - Jak tento abstraktní datový typ funguje?
 - Pro jaký typ úloh je ADT fronta vhodný?
 - Jak by se realizovala fronta pomocí kruhového pole?
 - Jaký bude postup operací pro vložení i výběr elementu a také postup výpočtu jejich indexů pomocí operací [modulární aritmetiky](https://cs.wikipedia.org/wiki/Modul%C3%A1rn%C3%AD_aritmetika)?

Pro řešení jsou připraveny zdrojové texty v souborech.
Soubor `main.c` obsahuje volání jednotlivých funkcí tak, jak byly používány v předchozích projektech. Funkce `print_element()` vytiskne hodnotu aktuálního elementu, na který ukazuje iterátor předaný jako parametr.

Funkce `is_element_with_value_43()` je definice predikátu (funkce vracející `bool`, která je volaná na místě výrazu v podmínce) používaného v programu. Tento predikát testuje, zda hodnota v elementu na který odkazuje iterátor, je rovna číslu 43.
Povšimněte si jeho použití - do dalších funkcí se předává pouze název tohoto predikátu (název funkce je totiž jejím vstupním bodem - adresou, na kterou se předá řízení při provádění jejího těla).

V souboru `TQueue.h` jsou dále definice struktur a deklarace funkcí.
Elementy fronty jsou opět definovány jako zobecněný typ `::TQueueElement`.
Délku použitého kruhového pole lze nastavit pomocí definice `QUEUE_MAXCOUNT`.
Vlastní fronta je reprezentována složkami ve struktuře `TQueue`, která obsahuje pole prvků `iValues` a dva indexy (tj. pozice v poli). Index `iBackPos` odkazuje na místo určené pro budoucí vložení nového elementu fronty. Index `iFrontPos` určuje místo pro budoucí odebrání platného elementu z fronty. Pokud mají oba indexy stejnou hodnotu, je fronta prázdná.

Při práci s indexy je nutné ošetřit "kruhovost" pole - tj. využívat pro výpočet hodnot indexů operátor `%` pro zbytek po dělení nebo normalizovat indexy tak, aby ukazovaly vždy do pole (po dosažení konce pole je opět nastavit na jeho začátek).

Opět si všimněte, že většina souborů v projektu je stejná jako při realizaci pomocí lineárního seznamu. Jediné co se liší, je obsah struktury `TQueue` a implementace funkcí, které musí respektovat změnu obsahu této struktury. Použití (názvy funkcí, jejich parametry i volání) zůstává stejné, což odpovídá definici pro abstraktní datový typ.

*Poznámka:   
Operátor `%` představující operaci zbytek po dělení [je definován](https://en.cppreference.com/w/c/language/operator_arithmetic#Remainder) tak, že je možné ho využívat i pro [záporné hodnoty operandů](https://cs.wikipedia.org/wiki/Zbytek_po_d%C4%9Blen%C3%AD) a dle definice v některých z těchto případů vrací jako výsledek zápornou hodnotu. Proto při implementaci fronty budeme, pro operaci zbytek po dělení, důsledně používat pro oba operandy pouze kladné hodnoty.*

## Funkce pro implementaci `TQueue_array` a popis jejich činnosti:
V následující kapitole je popsána stručně činnost.

	void queue_init(struct TQueue *aQueue);
		// Prvotní nastavení vnitřních proměnných fronty.
		// Pokud parametr aQueue není NULL,
		// vynuluj indexy do pole pro čelo (iFrontPos) i konec (iBackPos) fronty.

	bool queue_is_empty(const struct TQueue *aQueue);
		// Test, zda je fronta prázdná - (tj. fronta neobsahuje žádné elementy).
		// Pokud parametr aQueue není NULL, 
		// assercí otestuj konzistenci indexů na počátek a konec fronty (oba indexy musí být menší než QUEUE_MAXCOUNT),
		// odkazují-li indexy do pole pro čelo i konec fronty na různé elementy, vrať false,
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
		// Využijte iBackPos, který již ovšem ukazuje na pozici pro uložení dalšího (budoucího) elementu.
		// Výpočet indexu elementu na konci fronty musí řešit "kruhovost" pole.

	bool queue_push(struct TQueue *aQueue, TQueueElement aValue);
		// Vkládá element na konec fronty (dle indexu iBackPos).
		// Pokud parametr aQueue není NULL,
		// vypočti nový index pro příští uložení (iBackPos; použít modulo),
		// pokud je nový index pro uložení příštího elementu roven indexu elementu na čele fronty (iFrontPos),
		// vrať false.
		// Jinak na pozici (iBackPos) ulož vkládaný element aValue,
		// zapiš nový index pro uložení příštího elementu do iBackPos a vrať true.

	bool queue_pop(struct TQueue *aQueue);
		// Odebere element z čela fronty (dle indexu iFrontPos).
		// Pokud parametr aQueue není NULL a fronta není prázdná,
		// posuň čelo fronty na další element (iFrontPos; použít modulo), vrať true,
		// jinak vrať false.

	void queue_destroy(struct TQueue *aQueue);
		// Zrušení obsahu fronty.
		// Pokud parametr aQueue není NULL,
		// nastav čelo a konec fronty na pozici 0. 
		// Je možné použít funkci queue_init (zdůvodněte si).

## API pro práci s iterátory:
Struktura pro iterátor `TQueueIterator` obsahuje ukazatel na asociovanou frontu a aktuální pozici v této frontě. 
Koncept správného používání iterátoru musí opět zajistit programátor, který iterátor využívá, viz předchozí cvičení (tj. iterátor garantuje svoji správnou funkci pouze dokud, se nezmění počet elementů v asociované frontě).
V hlavičkovém souboru jsou i funkce rozšiřující možnosti iterátoru - jejich činnost je popsána na konci tohoto textu.

	struct TQueueIterator queue_iterator_begin(const struct TQueue *aQueue);
		// Inicializace a asociace/propojení iterátoru s frontou - zapíše odkaz na frontu a nastaví pozici v iterátoru na počátek fronty.
		// Pokud předaná fronta existuje (tj. aQueue není NULL) a není prázdná, zapiš do složky iterátoru iQueue adresu asociované fronty,
		// nastav iterátor na element na čele fronty (na čele je element, který se bude první odebírat),
		// vrať hodnotu vytvořeného iterátoru.
		// Jinak vrať iterátor s vynulovanými vnitřními složkami.

	bool queue_iterator_is_valid(const struct TQueueIterator *aIter);
		// Zjistí, zda iterátor odkazuje na platný element asociované fronty (tj. element má platnou pozici).
		// Pokud parametr aIter není NULL a
		// pokud je iterátor asociován s platnou frontou (tj. má platnou adresu TQueue),
		// tato fronta není prázdná, pokračuj.
		// Vrať true, pokud je aktuální odkaz iterátoru platný (odkazuje do platných pozic mezi počátek a konec),
		// jinak vrať false.

	bool queue_iterator_to_next(struct TQueueIterator *aIter);
		// Přesune odkaz v iterátoru z aktuálního elementu na následující element fronty.
		// Pokud je iterátor platný, pokračuj, jinak zruš propojení iterátoru s frontou a vrať false.
		// Posuň aktuální odkaz na další element.
		// Vrať true, když nově odkazovaný element existuje (odkazuje do platných pozic mezi počátek a konec; operace modulo),
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
Pokud při procházení elementů fronty pomocí iterátoru `aIter` nevrátí predikát `aPredicate` hodnotu `true` pro žádný z elementů, je návratovou hodnotou funkce neplatný iterátor.

Funkce `queue_find_if_not()` s prototypem: \code{.c}

	static inline struct TQueueIterator queue_find_if_not(struct TQueueIterator aIter, bool(*aPredicate)(const struct TQueueIterator *aIter))
\endcode   
Prvním parametrem funkce je iterátor `aIter`, jenž je předem asociovaný se zvolenou frontou a který tak definuje počáteční element pro zvolenou podmínku.   
Druhým parametrem funkce je ukazatel `aPredicate` na predikátovou funkci (např. `is_element_with_value_43()`), která je použita jako negativní podmínka pro ukončení cyklu.   
Funkce pomocí předaného iterátoru `aIter` postupně prochází elementy fronty a pro každý element zavolá funkci, na kterou směřuje ukazatel `aPredicate`. Pokud `aPredicate` vrátí `false` (aktuální element nesplňuje podmínky definované v těle predikátu), cyklus se ukončí a vrátí se iterátor ukazující na aktuální element **nesplňující** podmínku definovanou predikátovou funkcí `aPredicate`.   
Pokud při procházení elementů fronty pomocí iterátoru `aIter` nevrátí predikát `aPredicate` hodnotu `false` pro žádný z elementů, je návratovou hodnotou funkce neplatný iterátor.

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
`$Id: cv04_zadani_TQueue_array.md 2617 2024-02-21 13:53:31Z petyovsky $`
