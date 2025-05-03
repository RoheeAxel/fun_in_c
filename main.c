#include "auto.h"
#include "std__map.h"
#include "vector.h"
#include "function/lambda.h"
#include "function/sort.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "oop/class.h"
#include "error/try_catch.h"

#include "asyncio/yield.h"

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
    class(Entity, char *name;, NULL);
    addMethod(Entity, printName, char *, (void *self) {
        Entity *realself = self;
        printf("%s", realself->name);
    });
    class(Person, char *name;int age;, "Entity");
    addMethod(Person, speak, void, (void *self, char *word) {
        Person *realself = self;
        printf("%s: %s", realself->name, word);
    });

    Person p1 = new(Person, "Jon", 42);
    method(Person, printName, p1);
    method(Person, speak, p1, "Hello world!");
}

generator(int, getNext, int, n)
{
    static int counter;
    static int max;
    counter = 0;
    max = n;

    while (counter < max) {
        counter = counter+1;
        printf("Counter: %d\n", counter);
        yield(getNext, counter);
    }
    return 0;
}

generator(int, waiter, int, t)
{
    static time_t start;
    start = time(NULL);

    static int counter;
    counter = t;

    while (time(NULL) - start < counter) {
        printf("Waiting for %ld seconds\n", (time(NULL) - start));
        yield(waiter, counter);
    }
    printf("Waited for %d seconds\n", counter);
    return 0;
}

void generator_tester(void)
{
    // Non asynchronus
    // run_until_end(getNext, 3);
    // run_until_end(waiter, 1);

    // Asynchronus
    while (true) {
        bool a = getNext(120000);
        bool b = waiter(2);
        if (!a && !b) {
            break;
        }
    }
    printf("Done\n");
}

init_vector(int);
void vector_tester(void)
{
    vector(int) *v = newT(vector, int, 0);
    for (int i = 0; i < 10; i++) {
        v->push_back(v, &i);
    }
    for (int i = 0; i < v->get_size(v); i++) {
        printf("%d ", *(int *)v->at(v, i));
    }

    printf("\n");
    deleteT(vector, int, v);
}

init_std__map(char, int);

int char_cmp(char *a, char *b) {
    return *(char *)a - *(char *)b;
}

void map_tester(void)
{
    std__map(char, int) *node = new_std__map(char, int)(char_cmp);

    char *keys[10];
    int *values[10];

    for (int i = 0; i < 10; i++) {
        char *k = malloc(sizeof(char));
        int *v = malloc(sizeof(int));
        *k = 65 + i;
        *v = i * i;
        node->insert(node, k, v);
        keys[i] = k;
        values[i] = v;
    }

    char *k = malloc(sizeof(char));
    *k = 65+5;
    int *v = node->get(node, k);
    if (v == NULL) {
        printf("Key not found\n");
    } else {
        printf("Node with key %c value: %d\n", *k, *v);
    }
    free(k);

    delete_std__map(char, int)(node);

    for (int i = 0; i < 10; i++) {
        free(keys[i]);
        free(values[i]);
    }

    return 0;
}

int main(void) {
    // try_catch_tester();
    // printf("\n");
    // lamda_tester();
    // printf("\n");
    // oop_teste();
    // generator_tester();
    vector_tester();
    map_tester();
    return 0;
}
