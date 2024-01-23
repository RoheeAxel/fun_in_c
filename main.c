#include "auto.h"
#include "function/lambda.h"
#include "function/sort.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "oop/class.h"
#include "error/try_catch.h"

void second(void) {
    printf("Oh no an error in my program\n");
    throw(3, "Custom error");
}

void first(void) {
    printf("This one is printed\n");
    second();
    printf("This one is not printed\n");
}


int try_catch_tester() {
    try {
        first();
    }
    catch(0) {
        auto e = exception;
        printf("Error code: %d\n", e->error_code);
        printf("Error message: %s\n", e->what);
        printf("Error file: %s (line %d) in function '%s'\n", e->where->file, e->where->line, e->where->function);
    }
    catch(ERROR_DIVISION_BY_ZERO) {
        printf("Division by zero");
    }
    catch(2) {
        printf("Other error");
    }
    return 0;
}

int lamda_tester(void) {
    {
        int array2[] = {1, 4, 5, 3, 2};
        sort(lambda(bool, (int a, int b) { return a < b; }), array2);
        printf("result = [%d, %d, %d, %d, %d]\n", array2[0], array2[1], array2[2], array2[3], array2[4]);

        char *array3[] = {"Worl", "Hello", "!"};
        sort(lambda(bool, (char *a, char *b) { return (strlen(a) - strlen(b)) > 0; }), array3);
        printf("result = [%s, %s, %s]\n", array3[0], array3[1], array3[2]);
    }
    return 0;
}

void oop_teste(void)
{
    class(Entity, NULL, char *name;);
    class(Person , Entity_, char *name;int age;);
    addMethod(Person, speak, void, (void *self, char *word) {
        Person *realself = self;
        printf("%s: %s", realself->name, word);
    });

    Person p1 = new(Person, "Jon", 42);
    method(Person, speak, p1, "Hello world!");
}

int main(void) {
    try_catch_tester();
    printf("\n");
    lamda_tester();
    printf("\n");
    oop_teste();
    return 0;
}
