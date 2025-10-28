#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

void printList(List list) {
    Node* it = Begin(list);
   
    printf("Size: %zu [", GetSize(list));
    while(it != NULL) {
        Object* obj = &it->object;
        printf("%s:%g:%g", obj->name, obj->area, obj->circumference);
        it = Next(it);
        if(it != NULL){
            printf(", ");
        }
    }
    printf("]\n");
}

void printListReverse(List list) {
    Node* it = Begin(list);
    while(Next(it) != NULL) {
        it = Next(it);
    }
    printf("Size: %zu [", GetSize(list));
    while(it != NULL) {
        Object* obj = &it->object;
        printf("%s:%g:%g", obj->name, obj->area, obj->circumference);
        it = Prev(it);
        if(it != NULL){
            printf(", ");
        }
    }
    printf("] (reversed)\n");
}

int main(void) {
    List list = AllocateList();
   
    Object square = (Object){.name = "square", .area = 25, .circumference = 5, };
    Object rectan = (Object){.name = "rectangle", .area = 20, .circumference = 7, };
    Object many[] = {
        (Object){.name = "square", .area = 10, .circumference = 2},
        (Object){.name = "plane", .area = 0, .circumference = 3},
        (Object){.name = "circle", .area = 6.28, .circumference = 2},
    };
    InsertInfo ii = (InsertInfo){.objects = &square, .objectsCount = 1, .count = 1 };
    DeleteInfo di = (DeleteInfo){};

    // insert one and clear
    Insert(list, &ii);
    printList(list);
    Clear(list);

    // insert three
    ii.count = 3;
    Insert(list, &ii);
    printList(list);
    
    // insert two in at the position 2
    ii.count = 2;
    ii.where = 2;
    ii.objects = &rectan;
    Insert(list, &ii);
    printList(list);

    // delete first three
    di.count = 3;
    Delete(list, &di);
    printList(list);

    // insert from array and clear again
    ii.where = 1;
    ii.count = 3;
    ii.objects = many;
    ii.objectsCount = 3;
    Insert(list, &ii);
    printList(list);
    Clear(list);
  
    // insert repeating array
    ii.objectsCount = 2;
    ii.where = 0;
    ii.count = 6;
    Insert(list, &ii);
    printList(list);
    printListReverse(list);
    
    FreeList(&list);
}
