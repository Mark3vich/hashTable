#include <stdio.h> 
#include <stdlib.h>
#include <malloc.h> 
#include <ctype.h>
#include <string.h>

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

extern int get(HashTable *hashTable, char * key) {
    int index = hashCode(key);
    Node *node = hashTable->array[index];
    while(node != NULL) { 
        if(strcmp(node->key, key) == 0) {
            if(node->next == NULL) { 
                return node->value;
            }
        }
        node = node->next;
    }
    return -1;
}

extern void removeNode(HashTable *hashTable, char * key) {
    int index = hashCode(key);
    Node *node = hashTable->array[index];
    Node *lastNode = NULL;
    while(node != NULL) { 
        if(strcmp(node->key, key) == 0) { 
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
            
static int converterItem(char item) { 
    switch(item) {
        case 'O':
            return 16;
        case 'H': 
            return 1;
    }
}

extern void fillingTheWord(char *string) {
    HashTable *hashTable = newHashTable();
    char arrayKey[2] = {'\0'}; // массив ключей 
    int relativeMolecularWeight = 0;
    for(size_t i = 0; i < strlen(string); i++) { 
        if(isalpha(string[i]) && isdigit(string[i+1])) { 
            char key[2] = {string[i], '\0'};
            insert(hashTable, key, atoi(&string[i+1])); 
            arrayKey[strlen(arrayKey)] = string[i];
        } else if(isalpha(string[i]) && !isdigit(string[i+1])) { 
            char key[2] = {string[i], '\0'};
            insert(hashTable, key, 1); 
            arrayKey[strlen(arrayKey)] = string[i];
        }
    }
    for(size_t i = 0; i < 2; i++) { 
        char key[2] = {arrayKey[i], '\0'};
        relativeMolecularWeight += converterItem(arrayKey[i]) * get(hashTable, key);
    }
    printf("%d", relativeMolecularWeight);

    freeHashTable(hashTable);
}

extern void splittingStringIntoElements(char *string) { 
    char *word = strtok(string, " \t");
    int count = 0;
    char * words[20];
    fillingTheWord(word);

    while (1) {
        word = strtok(NULL, " \t");
        if(!word) break; 
        words[count] = word; 
        count++;
    }
    int index = 0;
   
    while(index < count) {
        // printf(",\n%s", words[index]);
        index++;
    }
}

int main() {
    FILE *f; 
    char *s; 
    long int l; 

    if ( (f = fopen("data.txt", "r")) == NULL ) exit(0); 
    fseek(f, 0, SEEK_END); 
    l = ftell(f) + 1; 
    fseek(f, 0, SEEK_SET); 
    s = (char *)malloc(l); 
    l = fread(s, 1, l, f); 
    s[l] = '\0'; 

    printf("%s\n", s); 
    fclose(f); 

    splittingStringIntoElements(s);

    return 0;
}