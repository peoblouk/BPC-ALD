<!-- @format -->
<h1 align="center">
  <img alt="logo" src="img/logo.png" > BPC-ALD 
</h1>

Iterátor - slouží obečně pro iterování v nějakém souboru dat (neboli procházení třeba jednotlivých složkách seznamu
ADT - alternativní datový typ

----------------------------------------------
## TVector

- Typ Vector spravuje dynamicky alokované pole prvků typu VectorElement a umožňuje s nimi pracovat pomocí definovaného API
  
- <b>POZOR !!! Pokud jsem změnil typ TVectorElement z int na char musím najít makro #define TVECTOR_ELEMENT_FRMSTR   "%d" </b>

```
struct TVector
	{
	TVectorElement *iValues;	///< Ukazatel na počáteční prvek pole hodnot typu VectorElement
	size_t iSize;	           	///< Počet elementů vektoru
	};
struct TVectorIterator
	{
	struct TVector *iVector;	///< Ukazatel na navázaný vektor (mutable iterátor - umožňuje měnit elementy VectorElement)
	size_t iPos;			///< Aktuální pozice pro indexaci elementu v navázaném vektoru
	};
```
<img alt="TVector_funkce" src="img/tvector_funkce.png" >

----------------------------------------------
## TStack_array a TStack_list (LIFO - zásobník)

- umět vysvětlit práci funkci pop, push, top

<img width="400" height="350" src="img/tstack.gif" alt="tstack_gif" />

<b>TStack_array (LIFO realizováno pomocí pole) </b>
```
typedef int TStackElement;					///< Definice typu StackElement (datový typ elementů zásobníku
#define STACK_MAXCOUNT 1024					///< Maximální počet elementů zásobníku

struct TStack
	{
	size_t iCount;						///< Aktuální počet elementů zásobníku
	TStackElement iValues[STACK_MAXCOUNT];			///< Pole hodnot typu StackElement
	};
struct TStackIterator
	{
	const struct TStack *iStack;	///< Ukazatel na navázaný zásobník (immutable iterátor - neumožňuje měnit elementy StackElement)
	size_t iPos;					///< Aktuální pozice pro indexaci elementu v navázaném zásobníku
	};
```
----------------------------------------------

<b>TStack_list (LIFO realizováno seznamu)</b>
```
typedef int TStackElement;					///< Definice typu StackElement (datový typ elementů zásobníku)
struct TStack
	{
	struct TStackNode *iTop;				///< PIMPL ukazatel na počáteční uzel lineárního jednosměrně vázaného seznamu obsahujícího elementy zásobníku (Pointer to private IMPLementation)
	};
struct TStackNode
	{
	struct TStackNode *iNext;				///< Ukazatel na následující uzel lineárního jednosměrně vázaného seznamu
	TStackElement iValue;					///< Element zásobníku uložený v uzlu lineárního jednosměrně vázaného seznamu
	};
```
<img alt="TStack_funkce" src="img/tstack_funkce.png" >

----------------------------------------------
## TQueue_list (FIFO - fronta)
- Typ Queue obsahuje ukazatele na dynamicky alokované proměnné typu QueueNode, které představují uzly s hodnotami elementů uspořádanými do lineárního jednosměrně vázaného seznamu
<img alt="TQueue_funkce" src="img/tqueue.gif" >

```
typedef int TQueueElement;					///< Definice typu QueueElement (datový typ elementů fronty) // Mohu změnit aby elementy byli například typu float
struct TQueue
	{
	struct TQueueNode *iFront;				///< PIMPL ukazatel na první uzel lineárního jednosměrně vázaného seznamu realizujícího frontu
	struct TQueueNode *iBack;				///< PIMPL ukazatel na poslední uzel lineárního jednosměrně vázaného seznamu realizujícího frontu
	};
```

----------------------------------------------
## TQueue_circle (FIFO - fronta)

- promměnná iBackPos ukazuje na první volné místo v kruhu !
<img width="400" height="350" alt="TQueue_circled_funkce" src="img/tqueue_circle.gif" >

```
typedef int TQueueElement;						///< Definice typu QueueElement (datový typ elementů fronty)
#define QUEUE_MAXCOUNT 1024						///< Maximální počet elementů fronty
struct TQueue
	{
	size_t iFrontPos;						///< Aktuální index prvku pole určeného pro odstranění elementu z čela fronty
	size_t iBackPos;						///< Aktuální index prvku pole určeného pro vložení elementu na konec fronty
	TQueueElement iValues[QUEUE_MAXCOUNT];				///< Pole hodnot typu QueueElement
	};
struct TQueueIterator
	{
	const struct TQueue *iQueue;					///< Ukazatel na navázanou frontu (mutable iterátor - umožňuje měnit elementy QueueElement)
	size_t iPos;							///< Aktuální pozice pro indexaci elementu v navázané frontě
	};
```
<img alt="TStack_funkce" src="img/tqueue_cicled_funkce.png" >

## Příklady užitečných funkcí, které stojí za využití s iterátory:
<b>Tisk elementů pomocí iterátoru, dá se využít všude </b>
- <b>print_element </b>

<b>Funkce projede pomocí iterátoru všechny elementy a vytiskne je </b>
- <b>př: queue_for_each(queue_iterator_begin(&queue1), print_element); <b> 

<b>Vyhledá první element fronty splňující zadaný predikát aPredicate. Vyhledávání probíhá od elementu určeného iterátorem aIter, až do konce fronty. </b>
- <b>př: struct TQueueIterator it = queue_find_if(queue_iterator_begin(&queue1), is_element_with_value_43); </b>

<b>Vyhledá první element fronty nesplňující zadaný predikát aPredicate. Vyhledávání probíhá od elementu určeného iterátorem aIter, až do konce fronty. </b>
- <b>př: struct TQueueIterator it = queue_find_if_not(queue_iterator_begin(&queue1), is_element_with_value_43); </b>
