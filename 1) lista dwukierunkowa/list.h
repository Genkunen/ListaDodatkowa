#ifndef LIST_HEADER
#define LIST_HEADER

typedef enum Error {
    SUCCESS = 0,
    FAILURE = 1,
    BAD_ALLOC = 2,
    OUT_OF_BOUNDS = 3,
    BAD_INFO = 4,
    NULL_LIST = 5,
} Error;

typedef struct List_T List_T;
typedef List_T* List;

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

typedef struct InsertInfo {
    size_t where;        // pozycja gdzie dodac
    size_t count;        // ile elementow dodajemy
    size_t objectsCount; // ilosc elementow tablicy objects
    Object* objects;     // gotowy/e element(y) do skopiowania
} InsertInfo;

typedef struct DeleteInfo {
    size_t where;
    size_t count;
} DeleteInfo;

List AllocateList();
void FreeList(List* list);

Node* Insert(List list, InsertInfo infoIn[static 1]);
void Delete(List list, DeleteInfo infoIn[static 1]);
size_t GetSize(List list);

Node* Begin(List list);
Node* Next(Node nodeIn[static 1]);
Node* Prev(Node nodeIn[static 1]);

Error GetError(List list);
void ClearError(List list);

#endif
