#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "linkedlist.h"

void LinkedListInit(struct LinkedList* ll, int maxCapacity)
{
	ll->maxCapacity = maxCapacity;
	ll->currentSize = 0;
	ll->head = NULL;
	ll->tail = NULL;
}

void LinkedListCleanup(struct LinkedList* ll)
{
	NodeFreeNodeChain(ll->head);
	ll->currentSize = 0;
}

int LinkedListIsFull(struct LinkedList* ll)
{
	return ll->currentSize == ll->maxCapacity;
}

int LinkedListIsEmpty(struct LinkedList* ll)
{
	return ll->currentSize == 0;
}

struct Node* LinkedListInsertFront(struct LinkedList* ll, void* data, void (*FreeData)(void*))
{
	if (LinkedListIsFull(ll))
		return NULL;

	struct Node* node = malloc(sizeof(struct Node));

	if (node == NULL)
		return NULL;

	NodeInit(node, data, FreeData);

	if (ll->tail == NULL)
	{
		ll->head = node;
		ll->tail = node;
	}
	else
	{
		ll->head->previous = node;
		node->next = ll->head;
		ll->head = node;
	}

	ll->currentSize++;
	return node;
}

struct Node* LinkedListInsertBack(struct LinkedList* ll, void* data, void (*FreeData)(void*))
{
	if (LinkedListIsFull(ll))
		return NULL;

	struct Node* node = malloc(sizeof(struct Node));

	if (node == NULL)
		return NULL;

	NodeInit(node, data, FreeData);

	if (ll->tail == NULL)
	{
		ll->head = node;
		ll->tail = node;
	}
	else
	{
		ll->tail->next = node;
		node->previous = ll->tail;
		ll->tail = node;
	}

	ll->currentSize++;
	return node;
}

void* LinkedListRemoveFront(struct LinkedList* ll)
{
	if (LinkedListIsEmpty(ll))
		return NULL;

	void* data = ll->head->data;
	struct Node* temp = ll->head->next;
	temp->previous = NULL;

	if (ll->head == ll->tail)
	{
		ll->tail = NULL;
	}

	free(ll->head);
	ll->head = temp;
	ll->currentSize--;
	return data;
}

void* LinkedListRemoveBack(struct LinkedList* ll)
{
	if (LinkedListIsEmpty(ll))
		return NULL;

	void* data = ll->tail->data;
	struct Node* temp = ll->tail->previous;
	temp->next = NULL;

	if (ll->head == ll->tail)
	{
		ll->head = NULL;
	}

	free(ll->tail);
	ll->tail = temp;
	ll->currentSize--;
	return data;
}

void LinkedListMoveToFront(struct LinkedList* ll, struct Node* node)
{
	if (LinkedListIsEmpty(ll) || ll->head == node)
		return;

	struct Node* nodePrev = node->previous;
	struct Node* nodeNext = node->next;

	if (ll->tail == node)
	{
		nodePrev->next = NULL;
		ll->tail = nodePrev;
	}
	else
	{
		nodePrev->next = nodeNext;
		nodeNext->previous = nodePrev;
	}

	ll->head->previous = node;
	node->previous = NULL;
	node->next = ll->head;
	ll->head = node;
}