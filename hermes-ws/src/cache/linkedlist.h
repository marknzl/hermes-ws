#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct LinkedList
{
	int maxCapacity;
	int currentSize;
	struct Node* head;
	struct Node* tail;
};

void LinkedListInit(struct LinkedList* ll, int maxCapacity);
void LinkedListCleanup(struct LinkedList* ll);
int LinkedListIsFull(struct LinkedList* ll);
int LinkedListIsEmpty(struct LinkedList* ll);
struct Node* LinkedListInsertFront(struct LinkedList* ll, void* data, void (*FreeData)(void*));
struct Node* LinkedListInsertBack(struct LinkedList* ll, void* data, void (*FreeData)(void*));
void* LinkedListRemoveFront(struct LinkedList* ll);
void* LinkedListRemoveBack(struct LinkedList* ll);
void LinkedListMoveToFront(struct LinkedList* ll, struct Node* node);

#endif