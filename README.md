<!-- @format -->
<h1 align="center">
  <img alt="logo" src="img/logo.png" > BPC-ALD 
</h1>

## 1.CV (ADT TVector)

- Prázdný projekt - (https://github.com/peoblouk/BPC-ALD/archive/139065b91894c760631c64bd2558fc8627cf8a95.zip)
```
struct TVector
	{
	TVectorElement *iValues;		///< Ukazatel na počáteční prvek pole hodnot typu VectorElement
	size_t iSize;					      ///< Počet elementů vektoru
	};
struct TVectorIterator
	{
	struct TVector *iVector;	///< Ukazatel na navázaný vektor (mutable iterátor - umožňuje měnit elementy VectorElement)
	size_t iPos;				///< Aktuální pozice pro indexaci elementu v navázaném vektoru
	};
```

## 2.CV (TStack_array, TStack_list)

- umět vysvětlit práci funkci pop, push, top
- Prázdný projekt - (https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/peoblouk/BPC-ALD/commit/2948d699a6ae5ffcbd3cdcafe8ac390ec5248fb2)
```

```

## 3. CV (TQueue_list)

- Prázdný projekt - ()
- Typ Queue obsahuje ukazatele na dynamicky alokované proměnné typu QueueNode, které představují uzly s hodnotami elementů uspořádanými do lineárního jednosměrně vázaného seznamu
```
typedef int TQueueElement;					///< Definice typu QueueElement (datový typ elementů fronty) // Mohu změnit aby elementy byli například typu float
struct TQueue
	{
	struct TQueueNode *iFront;				///< PIMPL ukazatel na první uzel lineárního jednosměrně vázaného seznamu realizujícího frontu
	struct TQueueNode *iBack;				///< PIMPL ukazatel na poslední uzel lineárního jednosměrně vázaného seznamu realizujícího frontu
	};
```

## 4. CV (TQueue) pomocí kruhového pole

- Prázdný projekt - ()
- promměnná iBackPos ukazuje na první volné místo v kruhu !
```

```

## Visual Studio settings
 - <img alt="vscode settings" src="img/vs2017_setup_new.png" align = "center">
