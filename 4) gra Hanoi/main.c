#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

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

void PrintRing(int indent, int size) {
    printf(" ");
    for(int i = 0; i < indent; ++i) {
        printf(" ");
    }
    for(int i = 0; i < size * 2 - 1; ++i) {
        printf("#");
    }
    for(int i = 0; i < indent; ++i) {
        printf(" ");
    }
    printf(" ");
}

void DisplayTowers(Stack* a, Stack* b, Stack* c) {
    int size = a->size + b->size + c->size;
    int indent = size - 1;

    Stack* stacks[3] = { a, b, c };
    Node* nodes[3] = { a->head, b->head, c->head };

    for(int s = 0; s < size; ++s) {
        for(int n = 0; n < 3; ++n) {
            if(stacks[n]->size >= size - s) {
                PrintRing(size - nodes[n]->weight, nodes[n]->weight);
                nodes[n] = nodes[n]->prev;
            }
            else {
                for(int i = 0; i < (size - 1) * 2 + 3; ++i) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    } 

}

static int FalseState() {
    return 0;
}

static int TrueState() {
    return 1;
}

int ProperStackName(char c) {
    c = toupper(c);
    return c == 'A' || c == 'B' || c == 'C';
}

int main(void) {
    Stack stacks[] = { {}, {}, {}};

    int size = {};
    printf("Hanoi size: ");
    scanf_s("%d", &size);

    for(int i = size; i > 0; --i) {
        Add(&stacks[0], i);
    }

    int(*shouldQuit)() = FalseState; 

    int moveN = 0;
    char from = {};
    char to = {};
    while(!shouldQuit()) {
        system("cls");
        DisplayTowers(&stacks[0], &stacks[1], &stacks[2]);

        printf("From: ");
        while(!ProperStackName(from)){
            from = getchar();
        }
        printf(" To: ");
        while(!ProperStackName(to)){
            to = getchar();
        }

        Stack* fromS = &stacks[from - 'A'];
        Stack* toS = &stacks[to - 'A'];
        int err = Move(fromS, toS);
        if(stacks[2].size == size) {
            shouldQuit = TrueState;
        }        
        to = from = 0;
    }

    system("cls");
    DisplayTowers(&stacks[0], &stacks[1], &stacks[2]);

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
