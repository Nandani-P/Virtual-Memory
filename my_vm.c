#include "my_vm.h"
void *physicalMemory;
int numberOfVirtPages;
int numberOfPhysPages;
bool initializePhysicalFlag = false;
//#define numberOfPages 1024*1024
//#define numberOfFrames 1024*1024

//TO-DO Initialize following in the SetPhysicalMem() using bitmap
int* innerPagetable[numberOfVirtPages];       //size of pte_t
bool physicalCheckFree[numberOfPhysPages] = {true};
bool virtualCheckFree[numberOfVirtPages] = {true};

//int tukdetukde = numberOfVirtPages*sizeof(int)/(PGSIZE);
int pageTableEntriesPerBlock = 1024;
int* outerPageTable[1024];      //pde_t 

/*
Function responsible for allocating and setting your physical memory
*/
void SetPhysicalMem() {

    //Allocate physical memory using mmap or malloc; this is the total size of
    //your memory you are simulating
    
    physicalMemory = (void *) malloc(MEMSIZE);
    initializePhysicalFlag = true;

    /*char * physicalMemory = mmap((void*) (PGSIZE * (1 << 100)),    
    PGSIZE,                         
    PROT_READ|PROT_WRITE|PROT_EXEC,
    MAP_ANON|MAP_PRIVATE,             
    
  );*/

    //HINT: Also calculate the number of physical and virtual pages and allocate
    //virtual and physical bitmaps and initialize them

    numberOfVirtPages = MAX_MEMSIZE/ PGSIZE;
    numberOfPhysPages = MEMSIZE/ PGSIZE;


    


}


/*
 * Part 2: Add a virtual to physical page translation to the TLB.
 * Feel free to extend the function arguments or return type.
 */
int add_TLB(void *va, void *pa)
{

    /*Part 2 HINT: Add a virtual to physical page translation to the TLB */

    return -1;
}


/*
 * Part 2: Check TLB for a valid translation.
 * Returns the physical page address.
 * Feel free to extend this function and change the return type.
 */
pte_t *
check_TLB(void *va) {

    /* Part 2: TLB lookup code here */

}


/*
 * Part 2: Print TLB miss rate.
 * Feel free to extend the function arguments or return type.
 */
void
print_TLB_missrate()
{
    double miss_rate = 0;

    /*Part 2 Code here to calculate and print the TLB miss rate*/




    fprintf(stderr, "TLB miss rate %lf \n", miss_rate);
}


/*
The function takes a virtual address and page directories starting address and
performs translation to return the physical address
*/
pte_t * Translate(pde_t *pgdir, void *va) {
    //HINT: Get the Page directory index (1st level) Then get the
    //2nd-level-page table index using the virtual address.  Using the page
    //directory index and page table index get the physical address

    /*
    outerPageTableAddress = physicalMemory(pgdir)
    innerPageTableAddress = outerPageTableAddress(va.outer)
    frameNumber = innerPageTable(va.inner)
    physicalAddressOfFrame = frameNumber(va.offset)
    return
    */

    //If translation not successfull
    return NULL;
}


/*
The function takes a page directory address, virtual address, physical address
as an argument, and sets a page table entry. This function will walk the page
directory to see if there is an existing mapping for a virtual address. If the
virtual address is not present, then a new entry will be added
*/
int
PageMap(pde_t *pgdir, void *va, void *pa)
{

    /*HINT: Similar to Translate(), find the page directory (1st level)
    and page table (2nd-level) indices. If no mapping exists, set the
    virtual to physical mapping */

    if outerPageTable[pgdir] == NULL
        outerPageTable[pgdir] == va;

    if innerPagetable[va] == NULL
        innerPagetable[va] == pa;


    /*int *addressPgTable;           //TO-DO 2 mapping condition
    int *addressPgDir;
    addressPgTable = innerPageTable;

    //Mapping physical address to inner page table 
    *addressPgTable = pa;

    //memcpy(addressPgTable, &pa, size);
    addressPgDir = outerPageTable;
    *addressPgDir = innerPagetable;*/
    
    return -1;
}

bool check_require_avail_pa(int num_pages){
    int havePageCounter = 0;
    for (int i = 0; i < numberOfPhysPages; i++) {  
        if (physicalCheckFree[i] == true){
            havePageCounter++; 

            if (havePageCounter == num_pages){
                return true; 
            }
        }
    }
    return false;
}


/*Function that gets the next available page
*/
void *get_next_avail_pa(int num_pages) {

    //check for free space using physicalCheckFree array of size is equal to physical memory
    // bool *pFreeAddress;

    // //
    // pFreeAddress = physicalCheckFree;

    //Use virtual address bitmap to find the next free page
    for (int i = 0; i < numberOfPhysPages; i++) {     // numberOfFrames = numberOfPhysPages
        if (physicalCheckFree[i] == true){
            physicalCheckFree[i] = false;
            printf("PA Free Flag: %u\n", physicalCheckFree[i]);    
            return physicalMemory + i*PGSIZE;     //Have to test
        }
    }
    return NULL;
}

int get_next_avail_va(int num_pages) {

    //check for free space using physicalCheckFree array of size is equal to physical memory
    // bool *pFreeAddress;

    // //
    // pFreeAddress = physicalCheckFree;

    //Use virtual address bitmap to find the next free page
    for (int i = 0; i < (numberOfVirtPages - num_pages); i++) {     // numberOfFrames = numberOfPhysPages
        if (virtualCheckFree[i] == true){

            bool haveContinuousPages = true;
            for (int j = 0; j < num_pages; j++){
                if (virtualCheckFree[i+j] != true){
                    haveContinuousPages = false;
                    break;
                }
            }

            if (!haveContinuousPages){
                // find next
                continue;
            }

            for (int j = 0; j < num_pages; j++){  
                virtualCheckFree[i+j] = false;
                printf("Free Flag: %u\n", virtualCheckFree[i]);    
            }
            return i;     //Have to test 

        }
    }
    return NULL;
}
/* Function responsible for allocating pages
and used by the benchmark
*/
void *myalloc(unsigned int num_bytes) {

    //HINT: If the physical memory is not yet initialized, then allocate and initialize.
    if (initializePhysicalFlag == false)
    {
        SetPhysicalMem();

    }
    printf("Outer page table: %d", outerPageTable[1]);

   /* HINT: If the page directory is not initialized, then initialize the
   page directory. Next, using get_next_avail(), check if there are free pages. If
   free pages are available, set the bitmaps and map a new page. Note, you will
   have to mark which physical pages are used. */

    //numner of pages required when myalloc is called 
    int num_pages = num_bytes/PGSIZE;
    if (num_bytes % PGSIZE != 0){
        num_pages = num_pages + 1; 
    }
    
    printf("Searching for virtual memory");
    int va_EntryNumber;
    va_EntryNumber = get_next_avail_va(num_pages);   // check null condition in pointer
    if (va_EntryNumber == NULL){
        printf("virtual memory is not available");
        return NULL;
    }
    
    int pgDirEntryNumber;
    pgDirEntryNumber = va_EntryNumber / pageTableEntriesPerBlock; 
    
    va = innerPagetable + va_EntryNumber*sizeof(int);  // Should use bitmap size instead of int

    printf("Searching for physical memory");
    int *pa;
    
    if (check_require_avail_pa(num_pages) == false){
        printf("Physical memory not found");
        return NULL;
    }


    for (int i = 0; i < num_pages; i++) {
        //checking for next free pages and getting the physical address of that page.
        pa = get_next_avail_pa(num_pages);   // check null condition in pointer
        if (pa != NULL){
            PageMap(*pgDirEntry, va, pa);
        }
    }

    // return final 32 bit VA 
    
    
}

/* Responsible for releasing one or more memory pages using virtual address (va)
*/
void myfree(void *va, int size) {

    //Free the page table entries starting from this virtual address (va)
    // Also mark the pages free in the bitmap
    //Only free if the memory from "va" to va+size is valid
    //free(va);
}


/* The function copies data pointed by "val" to physical
 * memory pages using virtual address (va)
*/
void PutVal(void *va, void *val, int size) {

    /* HINT: Using the virtual address and Translate(), find the physical page. Copy
       the contents of "val" to a physical page. NOTE: The "size" value can be larger
       than one page. Therefore, you may have to find multiple pages using Translate()
       function.*/

    // physical address is equal to virtual address... for now
   

    // find mapping
    physicalAddress = va;

    //setting value to a address(physicalAddress) 
    memcpy(physicalAddress, val, size);
    printf("Put value\n");

}


/*Given a virtual address, this function copies the contents of the page to val*/
void GetVal(void *va, void *val, int size) {

    /* HINT: put the values pointed to by "va" inside the physical memory at given
    "val" address. Assume you can access "val" directly by derefencing them.
    If you are implementing TLB,  always check first the presence of translation
    in TLB before proceeding forward */

        
    physicalAddress = va;

    //setting value located at physicalAddress to val
    memcpy(val, physicalAddress, size);
    printf("get value done\n");
}



/*
This function receives two matrices mat1 and mat2 as an argument with size
argument representing the number of rows and columns. After performing matrix
multiplication, copy the result to answer.
*/
void MatMult(void *mat1, void *mat2, int size, void *answer) {

    /* Hint: You will index as [i * size + j] where  "i, j" are the indices of the
    matrix accessed. Similar to the code in test.c, you will use GetVal() to
    load each element and perform multiplication. Take a look at test.c! In addition to
    getting the values from two matrices, you will perform multiplication and
    store the result to the "answer array"*/
/*for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            GetVal((void *)address_a, &x, sizeof(int));
            GetVal((void *)address_b, &x, sizeof(int));
        }
    }
*/
}
