#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum FunctionArgumentType : uint32_t {
    FA_INT,
    FA_VOID,
    FA_PCHAR,
} FunctionArgumentType;

typedef struct Function {
    const char* name;
    void* fun;
    FunctionArgumentType argumentType;
    char* arg;
} Function;

static int DispatchFunction(Function* fun);

static void podwoj(int a) {
    printf("%ld", (long int)a * 2);
}
static void wypisz(int b) {
    printf("%d", b);
}
static void do_malych(char* str) {
    for (char* it = str; *it != '\0'; ++it) {
        *it = (char)tolower(*it);
    }
    printf("%s", str);
}

int main(void) {

    Function functions[] = {
        (Function){ "podwoj", podwoj, FA_INT },
        (Function){ "wypisz", wypisz, FA_INT },
        (Function){ "do_malych", do_malych, FA_PCHAR },
    };

    char nameBuffer[256] = {};
    char argBuffer[256] = {};
    (void)scanf_s("%255s %255s", nameBuffer, 256, argBuffer, 256);

    Function* end = (Function*)functions + (sizeof(functions) / sizeof(Function));
    for (Function* it = functions; it != end; ++it) {
        if (strcmp(nameBuffer, it->name) == 0) {
            it->arg = argBuffer;
            DispatchFunction(it);
        }
    }

    return 0;
}

static int DispatchFunction(Function* function) {
    // clang-format off
    switch (function->argumentType) {
        break;case FA_INT: {
            void(*fun)(int) = function->fun;
            int arg = strtol(function->arg, NULL, 10);
            if(arg == 0) {
                return errno;

            }
            fun(arg);
        }
        break;case FA_VOID: {
            void(*fun)() = function->fun;
            fun();
        };
        break;case FA_PCHAR: {
            void(*fun)(char*) = function->fun;
            fun(function->arg);
        }
        break;default:;
    }
    // clang-format on
    return 0;
}
