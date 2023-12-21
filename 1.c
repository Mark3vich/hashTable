#include <stdio.h>
#include <malloc.h>

#if !defined(__TABLE10_H)
#define __TABLE10_H

const int TableOk = 0;
const int TableNotMem = 1;
const int TableUnder = 2;

typedef int BaseType; 

typedef struct element *ptrel;

typedef struct element
{
    BaseType data; // информационное поле 
    ptrel next;    // ссылка на следующий элемент 
} Element;

typedef struct List
{
    ptrel Start;    // содержит указатель на первый элемент списка 
    ptrel ptr;      // для перемещения по списку
    unsigned int N; // длина олс  
} List;

typedef unsigned int T_Key; // Определить тип ключа
typedef struct Table {
    List* Buf;            // Список 
    unsigned n;           // Количество элементов в таблице
    unsigned int SizeBuf; // Количество элементов в массиве Buf
} Table;

int TableError; // 0..2
void InitTable(Table *T, unsigned int SizeBuf);
int EmptyTable(Table *T); // Возвращает 1 , если таблица пуста, иначе — 0
int PutTable(Table *T, BaseType E); /* Включение элемента в таблицу.
Возвращает 1 , если элемент включен в таблицу, иначе — 0 (если в таблице
уже есть элемент с заданным ключем или нехватает памяти) */
int DeleteTable(Table *T, BaseType *E, T_Key Key); /* Исключение элемента.
Возвращает 1 , если элемент с ключем Key был в таблице, иначе — 0 */
int ReadTable(Table *T, BaseType *E, T_Key Key); /* Чтение элемента.
Возвращает 1 , если элемент с ключем Key есть в таблице, иначе — 0 */
int ChangeTable(Table *T, BaseType *E, T_Key Key); /* Изменение элемента.
Возвращает 1 , если элемент с ключем Key есть в таблице, иначе — 0 */
void DoneTable(Table *T); // Уничтожение таблицы

// Инициализация хеш-таблицы 
extern void InitTable(Table *T, unsigned int SizeBuf) { 
    T->Buf = (List*)malloc(SizeBuf * sizeof(List)); // Инициализируем список размер, которого передан в аргументе 
    T->n = 0;                                       // Колличество элементов в таблице изначально равно 0 
    T->SizeBuf = SizeBuf;                           // Количество элементов в массиве Buf равно переданному значению SizeBuf
    for (unsigned int i = 0; i < SizeBuf; i++) {    
        T->Buf[i].Start = NULL;                     // Указатель, который указывает на начальный элемент равер NULL 
                                                    // т.к элемента попрасту не существует 
        T->Buf[i].N = 0;                            // Длина в данный момент олс равна 0 
    }
}

// Проверка на то, что таблица пуста
extern int EmptyTable(Table *T) { 
    return T->n == 0; // Возвращает 1, если количество элементов в таблице равно 0, иначе возвращает 0
}

// Вставка в хеш-таблицу
extern int PutTable(Table *T, BaseType E) { 
    T_Key key = (T_Key)(E % T->SizeBuf); // Вычисление индекса в массиве списков
    if (ReadTable(T, &E, key) == 1) {
        return 0; // Элемент с таким ключом уже существует в таблице
    }

    if (T->Buf[key].Start == NULL) { // Если список пуст
        T->Buf[key].Start = (Element*)malloc(sizeof(Element)); // Создаем новый элемент списка
        T->Buf[key].Start->data = E; // Записываем данные в новый элемент
        T->Buf[key].Start->next = NULL; // Устанавливаем ссылку на следующий элемент как NULL
        T->Buf[key].ptr = T->Buf[key].Start; // Устанавливаем указатель для перемещения по списку на первый элемент
    }
    else { // Если список не пуст
        Element* newElement = (Element*)malloc(sizeof(Element)); // Создаем новый элемент списка
        if (newElement == NULL) { // Если память не выделилась
            TableError = TableNotMem;
            fprintf(stdout, "%s", "Error. There is not enough memory to add an item to the table\n");
            return 0; // Не хватает памяти для добавления элемента в таблицу
        }
        newElement->data = E; // Записываем данные в новый элемент
        newElement->next = NULL; // Устанавливаем ссылку на следующий элемент как NULL
        T->Buf[key].ptr->next = newElement; // Устанавливаем ссылку на следующий элемент для текущего элемента списка
        T->Buf[key].ptr = newElement; // Устанавливаем указатель для перемещения по списку на новый элемент
    }

    T->Buf[key].N++; // Увеличиваем длину списка на 1
    T->n++; // Увеличиваем количество элементов в таблице на 1

    return 1; // Элемент успешно включен в таблицу
}

// Удаление элемента из хеш-таблицы
// Исключение элемента из таблицы
extern int DeleteTable(Table* T, BaseType* E, T_Key Key) {
    if (EmptyTable(T) == 1) {
        fprintf(stdout, "%s", "Error. The table is empty, you cannot exclude an element\n");
        return 0; // Таблица пуста, нельзя исключить элемент 
    }
    
    if (T->Buf[Key].Start == NULL) {
        fprintf(stdout, "%s", "Error. The list is empty, you cannot exclude an element\n");
        return 0; // Список пуст, нельзя исключить элемент
    }

    Element* current = T->Buf[Key].Start; // Устанавливаем указатель на первый элемент списка
    Element* previous = NULL; // Указатель на предыдущий элемент

    while (current != NULL) {
        if (current->data == *E) { // Если найден элемент со значением, равным E
            if (previous != NULL) { // Если элемент не является первым элементом списка
                previous->next = current->next; // Пропускаем текущий элемент списка
                free(current); // Освобождаем память, занятую текущим элементом
                T->Buf[Key].N--; // Уменьшаем длину списка на 1
                T->n--; // Уменьшаем количество элементов в таблице на 1
                return 1; // Элемент успешно исключен из таблицы
            }
            else { // Если элемент является первым элементом списка
                T->Buf[Key].Start = current->next; // Устанавливаем начало списка на следующий элемент
                free(current); // Освобождаем память, занятую текущим элементом
                T->Buf[Key].N--; // Уменьшаем длину списка на 1
                T->n--; // Уменьшаем количество элементов в таблице на 1
                return 1; // Элемент успешно исключен из таблицы
            }
        }
        previous = current; // Сохраняем указатель на текущий элемент в качестве предыдущего
        current = current->next; // Переходим к следующему элементу списка
    }

    // Если элемент не найден
    return 0;
}

// Чтение элемента из хеш-таблицы
extern int ReadTable(Table *T, BaseType *E, T_Key Key) {
    if (EmptyTable(T) == 1) {
        return 0; // Таблица пуста, нельзя прочитать элемент
    }

    if (T->Buf[Key].Start != NULL) { // Если список не пуст
        Element* current = T->Buf[Key].Start; // Устанавливаем указатель на первый элемент списка
        while (current != NULL) { // Пока не достигнут конец списка
            if (current->data == *E) { // Если найден элемент с таким же значением
                *E = current->data; // Записываем данные элемента в переменную
                return 1; // Элемент успешно найден и прочитан
            }
            current = current->next; // Переходим к следующему элементу списка
        }
    }   

    return 0; // Элемент с таким ключом не найден в таблице
}

// Изменение элемента в хеш-таблице
extern int ChangeTable(Table *T, BaseType *E, T_Key Key) { 
    if (EmptyTable(T) == 1) {
        return 0; // Таблица пуста, нельзя записать элемент
    }

    if (T->Buf[Key].Start != NULL) { // Если список не пуст
        Element* current = T->Buf[Key].Start; // Устанавливаем указатель на первый элемент списка
        while (current != NULL) { // Пока не достигнут конец списка
            if (current->data == *E) { // Если найден элемент с таким же значением
                current->data = *E; // Записываем новые данные в элемент
                return 1; // Элемент успешно записан
            }
            current = current->next; // Переходим к следующему элементу списка
        }
    }

    return 0; // Элемент с таким ключом не найден в таблице
}

// Уничтожение хеш-таблицы
extern void DoneTable(Table *T) {
    for (unsigned int i = 0; i < T->SizeBuf; i++) {
        ptrel temp = T->Buf[i].Start;
        
        // Освобождение памяти, занимаемой каждым списком в массиве списков
        while (temp != NULL) {
            ptrel next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(T->Buf); // Освобождение памяти, занимаемой массивом списков
}

#endif

int main() {
    Table table;
    InitTable(&table, 10);

    PutTable(&table, 5);
    PutTable(&table, 15);
    PutTable(&table, 15);
    PutTable(&table, 16);
    PutTable(&table, 25);

    printf("Table size: %u\n", table.n);
    printf("Delete element\n");
    BaseType elem = 25;
    DeleteTable(&table, &elem, 5);
    printf("Read element\n");
    BaseType elem2 = 5;
    printf("%d ", ReadTable(&table, &elem2, 5));
    printf("\n");
    printf("Output of elements\n");
    for (unsigned int i = 0; i < table.SizeBuf; i++) {
        // Проход по всем элементам в массиве списков и вывод их значений
        if (table.Buf[i].Start != NULL) {
            ptrel temp = table.Buf[i].Start;
            while (temp != NULL) {
                printf("|Index - %d, Element - %d|", i, temp->data);
                temp = temp->next;
            }
        }
    }
    DoneTable(&table);

    return 0;
}