#ifndef CACHE_H
#define CACHE_H

struct CacheEntry
{
	char* key;
	void* data;
	void (*FreeData)(void* data);
};

struct CacheEntry* CacheEntryCreate(char* key, void* data, void (*FreeData)(void* data));
void CacheEntryDestroy(struct CacheEntry* cacheEntry);

struct Cache
{
	int maxCapacity;
	int currentSize;
	struct Hashtable* ht;
	struct LinkedList* ll;
};

void CacheInit(struct Cache* cache, int maxCapacity);
void CacheCleanup(struct Cache* cache);
void CacheInsert(struct Cache* cache, char* key, void* data, void (*FreeData)(void* data));
void* CacheGet(struct Cache* cache, char* key);

#endif