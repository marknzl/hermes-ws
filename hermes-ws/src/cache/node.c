#include <stdio.h>
#include <stdlib.h>
#include "node.h"

void NodeInit(struct Node* node, void* data, void (*FreeData)(void* data))
{
	node->data = data;
	node->next = NULL;
	node->previous = NULL;
	node->FreeData = FreeData;
}

void NodeFreeNodeChain(struct Node* head)
{
	struct Node* current = head;
	while (current)
	{
		struct Node* temp = current->next;
		if (current->FreeData != NULL)
		{
			current->FreeData(current->data);
		}
		current = temp;
	}
}