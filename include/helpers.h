#ifndef HELPERS 
#define HELPERS

#include "raylib.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#define NULL 0


// Linked list
typedef struct LLNode {
    void* data;
    struct LLNode* next;
    struct LLNode* prev;
} LLNode;

typedef struct LinkedList {
    LLNode* start;
    LLNode* end;
} LinkedList;



LLNode _CreateLLNode(void* data, size_t size) {
    LLNode node;
    node.data = malloc(size);
}


LLNode _CleanupLLNode(LLNode* node) {
    free(node->data);
}

LinkedList CreateLinkedList() {

    LinkedList lst;
    //LLNode start = (LLNode) { .data = data, .next = NULL, .prev = NULL };
    lst.start = NULL;
    lst.end = NULL;

    return lst;
}

void CleanupLinkedList(LinkedList* list) {
    if (list == NULL)
        return;

    LLNode* nxt = list->start;
    while (nxt != NULL) {
        _CleanupLLNode(nxt);
        nxt = nxt->next;
    }

}

// Vector
typedef struct DynArray {
    size_t size;
    size_t capacity;
    size_t data_size;
    void* data;
} DynArray;


DynArray CreateDynamicArray(size_t capacity, size_t data_size) {
    DynArray array;
    array.data = malloc(data_size*capacity);
    array.capacity = capacity;
    array.size = 0;
    array.data_size = data_size;
    return array;
}

void CleanupDynamicArray(DynArray array) {
    free(array.data);
}


// Dictionary
#define DICTIONARY_CAPACITY 20
typedef struct Dictionary {
    LinkedList data[DICTIONARY_CAPACITY];
} Dictionary;


Dictionary CreateDictionary(size_t data_size) {
    Dictionary dict;

    for (int i = 0; i < DICTIONARY_CAPACITY; i++) {
        dict.data[i] = CreateLinkedList();
    }

//CleanupLinkedList(LinkedList* list) {

    return dict;
}


static Vector2 NormalizeVector(Vector2 vec) {
    if (vec.x == 0.f && vec.y == 0.f)
        return vec;
    float len = sqrtf(vec.x*vec.x + vec.y*vec.y);
    return (Vector2){ vec.x / len, vec.y / len };
}

#endif
