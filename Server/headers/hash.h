#ifndef HASH_H
#define HASH_H

#define BUCK 100              // Number of buckets

typedef struct node{                    // Node of a hash table
    char* name;
    struct node* next;
}Node;


typedef struct hasht{                  //  Hash table
    Node  *buckets[BUCK];

}HashTable;

unsigned long long hash(const char*);
void insert(HashTable* , char*);
int search(HashTable* , char* ) ;
int lookup(HashTable *, char *);
void destroyHashTable(HashTable*);



#endif