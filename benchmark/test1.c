#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../my_vm.h"

#define SIZE 5

int main() {

    printf("Allocating a variable of 400 bytes\n");
    void *a = myalloc(100*4);
    void *b = myalloc(100*4);
    int old_a = (int)a;
    int old_b = (int)b;
    //int x = 1;
    //char x1 = 'j';
    char x1[] = "Operating\n";
    char x2[] = "Systems Project 3\n";
    //int y;
    char y1[sizeof(x1)];
    char y2[sizeof(x2)];

    int address_a = 0;
    int address_b = 0;
   
    printf("Address of the allocation: %x\n", (int)a);

    printf("Storing integer a\n");
    address_a = (unsigned int)a;
    address_b = (unsigned int)b;
    
    //PutVal((void *)address_a, &x, sizeof(int));
    PutVal((void *)address_a, &x1, sizeof(x1)); 
    PutVal((void *)address_b, &x2, sizeof(x2));       
    printf("Fetching element stored in the integer x\n");

    //GetVal((void *)address_a, &y, sizeof(int));
    GetVal((void *)address_a, &y1, sizeof(x1));
    GetVal((void *)address_b, &y2, sizeof(x2));
    printf("\nValue: %s\n ", y1);
    printf("\nValue Y2: %s\n ", y2);
    //printf("\n");
    


    printf("Freeing the allocations!\n");
    //printf("%d\n", a );
    myfree(a, 100*4);
   

    printf("Checking if allocations were freed!\n");
    a = myalloc(100*4);
    if ((int)a == old_a)
    {
        printf("free function works\n");
    }
    else
    {
        printf("free function does not work\n");
    }

    return 0;
}