#ifndef NODE_H
#define NODE_H

struct Node
{
	void* data;
	struct Node* next;
	struct Node* previous;
	void (*FreeData)(void* data);
};

void NodeInit(struct Node* node, void* data, void (*FreeData)(void* data));
void NodeFreeNodeChain(struct Node* head);

#endif