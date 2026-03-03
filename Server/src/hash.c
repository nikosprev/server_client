
#include "../headers/server.h"


                                                // Hash table for keeping the names that have already voted

unsigned long long hash(const char* str) {
    unsigned long long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // djb2 hash function
    }

    return hash%BUCK;
}

void insert(HashTable* hashtable, char* name) {       // Insert into table
    int key = hash(name);

    Node* new = malloc(sizeof(Node));
    new->name=strdup(name);
    new->next = NULL;

    if (hashtable->buckets[key] == NULL) {
        hashtable->buckets[key] = new;
    } else {
        Node* current = hashtable->buckets[key];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new;
    }
}


int search(HashTable* hashtable, char* name) {       // Search table
    long long index = hash(name);


    Node* current = hashtable->buckets[index];

    while (current != NULL) {


        if (strcmp(current->name, name) == 0) {
            return 1;                      // Found
        }

        current = current->next;
    }
    return 0; // Not found
}

void destroyHashTable(HashTable* hashtable) {        // Destroy table
    Node* current,*temp;
    for (int i = 0; i < BUCK; i++) {
        current = hashtable->buckets[i];
        while (current != NULL) {
            temp = current;
            current = current->next;
            free(temp->name);
            free(temp);
        }
        hashtable->buckets[i] = NULL;
    }
    free(hashtable);
}


int lookup(HashTable *hashtable, char * name){          // Call search, inserting if not found
    if(!search(hashtable,name)){


        insert(hashtable,name);

        return 0;      // Not found , insert name
    }
    return 1; //found

}