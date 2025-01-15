#ifndef HELPERS 
#define HELPERS

#include "raylib.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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



static LLNode* _CreateLLNode(void* data, size_t size) {
    LLNode* node = malloc(sizeof(LLNode));
    node->next = NULL;
    node->prev = NULL;
    node->data = malloc(size);
    return node;
}


static void _CleanupLLNode(LLNode* node) {
    free(node->data);
}

static LinkedList CreateLinkedList() {
    LinkedList lst;
    //LLNode start = (LLNode) { .data = data, .next = NULL, .prev = NULL };
    lst.start = NULL;
    lst.end = NULL;
    return lst;
}

static void LinkedListAppend(LinkedList* list, void* data, size_t data_size) {
    LLNode* old_end = list->end;
    list->end = _CreateLLNode(data, data_size);
    list->end->prev = old_end;
    list->end->next = NULL;
}

static void CleanupLinkedList(LinkedList* list) {
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


static DynArray CreateDynamicArray(size_t capacity, size_t data_size) {
    DynArray array;
    array.data = malloc(data_size*capacity);
    array.capacity = capacity;
    array.size = 0;
    array.data_size = data_size;
    return array;
}

static void ClearDynArray(DynArray* array) {
    array->size = 0;
}

static void AddToDynamicArray(DynArray* array, void* data) {

    // Resize array, copy old data
    if (array->size >= array->capacity) {
        size_t old_capacity = array->capacity;
        array->capacity *= 2;
        if (array->capacity == 0)
            array->capacity = 10;
        
        void* new_data = malloc(array->capacity * array->data_size);
        memcpy(new_data, array->data, old_capacity);
        free(array->data);
        array->data = new_data;
        
    }

    // Copy new data
    memcpy(((char*)(array->data)) + (array->data_size*array->size), data, array->data_size);
    array->size++;
}

static void CleanupDynamicArray(DynArray array) {
    free(array.data);
}

typedef struct KVPair {
    void* key; 
    void* value;
    struct KVPair* next;
} KVPair;

// Dictionary
#define DICTIONARY_CAPACITY 20
typedef struct Dictionary {
    KVPair data[DICTIONARY_CAPACITY];
    size_t key_size;
    size_t value_size;
} Dictionary;


static Dictionary CreateDictionary(size_t data_size) {
    Dictionary dict;

    memset(&dict, 0, sizeof(dict));


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
