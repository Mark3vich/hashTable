#include <stdio.h> 
#include <malloc.h> 

#define SIZE 10

// Структура для узла связного списка
typedef struct Node {
    char * key;
    int value;
    struct Node* next;
} Node;

// Структура для хеш-таблицы
typedef struct HashTable {
    Node **array;
} HashTable;

extern Node *newNode(char * key, int value) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

extern HashTable *newHashTable() {
    HashTable *hashTable = (HashTable*)malloc(sizeof(HashTable*));
    hashTable->array = (Node**)malloc(SIZE * sizeof(Node*));
    for(int i = 0; i < SIZE; i++) {
        hashTable->array[i] = NULL;
    }
    return hashTable;
}

extern int hashCode(char * str) {
    unsigned long i = 0;
    for (int j=0; str[j]; j++)
        i += str[j];
    return i % SIZE;
}

extern void insert(HashTable *hashTable, char * key, int value) {
    Node *node = newNode(key, value); // создание узла
    int index = hashCode(key);
    if(hashTable->array[index] == NULL) { 
        hashTable->array[index] = node;
    } else { 
        Node *_node = hashTable->array[index];
        while(_node->next != NULL) {
            _node = _node->next;
        }
        _node->next = node;
    }
}

extern void get(HashTable *hashTable, char * key) {
    int index = hashCode(key);
    Node *node = hashTable->array[index];
    while(node != NULL) { 
        if(node->key == key) {
            printf("%d ", node->value); 
            if(node->next == NULL) { 
                return;
            }
        }
        node = node->next;
    }
}

extern void removeNode(HashTable *hashTable, char * key) {
    int index = hashCode(key);
    Node *node = hashTable->array[index];
    Node *lastNode = NULL;
    while(node != NULL) { 
        if(node->key == key) { 
            if(lastNode == NULL) { 
                hashTable->array[index] = node->next;
            } else { 
                lastNode->next = node->next;
            }
            free(node);
            return;
        }
        lastNode = node;
        node = node->next;
    }
}

extern void freeHashTable(HashTable* hashTable) {
    int i;
    for (i = 0; i < SIZE; i++) {
        Node* currentNode = hashTable->array[i];
        while (currentNode != NULL) {
            Node* temp = currentNode;
            currentNode = currentNode->next;
            free(temp);
        }
    }
    free(hashTable->array);
    free(hashTable);
}

int main() { 
    HashTable * __hashTable = newHashTable();

    insert(__hashTable, "H", 5); 
    insert(__hashTable, "O", 7); 
    insert(__hashTable, "S", 15); 

    get(__hashTable, "S");

    freeHashTable(__hashTable);

    return 0;
}