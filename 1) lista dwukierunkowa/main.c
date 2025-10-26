#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Object {
    const char* name;
    float area;
    float circumference;
} Object;

typedef struct Node {
    struct Node* next;
    struct Node* prev;
    Object object;
} Node;

typedef struct List {
    Node* root;
    size_t size;
} List;

void FreeList(List* list);

typedef struct InsertInfo {
    size_t where;        // pozycja gdzie dodac
    size_t count;        // ile elementow dodajemy
    size_t objectsCount; // ilosc elementow tablicy objects
    Object* objects;     // gotowy/e element(y) do skopiowania
} InsertInfo;

Node* Insert(List list[static 1], InsertInfo infoIn[static 1]);

Node* Next(Node nodeIn[static 1]);
Node* Prev(Node nodeIn[static 1]);

int main(void) {
    List list = { 0 };

    // Insert(&list, &(InsertInfo){ .name = "123", .area = 15, .circumference = 10, .where = 0 });
    // Insert(&list, &(InsertInfo){ .name = "123", .area = 15, .circumference = 10, .where = 1, .count = 5 });

    const char* arr[] = { "1", "2", "3", "4", "5" };

    // list.root = malloc(sizeof(Node));
    // *list.root = (Node){ .name = arr[0] };
    // Node* prev = list.root;
    // Node* it = NULL;
    // for (int i = 1; i < 5; ++i) {
    //     it = malloc(sizeof(Node));
    //     *it = (Node){ .name = arr[i] };
    //     prev->next = it;
    //     it->prev = prev;
    //     prev = it;
    // }
    // list.size = 5;

    Object objects[] = {
        (Object){ .name = "1" },
        (Object){ .name = "2" },
        (Object){ .name = "3" },
    };

    Insert(&list,
           &(InsertInfo){
               .objects = &(Object){ .name = "kwadrat", .area = 25, .circumference = 5 }, .where = 0, .count = 4 });
    Insert(&list, &(InsertInfo){ .objects = objects, .where = 2, .count = 3 });

    Node* it = list.root;
    while (it != NULL) {
        printf("%s ", it->object.name);
        it = Next(it);
    }

    FreeList(&list);
    return 0;
}

Node* Insert(List list[static 1], InsertInfo infoIn[static 1]) {
    // sprawdzamy liste
    if (list == NULL) {
        return NULL; ////////////////////////
    }
    // sprawdzamy info
    if (infoIn == NULL || infoIn->where > list->size + 1) {
        return NULL; ////////////////////////
    }

    if (infoIn->count == 0) {
        return NULL;
    }

    if (list->root == NULL || infoIn->where == 0) {
        Node* splitNext = NULL;
        if (infoIn->where == 0) {
            splitNext = list->root;
        }
        list->root = malloc(sizeof(Node));
        list->root->object = *infoIn->objects;

        Node* prev = list->root;
        Node* it = NULL;
        for (size_t i = 0; i < infoIn->count; ++i) {
            it = malloc(sizeof(Node));
            if (infoIn->objects != NULL) {
                list->root->object = infoIn->objects[i];
            }
            else {
                list->root->object = *infoIn->objects;
            }
            prev->next = it;
            it->prev = prev;
            prev = it;
        }

        it->next = splitNext;
        list->size += infoIn->count;
        return it;
    }

    Node* splitPrev = NULL;
    Node* it = list->root;
    size_t where = infoIn->where;
    while (where--) {
        splitPrev = it;
        it = Next(it);
        if (it == NULL && where != 0) {
            return NULL; ////////////////////////
        }
    }

    Node* splitNext = it != NULL ? it : NULL;

    Node* prev = splitPrev;
    for (size_t i = 0; i < infoIn->count; ++i) {
        it = malloc(sizeof(Node));
        if (infoIn->objects != NULL) {
            list->root->object = infoIn->objects[i];
        }
        else {
            list->root->object = *infoIn->objects;
        }

        if (prev != NULL) {
            prev->next = it;
        }
        it->prev = prev;
        prev = it;
    }
    it->next = splitNext;

    list->size += infoIn->count;
    return it;
}

Node* Next(Node nodeIn[static 1]) {
    if (nodeIn == NULL || nodeIn->next == NULL) {
        return NULL; ////////////////////////
    }
    return nodeIn->next;
}

Node* Prev(Node nodeIn[static 1]) {
    if (nodeIn == NULL || nodeIn->prev == NULL) {
        return NULL; ////////////////////////
    }
    return nodeIn->prev;
}

void FreeList(List* list) {
    free(list->root);
    list->root = NULL;
}
