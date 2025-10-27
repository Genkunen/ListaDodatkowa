#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

int main(void) {
    List list = AllocateList();

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
        (Object){ .name = "1" }, (Object){ .name = "2" }, (Object){ .name = "3" },
        (Object){ .name = "4" }, (Object){ .name = "5" },
    };

    InsertInfo ii = (InsertInfo){ .objects = &(Object){ .name = "kwadrat", .area = 25, .circumference = 5 },
                                  .objectsCount = 1,
                                  .where = 0,
                                  .count = 4 };
    Insert(list, &ii);
    Insert(list, &(InsertInfo){ .objects = objects, .where = 1, .count = 5, .objectsCount = 5 });

    Node* it = Begin(list);
    while (it != NULL) {
        printf("%s ", it->object.name);
        if (Next(it) == NULL) {
            break;
        }
        it = Next(it);
    }
    printf(" | ");
    while (it != NULL) {
        printf("%s ", it->object.name);
        it = Prev(it);
    }

    printf("\n");

    Delete(list, &(DeleteInfo){ .count = 2, .where = 1 });

    it = Begin(list);
    while (it != NULL) {
        printf("%s ", it->object.name);
        if (Next(it) == NULL) {
            break;
        }
        it = Next(it);
    }
    printf(" | ");
    while (it != NULL) {
        printf("%s ", it->object.name);
        it = Prev(it);
    }

    FreeList(&list);
    return 0;
}
