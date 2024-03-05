/** \file TQueue.c
 *  \brief Implementace API pro typ fronta (realizace pomocí lineárního jednosměrně vázaného seznamu)
 *  \author Petyovský
 *  \version 2024
 *  $Id: TQueue.c 2616 2024-02-16 15:36:28Z petyovsky $
 */

#include <assert.h>
#include "TQueue.h"

/** \brief Úplná definice privátního typu QueueNode
 *  \details Privátní typ QueueNode (nedostupný mimo funkce ze souboru TQueue.c) reprezentuje typ pro jednotlivé uzly lineárního jednosměrně vázaného seznamu.
 */
struct TQueueNode
	{
	struct TQueueNode *iNext;				///< Ukazatel na následující uzel lineárního jednosměrně vázaného seznamu
	TQueueElement iValue;					///< Element fronty uložený v uzlu lineárního jednosměrně vázaného seznamu
	};

void queue_init(struct TQueue* aQueue)
	{
	if (aQueue == NULL) // Ukazuje na platný objekt?
		return;

	aQueue->iBack = NULL; // Nastavení počátečního uzlu
	aQueue->iFront = NULL; // Nastavení koncového uzlu
	}
bool queue_is_empty(const struct TQueue *aQueue)
	{
	if (aQueue == NULL) // Existuje fronta -> tím pádem je prázdná?
		return true;

	assert((aQueue->iBack != NULL && aQueue->iFront != NULL) || (aQueue->iBack == NULL && aQueue->iFront == NULL));
	
	//if(aQueue->iFront != NULL) // Pak vezmu jeden z ukazatelů a otestuji jestli jsou prázdné
	//return true;
	return aQueue->iFront == NULL;
	}

bool /* TQueueIterator */ queue_front(const struct TQueue *aQueue, TQueueElement *aValue)
	{
	if (queue_is_empty(aQueue)) // Je fronta prázdná
		return false;
	
	if (aValue == NULL) // Ukazuje na platný objekt?
		return false;

	*aValue = aQueue->iFront->iValue; // Kopírování hodnoty z čela fronty
	return true;
	}

bool /* TQueueIterator */ queue_back(const struct TQueue *aQueue, TQueueElement *aValue)
	{
	if (queue_is_empty(aQueue)) // Je fronta prázdná?
		return false;

	if (aValue == NULL) // Ukazuje na platný objekt?
		return false;

	*aValue = aQueue->iBack->iValue; // Kopírování hodnoty z čela fronty
	return true;
	}

bool queue_push(struct TQueue *aQueue, TQueueElement aValue)
	{
	if (aQueue == NULL) // Je objekt platný?
		return false;

	struct TQueueNode *newnode = NULL; // Pomocná proměnná
	newnode = malloc(sizeof(struct TQueueNode));
	if (newnode == NULL) // Proběhla alokace v pořádku?
		return false;

	newnode->iNext = NULL; // Nastav hodnotu ukazatele na NULL jelikož je to konec
	newnode->iValue = aValue; // Vložení hodnoty proměnné

	if (queue_is_empty(aQueue)) // Je fronta prázdná?
		aQueue->iFront = newnode; // Zapiš nový prvek do ukazatele, vytvoř tak nový
	else // zařaď nový uzel za současný konec seznamu
		aQueue->iBack->iNext = newnode; // Zařaď hodnotu na konec existujícího seznamu	
	
	aQueue->iBack = newnode; // Posuň ukazatele na konec
	return true;
	}

bool queue_pop(struct TQueue *aQueue)
	{
	if (queue_is_empty(aQueue)) // Je objekt platný?
		return false;

	struct TQueueNode* oldnode = aQueue->iFront;  // Pomocná proměnná // Zapamatuj si v dočasné proměnné odkaz na počáteční uzel seznamu
	aQueue->iFront = oldnode->iNext; // Jako nový počátek fronty zapiš do proměnné iFront druhý uzel seznamu
	
	if (oldnode->iNext == NULL) // Otestuji zda má iFront NULL tak vyprazdňuji frontu a musím tím pádem smazat i iBack
		aQueue->iBack = NULL; // Odebrání posledního prvku z fronty, tím pádem je aQueue je prázdná

	free(oldnode); // Odalokuji 
	return true;
	}

void queue_destroy(struct TQueue *aQueue)
	{
	if (queue_is_empty(aQueue)) // Kontrola zda fronta existuje
		return;

	struct TQueueNode* actnode = aQueue->iFront; // Pomocná proměnná , zapamatuj si v dočasné proměnné odkaz na počáteční uzel seznamu
	
	aQueue->iBack = NULL; // Vymaž ukazatele
	aQueue->iFront = NULL; // Vymaž ukazatele

	while (actnode != NULL) // Dokud zde
		{
		struct TQueueNode *oldnode = actnode; // Druhý pomocný, uchovaná adresa pro aktuálně rušený uzel
		actnode = actnode->iNext; // Posunu pomocný ukazatel
		free(oldnode); // Mám adresu na další uzel, tak odalokuj rušený uzel pomocí zapamatovaného odkazu
		}
	}

struct TQueueIterator queue_iterator_begin(const struct TQueue *aQueue)
	{
	if (aQueue == NULL) // Je platný?
		return (struct TQueueIterator) { .iQueue = NULL, .iActual = NULL };

	return (struct TQueueIterator) { .iQueue = aQueue, .iActual = aQueue->iFront };
	}

bool queue_iterator_is_valid(const struct TQueueIterator *aIter)
	{
	if (aIter == NULL) // Existuje?
		return false;

	if (queue_is_empty(aIter->iQueue))
		return false;

	return aIter->iActual != NULL; //  Pokud je iActual NULL tak neexistuje
	}

bool queue_iterator_to_next(struct TQueueIterator *aIter)
	{
	if (!queue_iterator_is_valid(aIter))
		return false;

	aIter->iActual = aIter->iActual->iNext;
	return aIter->iActual != NULL; //  Pokud je iActual NULL tak neexistuje
	}

TQueueElement queue_iterator_value(const struct TQueueIterator *aIter)
	{
	if (!queue_iterator_is_valid(aIter))
		return (TQueueElement) { 0 };

	return aIter->iActual->iValue;
	}

bool queue_iterator_set_value(const struct TQueueIterator *aIter, TQueueElement aValue)
	{
	if (!queue_iterator_is_valid(aIter))
		return false;
	
	aIter->iActual->iValue = aValue;
	return true;
	}


bool queue_push_front(struct TQueue* aQueue, TQueueElement aValue)
	{
	if (aQueue == NULL)
		return false;

	struct TQueueNode* new_node = malloc(sizeof(struct TQueueNode));
	if (new_node == NULL) // Ověření alokace
		return false;

	new_node->iValue = aValue; // Předání hodnoty nového prvku
	new_node->iNext = aQueue->iFront; // Vložím adresu následujícího prvku za nový prvek

	aQueue->iFront = new_node;

	return true;

	//aQueue->iFront->iNext = 

	
	}
