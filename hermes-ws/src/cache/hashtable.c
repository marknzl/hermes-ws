#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

void HashtableInit(struct Hashtable* ht, int maxCapacity)
{
	ht->maxCapacity = maxCapacity;
	ht->currentSize = 0;
	ht->htEntries = calloc(maxCapacity, sizeof(struct HTEntry*));
}

void HashtableCleanup(struct Hashtable* ht)
{
	for (int i = 0; i < ht->maxCapacity; i++)
	{
		if (ht->htEntries[i] != NULL)
		{
			printf("[HT Cleanup] Destroying HT Entry with key '%s'\n", ht->htEntries[i]->key);
			HTEntryDestroy(ht->htEntries[i]);
		}
	}
}

struct HTEntry* HTEntryCreate(char* key, void* data, void (*FreeData)(void* data))
{
	struct HTEntry* htEntry = malloc(sizeof(struct HTEntry));

	if (htEntry == NULL)
		return NULL;

	size_t keyLen = strlen(key) + 1;
	char* buf = malloc(keyLen * sizeof(char));

	if (buf == NULL)
		return NULL;

	memcpy_s(buf, keyLen, key, keyLen);

	htEntry->key = buf;
	htEntry->data = data;
	htEntry->FreeData = FreeData;

	return htEntry;
}

void HTEntryDestroy(struct HTEntry* htEntry)
{
	free(htEntry->key);

	if (htEntry->FreeData != NULL)
		htEntry->FreeData(htEntry->data);

	free(htEntry);
}

int HashtableIsFull(struct Hashtable* ht)
{
	return (ht->currentSize == ht->maxCapacity);
}

int HashtableIsEmpty(struct Hashtable* ht)
{
	return (ht->currentSize == 0);
}

int HashtableInsert(struct Hashtable* ht, char* key, void* data, void (*FreeData)(void* data))
{
	if (HashtableIsFull(ht))
		return 0;

	int hash = HashtableHash(ht, key);
	struct HTEntry* htEntry = HTEntryCreate(key, data, FreeData);

	if (ht->htEntries[hash] != NULL)
		hash = HashtableRehash(ht, hash);

	ht->htEntries[hash] = htEntry;
	ht->currentSize++;
	//printf("%s inserted at index %d\n", key, hash);
	return hash;
}

void HashtableRemove(struct Hashtable* ht, char* key)
{
	int index = HashtableContainsKey(ht, key);
	if (index != -1)
	{
		HTEntryDestroy(ht->htEntries[index]);
		ht->htEntries[index] = NULL;
		//printf("Data with key '%s' removed.\n", key);
		ht->currentSize--;
	}
}

void* HashtableGet(struct Hashtable* ht, char* key)
{
	int index = HashtableContainsKey(ht, key);
	if (index == -1)
		return NULL;

	return ht->htEntries[index]->data;
}

int HashtableContainsKey(struct Hashtable* ht, char* key)
{
	if (HashtableIsEmpty(ht))
		return -1;

	int hash = HashtableHash(ht, key);

	if (ht->htEntries[hash] == NULL)
	{
		return -1;
	}

	if (strcmp(ht->htEntries[hash]->key, key) != 0)
	{
		int prevIndex = hash;
		hash = HashtableLinearProbe(ht, hash + 1);
		while (hash != prevIndex)
		{
			if (ht->htEntries[hash] != NULL)
			{
				if (!strcmp(ht->htEntries[hash]->key, key))
					return hash;
			}
			else
			{
				break;
			}
			hash = HashtableLinearProbe(ht, hash + 1);
		}

		return -1;
	}

	return hash;
}

int HashtableHash(struct Hashtable* ht, char* key)
{
	int total = 0;

	for (int i = 0; i < strlen(key); i++)
	{
		total += key[i];
	}

	return total % ht->maxCapacity;
}

int HashtableLinearProbe(struct Hashtable* ht, int init)
{
	return (init) % ht->maxCapacity;
}

int HashtableRehash(struct Hashtable* ht, int init)
{
	int index = init;
	while (ht->htEntries[HashtableLinearProbe(ht, index)] != NULL)
	{
		index = HashtableLinearProbe(ht, index + 1);
	}

	return index;
}