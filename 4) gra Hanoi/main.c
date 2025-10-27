#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    struct Node* prev;
    int weight;
} Node;

typedef struct Stack {
    Node* head;
    int size;
} Stack;

void Add(Stack* stack, int weight);
int Pop(Stack* stack);
int Top(Stack* stack);

_Bool Move(Stack* from, Stack* to);

void DisplayTowers(Stack* a, Stack* b, Stack* c) {
    printf("\nTower A (size=%d), Tower B (size=%d), Tower C (size=%d)\n", a->size, b->size, c->size);
}
int main(void) {
    Stack A = {};
    Stack B = {};
    Stack C = {};

    const int HanoiSize = 3;
    for (int i = HanoiSize; i > 0; --i) {
        Add(&A, i);
    }

    {
        struct Move {
            Stack* from;
            Stack* to;
        };

        // clang-format off
        struct Move moves[] = {
            (struct Move){ &A, &C },
            (struct Move){ &A, &B },
            (struct Move){ &C, &B },
            (struct Move){ &A, &C },
            (struct Move){ &B, &A },
            (struct Move){ &B, &C },
            (struct Move){ &A, &C },
            (struct Move){ &C, &A },
            (struct Move){ &C, &A },
        };
        // clang-format on

        DisplayTowers(&A, &B, &C);
        int movesSize = sizeof(moves) / sizeof(struct Move);
        for (struct Move* it = moves; it < moves + movesSize; ++it) {
            if (!Move(it->from, it->to)) {
                printf("Nielegalny ruch!\n");
                return 1;
            }
            DisplayTowers(&A, &B, &C);
        }
    }

    return 0;
}

_Bool Move(Stack* from, Stack* to) {
    if (from == to) {
        return 1;
    }
    if (from->size == 0) {
        return 0;
    }

    int disc = Top(from);
    if (to->size > 0 && disc > Top(to)) {
        return 0;
    }

    Pop(from);
    Add(to, disc);
    return 1;
}

void Add(Stack* stack, int weight) {
    if (stack == NULL) {
        return;
    }
    if (stack->head == NULL) {
        stack->head = malloc(sizeof(Node));
        // check bad alloc
        *stack->head = (Node){ .weight = weight };
        ++stack->size;
        return;
    }

    Node* new = malloc(sizeof(Node));
    // check bad alloc
    *new = (Node){ .weight = weight };
    new->prev = stack->head;
    stack->head = new;
    ++stack->size;
}

int Pop(Stack* stack) {
    int retval = stack->head->weight;
    Node* tmp = stack->head;
    if (stack->head != NULL) {
        stack->head = stack->head->prev;
    }
    free(tmp);
    --stack->size;
    return retval;
}

int Top(Stack* stack) {
    return stack->head->weight;
}
