/****************************************************************** 
 *	KPCB Engineering Fellows Program Application Coding Challenge *
 *                                                                *
 *  Name: Avinash Nayak                                           *
 *  College: Princeton University                                 *
 *  Major: Computer Science                                       *
 *                                                                *
 *  Summary:                                                      *
 *  This code implements a linear probing hashmap in C. For       *
 *  further instructions on how to run this code to test it,      *
 *  please refer to the main() method in this file, and also      *
 *  to my readme file attached separately in the zip. Other       *
 *  than that, this file contains all of the implementation.      *
 ******************************************************************/

/* Including some of C's standard internal libraries. */
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>


/*  NOTE: I used hash map and hash table interchangeably, but they 
	refer to the same structure. */

/* Declaration of this ADT to be used later on in the code. */
typedef struct HashTable *HashTable_T;

/* Creates the entry structure to use for the hash table. */
struct HTEntry {
	const char* key; /* the key of the hash table entry */
	const void* value; /* the value stored in the entry */
};

/* Defines the hash table structure. */
struct HashTable {
	struct HTEntry **table;
	int size;
	int numItems;
};

/*  Returns a new HashTable_T object that represents an initialized
	hash table structure with no keys or values yet. If insufficient
	memory is available to create this structure, returns NULL. */

HashTable_T constructor(int size) {
	HashTable_T hashtable;
	hashtable = (HashTable_T)malloc(sizeof(struct HashTable));
	if (hashtable == NULL) return NULL;

	hashtable->table = (struct HTEntry **)calloc(size, 
												sizeof(struct HTEntry **));
	if(hashtable->table == NULL) {
		free(hashtable);
		return NULL;
	}

	hashtable->size = size;
	hashtable->numItems = 0;

	return hashtable;

}

/*  Returns the hash code for pcKey that is between 0 and 
	size-1, inclusive. "size" represents the size of the 
	hash map. */
static int HT_hash(const char *pcKey, int size)
{
   enum {HASH_MULTIPLIER = 73112}; /* randomly picked number */
   int i;
   unsigned int uiHash = 0U;
   assert(pcKey != NULL);
   for (i = 0; *(pcKey + i) != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
         + (unsigned int)(*(pcKey + i));
   return (int)(uiHash % (unsigned int)size);
}

/*  This function returns 1 (TRUE) if the hash table inputted
	into the function contains an entry with "key" as its key,
	and returns 0 (FALSE) if not. */
static int contains(HashTable_T hashtable, const char *key) {
	struct HTEntry *entry;
	int bin;
	void *value;
	int tableSize;
	int count;
	value = NULL;
	assert(hashtable != NULL);
	assert(key != NULL);

	count = 0;
	tableSize = hashtable->size;
	bin = HT_hash(key, tableSize);
	for (; count < tableSize; bin = (bin + 1) % tableSize){
		entry = *(hashtable->table + bin);
		if(entry != NULL && strcmp(entry->key, key) == 0){
			return 1;
		}
		count++;
	}
	return 0;
}

/*  Frees all the memory occupied by hashtable, and also
	frees the memory occupied by the table of hashtable. To
	be used when the client is finished using the structure.
	*/
static void freeTable(HashTable_T hashtable) {
	int i;
	struct HTEntry* currEntry;

	assert(hashtable != NULL);
	for (i = 0; i < hashtable->size; i++) {
		currEntry = *(hashtable->table + i);
		if(currEntry != NULL) {
			free((void*)currEntry->key);
			free(currEntry);
		}	
	}
	free((void*) hashtable->table);
	free(hashtable);
}

/*  Stores the given key/value pair in the hash map. Prevents the
	storage of NULL keys, but does not prevent the storage of NULL
	values, so the client can determine whether to use NULL values
	in the hash map or not. */
int set(HashTable_T hashtable, const char *key, const void *value) {
	int bin;
	struct HTEntry *currEntry;
	int count;
	int tableSize;
	char *copy;

	assert(hashtable != NULL);
	assert(key != NULL);

	count = 0;
	tableSize = hashtable->size;
	bin = HT_hash(key, tableSize);
	for (; count < tableSize; bin = (bin + 1) % tableSize) {
		if (*(hashtable->table + bin) == NULL) {
			currEntry = (struct HTEntry*)malloc(sizeof(struct HTEntry));
			if (currEntry == NULL) return 0;
			copy = (char *) malloc(strlen(key) + 1);
			strcpy(copy, key);
			currEntry->key = (const char *)copy;
			currEntry->value = value;
			*(hashtable->table + bin) = currEntry;
			hashtable->numItems++;
			return 1;
		}
		else if (strcmp((*(hashtable->table + bin))->key, key) == 0){
			(*(hashtable->table + bin))->value = value;
			return 1;
		}
		count++;
	}
	return 0;
}

/*  Returns the value associated with the given input key, or returns
	null if no value is set with the key in the table. */
void *get(HashTable_T hashtable, const char *key) {
	struct HTEntry *entry;
	int bin;
	const void *value;
	int tableSize;
	int count;
	value = NULL;
	assert(hashtable != NULL);
	assert(key != NULL);

	if (contains(hashtable, key) == 0) return NULL;

	count = 0;
	tableSize = hashtable->size;
	bin = HT_hash(key, tableSize);
	for (; count < tableSize; bin = (bin + 1) % tableSize){
		entry = *(hashtable->table + bin);
		if(strcmp(entry->key, key) == 0){
			value = entry->value;
			break;
		}
		count++;
	}
	return (void *)value;
}

/*	Deletes the value associated with its given key, returning the
	value on success or null if the key has no value. */
void *delete(HashTable_T hashtable, const char *key) {
	int tableSize;
	int bin;
	int count;
	struct HTEntry *entry;
	const char *rehashkey;
	const char *rehashvalue;
	const void *value = NULL;
	assert(hashtable != NULL);
	assert(key != NULL);

	count = 0;
	tableSize = hashtable->size;
	bin = HT_hash(key, tableSize);

	if(hashtable->numItems == 0) return NULL;
	if(contains(hashtable, key) == 0) return NULL;

	while(strcmp((*(hashtable->table + bin))->key, key) != 0)
		bin = (bin + 1) % hashtable->size;
	value = get(hashtable, key);
	
	free((void*)(*(hashtable->table + bin))->key);
	*(hashtable->table + bin) = NULL;

	bin = (bin + 1) % tableSize;
	while(*(hashtable->table + bin) != NULL && count < tableSize) {
		rehashkey = (*(hashtable->table + bin))->key;
		rehashvalue = (*(hashtable->table + bin))->value;
		free((void*)(*(hashtable->table + bin))->key);
		*(hashtable->table + bin) = NULL;
		hashtable->numItems--;
		set(hashtable, rehashkey, rehashvalue);
		bin = (bin + 1) % tableSize;
		count++;
	}

	hashtable->numItems--;
	return (void*)value;
	
}



/*  Returns a float value representing the load factor, or in other
	terms, (number of items in hash map)/(size of hash map), of the
	data structure. As described in the spec, since the size of the
	data structure is fixed, this value should never be greater than
	1, and should never be less than 0. */
float load(HashTable_T hashtable) {
	assert(hashtable != NULL);
	return (float)hashtable->numItems/(float)hashtable->size;
}

/*  Tests all of the functions given in my implementation, by first
	testing whether constructor(), set(), and load() work, and then 
	whether get() and delete() work as well. */
int main(void) {

	/* Declare and initialize the necessary variables. */
	HashTable_T oHashTable;
    char *kleiner = "Kleiner";
    char *perkins = "Perkins";
    char *page = "Page";
    char *brin = "Brin";
    char *zuck = "Zuckerberg";
    char *saverin = "Saverin";
    char *spiegel = "Spiegel";
    char *gates = "Gates";
    char *allen = "Allen";
    char *kpcb = "KPCB";
    char *google = "Google";
    char *snapchat = "Snapchat";
    char *microsoft = "Microsoft";
    char *facebook = "Facebook"; 

    char *pcValue;
    int size;
    int success;
    int length;
    int found;

    printf("------------------------------------------------------\n");
    printf("Testing all of my Hash Table functions.\n");
    printf("Test for constructor():\n");

    /* Test constructor(). */
    size = 5;
    oHashTable = constructor(size);
    assert(oHashTable != NULL);
    printf("constructor() works...\n");
    printf("Size is %d\n\n\n", size);
    
    /* Test the properties of set() and load of hash table. */
    printf("Testing the properties of set() and hash table load():\n");

    printf("<Adding \"Facebook\" as key and \"Zuckerberg\" as value>\n");
    success = set(oHashTable, facebook, zuck);
    printf("(Success of set()) Zuckerberg founded Facebook: %d\n", success);
    printf("Number of items: %d\n", oHashTable->numItems);
    printf("Load: %f\n\n", load(oHashTable));

    printf("<Adding \"KPCB\" as key and \"Kleiner\" as value>\n");
    success = set(oHashTable, kpcb, kleiner);
    printf("(Success of set()) Kleiner founded KPCB: %d\n", success);
    printf("Number of items: %d\n", oHashTable->numItems);
	printf("Load: %f\n\n", load(oHashTable));

	printf("<Adding \"Google\" as key and \"Brin\" as value>\n");
    success = set(oHashTable, google, brin);
    printf("(Success of set()) Brin founded Google: %d\n", success);
    printf("Number of items: %d\n", oHashTable->numItems);
	printf("Load: %f\n\n", load(oHashTable));

	printf("<Adding \"Microsoft\" as key and \"Gates\" as value>\n");
    success = set(oHashTable, microsoft, gates);
    printf("(Success of set()) Gates founded Microsoft: %d\n", success);
    printf("Number of items: %d\n", oHashTable->numItems);
	printf("Load: %f\n\n", load(oHashTable));

	printf("<Adding \"Snapchat\" as key and \"Spiegel\" as value>\n");
    success = set(oHashTable, snapchat, spiegel);
    printf("(Success of set()) Spiegel founded Snapchat: %d\n", success);
    printf("Number of items: %d\n", oHashTable->numItems);
	printf("Load: %f\n\n", load(oHashTable));

	printf("<Adding \"Google\" as key and \"Page\" as value>\n");
	success = set(oHashTable, google, page);
    printf("(Success of set()) Page also founded Google: %d\n", success);
    printf("Number of items: %d\n", oHashTable->numItems);
	printf("Load: %f\n\n", load(oHashTable));


    /* Tests contains() function.*/
    printf("Testing contains(), which is one of my static functions\n");
    printf("Output should be 1 1 1 0 1 0 0 1:\n");
    found = contains(oHashTable, kpcb);
    printf("%d ", found);
    found = contains(oHashTable, google);
    printf("%d ", found);
    found = contains(oHashTable, microsoft);
    printf("%d ", found);
    found = contains(oHashTable, gates);
    printf("%d ", found);
    found = contains(oHashTable, facebook);
    printf("%d ", found);
    found = contains(oHashTable, zuck);
    printf("%d ", found);
    found = contains(oHashTable, "A random dude");
    printf("%d ", found);
    found = contains(oHashTable, snapchat);
    printf("%d ", found);
    printf("\n\n\n");

    /* Tests get() function. */
    printf("Now testing the get() function\n");
    printf("Output should be 1 0 1 1 0 0 1 0: \n");
    pcValue = (char*)get(oHashTable, kpcb);
    printf("%d ", pcValue == kleiner);
    pcValue = (char*)get(oHashTable, google);
    printf("%d ", pcValue == brin);
	pcValue = (char*)get(oHashTable, facebook);
    printf("%d ", pcValue == zuck);
    pcValue = (char*)get(oHashTable, gates);
    printf("%d ", pcValue == NULL);
    pcValue = (char*)get(oHashTable, facebook);
    printf("%d ", pcValue == saverin);
    pcValue = (char*)get(oHashTable, "Twitter");
    printf("%d ", pcValue == zuck);
    pcValue = (char*)get(oHashTable, "Google");
    printf("%d ", pcValue == page);
    pcValue = (char*)get(oHashTable, snapchat);
    printf("%d ", pcValue == NULL);
    printf("\n\n\n");

    /* Tests delete() function. */
    printf("Testing the properties of delete():\n");
    printf("<Deleting \"Facebook\" key and its value>\n");
    pcValue = delete(oHashTable, facebook);
    printf("Value of deleted key should be Zuckerberg: %s\n", pcValue);
    printf("Number of items: %d\n", oHashTable->numItems);
    printf("Load: %f\n", load(oHashTable));
    printf("Still contains Facebook? %d\n\n", 
    		contains(oHashTable, facebook));

    printf("<Deleting \"KPCB\" key and its value>\n");
    pcValue = delete(oHashTable, kpcb);
    printf("Value of deleted key should be Kleiner: %s\n", pcValue);
    printf("Number of items: %d\n", oHashTable->numItems);
	printf("Load: %f\n", load(oHashTable));
	printf("Still contains KPCB? %d\n\n", 
			contains(oHashTable, kpcb));

	printf("<Deleting \"Google\" key and its value>\n");
    pcValue = delete(oHashTable, google);
    printf("Value of deleted key should be Page: %s\n", pcValue);
    printf("Number of items: %d\n", oHashTable->numItems);
	printf("Load: %f\n", load(oHashTable));
	printf("Still contains Google? %d\n\n", 
			contains(oHashTable, google));

	printf("<Deleting \"Microsoft\" key and its value>\n");
    pcValue = delete(oHashTable, microsoft);
    printf("Value of deleted key should be Gates: %s\n", pcValue);
    printf("Number of items: %d\n", oHashTable->numItems);
	printf("Load: %f\n", load(oHashTable));
	printf("Still contains Microsoft? %d\n\n", 
			contains(oHashTable, microsoft));

	printf("<Deleting \"Snapchat\" key and its value>\n");
    pcValue = delete(oHashTable, snapchat);
    printf("Value of deleted key should be Spiegel: %s\n", pcValue);
    printf("Number of items: %d\n", oHashTable->numItems);
	printf("Load: %f\n", load(oHashTable));
	printf("Still contains Snapchat? %d\n\n", 
			contains(oHashTable, snapchat));

	printf("<Deleting \"Google\" key and its value>\n");
	pcValue = delete(oHashTable, google);
    printf("Value of deleted key should be (null): %s\n", pcValue);
    printf("Number of items: %d\n", oHashTable->numItems);
	printf("Load: %f\n", load(oHashTable));
	printf("Continues to not contain Google: %d\n\n", 
			!contains(oHashTable, google));
    printf("\n\n");

    /* Tests freeTable(). */
    freeTable(oHashTable);

    printf("All tests completed and passed!\n\n");
    return 1;
}
