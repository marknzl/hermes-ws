#ifndef HASHTABLE_H
#define HASHTABLE_H

struct HTEntry
{
	char* key;
	void* data;
	void (*FreeData)(void* data);
};

struct HTEntry* HTEntryCreate(char* key, void* data, void (*FreeData)(void* data));
void HTEntryDestroy(struct HTEntry* htEntry);

struct Hashtable
{
	int maxCapacity;
	int currentSize;
	struct HTEntry** htEntries;
};

void HashtableInit(struct Hashtable* ht, int maxCapacity);
void HashtableCleanup(struct Hashtable* ht);
int HashtableInsert(struct Hashtable* ht, char* key, void* data, void (*FreeData)(void* data));
void HashtableRemove(struct Hashtable* ht, char* key);
void* HashtableGet(struct Hashtable* ht, char* key);
int HashtableContainsKey(struct Hashtable* ht, char* key);
int HashtableIsFull(struct Hashtable* ht);
int HashtableIsEmpty(struct Hashtable* ht);
int HashtableHash(struct Hashtable* ht, char* key);
int HashtableLinearProbe(struct Hashtable* ht, int init);
int HashtableRehash(struct Hashtable* ht, int init);

#endif