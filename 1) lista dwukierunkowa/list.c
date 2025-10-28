#include "list.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
//                             FILE DECLARATIONS
////////////////////////////////////////////////////////////////////////////////

typedef struct List_T {
    Error error;
    Node* root;
    size_t size;
} List_T;

static void SetGlobalError(Error error);
static Error GetGlobalError();

static Node* InsertNext(Node prev[static 1], InsertInfo InfoIn[static 1], uint32_t skip);

////////////////////////////////////////////////////////////////////////////////
//                            EXTERN DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

List AllocateList() {
    List retval = malloc(sizeof(List_T));
    *retval = (List_T){};
    return retval;
}

void FreeList(List* list) {
    if (list == NULL || *list == NULL) {
        return;
    }
    Clear(*list);
    free(*list);
    *list = NULL;
}

Node* Insert(List list, InsertInfo infoIn[static 1]) {
    if (list == NULL) {
        SetGlobalError(NULL_LIST);
        return NULL;
    }
    if (infoIn == NULL || infoIn->where > list->size) {
        list->error = OUT_OF_BOUNDS;
        return NULL;
    }

    if (infoIn->count == 0 || infoIn->objectsCount == 0) {
        return NULL;
    }

    if (list->root == NULL || infoIn->where == 0) {
        Node* splitNext = NULL;
        if (infoIn->where == 0) {
            splitNext = list->root;
        }
        list->root = malloc(sizeof(Node));
        if (list->root == NULL) {
            list->error = BAD_ALLOC;
            return NULL;
        }
        *list->root = (Node){ .object = *infoIn->objects };

        InsertInfo temp = *infoIn;
        Node* it = InsertNext(list->root, infoIn, 1);
        if (GetGlobalError() == BAD_ALLOC && it == NULL) {
            list->error = BAD_ALLOC;
            return NULL;
        }

        it->next = splitNext;
        if (splitNext != NULL) {
            splitNext->prev = it;
        }

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
            list->error = FAILURE;
            return NULL;
        }
    }

    Node* splitNext = it != NULL ? it : NULL;

    it = InsertNext(splitPrev, infoIn, 0);

    it->next = splitNext;
    if (splitNext != NULL) {
        splitNext->prev = it;
    }
    list->size += infoIn->count;
    return it;
}

void Delete(List list, DeleteInfo infoIn[static 1]) {
    if (list == NULL) {
        SetGlobalError(NULL_LIST);
        return;
    }
    if (infoIn == NULL || infoIn->where > list->size) {
        list->error = OUT_OF_BOUNDS;
        return;
    }

    if (infoIn->count == 0) {
        return;
    }

    if (list->root == NULL) {
        list->error = FAILURE;
        return;
    }

    Node* curr = list->root;

    size_t where = infoIn->where;
    while (where--) {
        curr = Next(curr);
    }
    if (curr == NULL) {
        list->error = FAILURE;
        return;
    }

    Node* splitPrev = curr->prev;
    Node* next = NULL;
    size_t count = infoIn->count;

    while (count--) {
        if (curr == NULL) {
            list->error = OUT_OF_BOUNDS;
            return;
        }
        next = curr->next;
        free(curr);
        curr = next;
        --list->size;
    }

    curr->prev = splitPrev;
    if (splitPrev == NULL) {
        list->root = curr;
        return;
    }
    splitPrev->next = curr;
}

void Clear(List list) {
    if(list == NULL) {
        SetGlobalError(NULL_LIST);
        return;
    }
    if(list->size == 0 && list->root == NULL){
        return;
    } 
    Node* curr = list->root;
    Node* next = Next(curr);
    while(next != NULL) {
        free(curr);
        curr = next;
        next = Next(curr);
    }
    list->root = NULL;
    list->size = 0;
}

size_t GetSize(List list) {
    if (list == NULL) {
        SetGlobalError(NULL_LIST);
        return 0;
    }
    return list->size;
}

Node* Begin(List list) {
    if (list == NULL) {
        SetGlobalError(NULL_LIST);
        return NULL;
    }
    return list->root;
}

Node* Next(Node nodeIn[static 1]) {
    if (nodeIn == NULL) {
        return NULL;
    }
    return nodeIn->next;
}

Node* Prev(Node nodeIn[static 1]) {
    if (nodeIn == NULL) {
        return NULL;
    }
    return nodeIn->prev;
}

Error GetError(List list) {
    return list->error;
}

void ClearError(List list) {
    if (list->error != FAILURE) {
        list->error = SUCCESS;
    }
}

////////////////////////////////////////////////////////////////////////////////
//                             FILE DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

typedef enum ErrorState {
    SET,
    GET,
} ErrorState;

static Error ChangeState(Error* state, Error error) {
    *state = error;
    return SUCCESS;
}

static Error GetState(Error* state, Error error) { // NOLINT
    (void)error;
    return *state;
}

static Error GlobalErrorState(Error error, Error (*operation)(Error* state, Error error)) {
    static Error state = SUCCESS;
    return operation(&state, error);
}

static void SetGlobalError(Error error) {
    (void)GlobalErrorState(error, ChangeState);
}

static Error GetGlobalError() {
    return GlobalErrorState(SUCCESS, GetState);
}

static Node* InsertNext(Node prev[static 1], InsertInfo InfoIn[static 1], uint32_t skip) {
    Node* it = NULL;
    size_t objectIdx = skip % InfoIn->objectsCount;
    for (size_t i = skip; i < InfoIn->count; ++i) {
        it = malloc(sizeof(Node));
        if (it == NULL) {
            SetGlobalError(BAD_ALLOC);
            return NULL;
        }

        if (InfoIn->objectsCount == 1) {
            it->object = *InfoIn->objects;
        }
        else {
            it->object = InfoIn->objects[objectIdx];
            objectIdx = (objectIdx + 1) % InfoIn->objectsCount;
        }
        if (prev != NULL) {
            prev->next = it;
        }
        it->prev = prev;
        prev = it;
    }
    return skip < InfoIn->count ? it : prev;
}

//
