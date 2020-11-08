#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../my_vm.h"

#define SIZE 5

int main() {

    printf("Allocating a variable of 400 bytes\n");
    void *a = myalloc(100*4);
    int old_a = (int)a;
    int x = 1;
    int y;
    int address_a = 0;
   
    printf("Addresses of the allocation: %x\n", (int)a);

    printf("Storing integer a\n");
            address_a = (unsigned int)a;
            PutVal((void *)address_a, &x, sizeof(int));
            
    printf("Fetching matrix elements stored in the arrays\n");

        GetVal((void *)address_a, &y, sizeof(int));
        printf("%d ", y);
        printf("\n");
    


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