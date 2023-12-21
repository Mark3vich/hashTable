#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

// Структура для узла связного списка
typedef struct Node {
    int key;
    int value;
    struct Node* next;
} Node;

// Структура для хеш-таблицы
typedef struct HashTable {
    Node** array;
} HashTable;

// Создание нового узла
Node* newNode(int key, int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

// Создание новой хеш-таблицы
HashTable* newHashTable() {
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->array = (Node**)malloc(SIZE * sizeof(Node*));
    int i;
    for (i = 0; i < SIZE; i++) {
        hashTable->array[i] = NULL;
    }
    return hashTable;
}

// Хеш-функция
int hashCode(int key) {
    return key % SIZE;
}

// Вставка элемента в хеш-таблицу
void insert(HashTable* hashTable, int key, int value) {
    int index = hashCode(key);
    Node* _newNode = newNode(key, value);
    if (hashTable->array[index] == NULL) {
        hashTable->array[index] = _newNode;
    } else {
        Node* currentNode = hashTable->array[index];
        while (currentNode->next != NULL) {
            currentNode = currentNode->next;
        }
        currentNode->next = _newNode;
    }
}

// Получение значения элемента по ключу
int get(HashTable* hashTable, int key) {
    int index = hashCode(key);
    Node* currentNode = hashTable->array[index];
    while (currentNode != NULL) {
        if (currentNode->key == key) {
            return currentNode->value;
        }
        currentNode = currentNode->next;
    }
    return -1; // Значение не найдено
}

// Удаление элемента из хеш-таблицы
void removeNode(HashTable* hashTable, int key) {
    int index = hashCode(key);
    Node* currentNode = hashTable->array[index];
    Node* prevNode = NULL;
    while (currentNode != NULL) {
        if (currentNode->key == key) {
            if (prevNode == NULL) {
                hashTable->array[index] = currentNode->next;
            } else {
                prevNode->next = currentNode->next;
            }
            free(currentNode);
            return;
        }
        prevNode = currentNode;
        currentNode = currentNode->next;
    }
}

// Освобождение памяти, занятой хеш-таблицей
void freeHashTable(HashTable* hashTable) {
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
    HashTable* hashTable = newHashTable();

    insert(hashTable, 1, 100);
    insert(hashTable, 1, 200);
    insert(hashTable, 3, 300);

    printf("%d\n", get(hashTable, 1)); // Вывод: 100
    printf("%d\n", get(hashTable, 2)); // Вывод: 200
    printf("%d\n", get(hashTable, 3)); // Вывод: 300

    removeNode(hashTable, 2);
    printf("%d\n", get(hashTable, 2)); // Вывод: -1

    freeHashTable(hashTable);

    return 0;
}