#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "cache.h"
#include "linkedlist.h"
#include "node.h"

void CacheInit(struct Cache* cache, int maxCapacity)
{
	if (maxCapacity < 3)
		maxCapacity = 3;

	cache->maxCapacity = maxCapacity;
	cache->currentSize = 0;

	cache->ht = malloc(sizeof(struct Hashtable));
	HashtableInit(cache->ht, maxCapacity);

	cache->ll = malloc(sizeof(struct LinkedList));
	LinkedListInit(cache->ll, maxCapacity);
}

void CacheCleanup(struct Cache* cache)
{
	LinkedListCleanup(cache->ll);
	HashtableCleanup(cache->ht);
	free(cache->ll);
	free(cache->ht);
}

struct CacheEntry* CacheEntryCreate(char* key, void* data, void (*FreeData)(void* data))
{
	struct CacheEntry* cacheEntry = malloc(sizeof(struct CacheEntry));
	if (cacheEntry == NULL)
		return NULL;

	size_t keyLen = strlen(key) + 1;
	char* buf = malloc(keyLen * sizeof(char));

	if (buf == NULL)
		return NULL;

	memcpy_s(buf, keyLen, key, keyLen);

	cacheEntry->key = buf;
	cacheEntry->data = data;
	cacheEntry->FreeData = FreeData;

	return cacheEntry;
}

void CacheEntryDestroy(struct CacheEntry* cacheEntry)
{
	printf("[Cache Cleanup] Destroying cache entry with key '%s'\n", cacheEntry->key);
	free(cacheEntry->key);
	if (cacheEntry->FreeData != NULL)
	{
		cacheEntry->FreeData(cacheEntry->data);
	}
	free(cacheEntry);
}

void CacheInsert(struct Cache* cache, char* key, void* data, void (*FreeData)(void* data))
{
	if (HashtableContainsKey(cache->ht, key) != -1)
	{
		printf("Cache already contains key '%s'\n", key);
		return;
	}

	struct CacheEntry* cacheEntry = CacheEntryCreate(key, data, FreeData);

	if (cache->currentSize == cache->maxCapacity)
	{
		struct CacheEntry* temp = LinkedListRemoveBack(cache->ll);
		printf("Cache at max capacity! Removing least recently used item with key '%s'.\n", temp->key);
		HashtableRemove(cache->ht, temp->key);
	}

	struct Node* node = LinkedListInsertFront(cache->ll, cacheEntry, CacheEntryDestroy);
	HashtableInsert(cache->ht, key, node, NULL);

	if (cache->currentSize != cache->maxCapacity)
		cache->currentSize++;

	printf("%s has been inserted into the cache!\n", key);
}

void CacheReplace(struct Cache* cache, char* key, void* data)
{
	int index = HashtableContainsKey(cache->ht, key);
	if (index == -1)
	{
		printf("Cache doesn't contain key!");
		return;
	}

	struct HTEntry* htEntry = cache->ht->htEntries[index];
	// Need to implement...
}

void* CacheGet(struct Cache* cache, char* key)
{
	if (HashtableContainsKey(cache->ht, key) == -1)
	{
		return NULL;
	}

	struct Node* node = HashtableGet(cache->ht, key);
	if (node == NULL)
		return NULL;

	struct CacheEntry* cacheEntry = node->data;
	LinkedListMoveToFront(cache->ll, node);

	printf("Cache hit: '%s'\n", key);
	return cacheEntry->data;
}