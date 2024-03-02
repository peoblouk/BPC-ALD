<!-- @format -->
<h1 align="center">
  <img alt="logo" src="img/logo.png" > BPC-ALD 
</h1>

Iterátor slouží obečně pro iterování v nějakém souboru dat (neboli procházení třeba jednotlivých složkách seznamu)

----------------------------------------------
## 1.CV (ADT TVector)

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
## 2.CV (TStack_array, TStack_list)

- umět vysvětlit práci funkci pop, push, top
- Prázdný projekt - (https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/peoblouk/BPC-ALD/commit/2948d699a6ae5ffcbd3cdcafe8ac390ec5248fb2)

<img width="400" height="350" src="img/tstack.gif" alt="tstack_gif" />

TStack_array (LIFO realizováno pomocí pole)
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
<img alt="TStack_funkce" src="img/tstack_funkce.png" >

----------------------------------------------
TStack_list (LIFO realizováno seznamu)
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

----------------------------------------------
## 3. CV (TQueue_list)

- Prázdný projekt - ()
- Typ Queue obsahuje ukazatele na dynamicky alokované proměnné typu QueueNode, které představují uzly s hodnotami elementů uspořádanými do lineárního jednosměrně vázaného seznamu
<img width="400" height="350" src="img/tstack.gif" alt="tstack_gif" />
```
typedef int TQueueElement;					///< Definice typu QueueElement (datový typ elementů fronty) // Mohu změnit aby elementy byli například typu float
struct TQueue
	{
	struct TQueueNode *iFront;				///< PIMPL ukazatel na první uzel lineárního jednosměrně vázaného seznamu realizujícího frontu
	struct TQueueNode *iBack;				///< PIMPL ukazatel na poslední uzel lineárního jednosměrně vázaného seznamu realizujícího frontu
	};
```
<img alt="TQueue_funkce" src="img/tqueue.gif" >

----------------------------------------------
## 4. CV (TQueue) pomocí kruhového pole

- Prázdný projekt - ()
- promměnná iBackPos ukazuje na první volné místo v kruhu !
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

## Užitečné funkce, které stojí za využití
- print_element (tisk elementů pomocí iterátoru, dá se využít všude)
```

```


## Visual Studio settings
 - <img alt="vscode settings" src="img/vs2017_setup_new.png" align = "center">
