#include "my_vm.h"
void *physicalMemory;
int numberOfVirtPages;
int numberOfPhysPages;
bool initializePhysicalFlag = false;

//TO-DO 1 Initialize following in the SetPhysicalMem() using bitmap
int** innerPagetable;       //size of pte_t
bool* physicalCheckFree;
bool* virtualCheckFree;

//int tukdetukde = numberOfVirtPages*sizeof(int)/(PGSIZE);
unsigned int secondTenBitsMask = 4190208;
unsigned int lastTwelveBitsMask = 4095;
int pageTableEntriesPerBlock = 1024;
int* outerPageTable[1024];      //pde_t 

const int innerLength = floor((32 - log2(PGSIZE))/2);
const int outerLength = ceil((32 - log2(PGSIZE))/2);
const int offsetLength = log2(PGSIZE);

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

    int i;
    innerPagetable = malloc(numberOfVirtPages*sizeof(int*));
    for(i = 0; i<numberOfVirtPages; ++i){
        innerPagetable[i] = NULL;
    }

    physicalCheckFree = malloc(numberOfPhysPages*sizeof(bool));
    for(i = 0; i<numberOfPhysPages; ++i){
        physicalCheckFree[i] = true;
    }

    virtualCheckFree = malloc(numberOfVirtPages*sizeof(bool));
    for(i = 0; i<numberOfVirtPages; ++i){
        virtualCheckFree[i] = true;
    }


    //innerPagetable[numberOfVirtPages];   
        //size of pte_t
    //physicalCheckFree[numberOfPhysPages] = { true };
    //virtualCheckFree[numberOfVirtPages] = { true };

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




    printf(stderr, "TLB miss rate %lf \n", miss_rate);
}


/*
The function takes a virtual address and page directories starting address and
performs translation to return the physical address
*/
pte_t * Translate(pde_t *pgdir, void *va) {
    //HINT: Get the Page directory index (1st level) Then get the
    //2nd-level-page table index using the virtual address.  Using the page
    //directory index and page table index get the physical address
    printf("Inside translate \n");
    unsigned int va_int = va; 
    unsigned int firstTenbitsVA = va_int >> 22;
    int pgdirVal = outerPageTable[firstTenbitsVA];
    pte_t *pa;
        
    unsigned int nextTenbitsVA = (va_int & secondTenBitsMask) >> 12;
    int addressInnerPgTable = pgdirVal *pageTableEntriesPerBlock + nextTenbitsVA;

    unsigned int lastTwelvebitsVA = va_int & lastTwelveBitsMask;

    printf("Address Inner Page Table in Translate %d\n", addressInnerPgTable);

    if (innerPagetable[addressInnerPgTable] != NULL){
        pa = innerPagetable[addressInnerPgTable];
        printf("PA in Translate before: %u\n", pa);

        // adding offset to PA
        pa = (char*) pa + lastTwelvebitsVA;
        printf("PA in Translate After offset: %u\n", pa);
        return pa;
    }
        
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
    printf("Inside PageMap \n");
    printf("PA inside PageMap %u\n", pa);
    unsigned int va_int = va; 
    unsigned int firstTenbitsVA = va_int >> 22;
    if (outerPageTable[firstTenbitsVA] == NULL){
        outerPageTable[firstTenbitsVA] = pgdir;
    }

    unsigned int pgdir_int = pgdir;
    unsigned int nextTenbitsVA = (va_int & secondTenBitsMask) >> 12;
    int addressInnerPgTable = (pgdir_int * pageTableEntriesPerBlock) + nextTenbitsVA;

    printf("Address Inner Page Table in PageMap %d\n", addressInnerPgTable);
    if (innerPagetable[addressInnerPgTable] == NULL){
        innerPagetable[addressInnerPgTable] = pa;
         printf("PA inside PageMap if condition: %u\n", innerPagetable[addressInnerPgTable]);
    }

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
    // pFreeAddress = physicalCheckFree;

    //Use virtual address bitmap to find the next free page
    for (int i = 0; i < numberOfPhysPages; i++) {     // numberOfFrames = numberOfPhysPages
        if (physicalCheckFree[i] == true){
            physicalCheckFree[i] = false;
            printf("PA Free Flag: %d\n", i); 
            printf("PA Free Flag: %u\n", physicalMemory + i*PGSIZE);   
            return physicalMemory + i*PGSIZE;     //Have to test
        }
    }
    return NULL;
}

int get_next_avail_va(int num_pages) {

    //Use virtual address bitmap to find the next free page
    for (int i = 0; i < (numberOfVirtPages - num_pages); i++) {     // numberOfFrames = numberOfPhysPages
        if (virtualCheckFree[i] == true){
            printf("Inside get_next_avail_va if condition\n");
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
                    
            }
            printf("Virtual address inside get_next_avail_va: %d\n", i);
            return i;     //Have to test 

        }
    }
    return -1;
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
    printf("Outer page table: %d\n", outerPageTable[1]);

   /* HINT: If the page directory is not initialized, then initialize the
   page directory. Next, using get_next_avail(), check if there are free pages. If
   free pages are available, set the bitmaps and map a new page. Note, you will
   have to mark which physical pages are used. */

    //numner of pages required when myalloc is called 
    int num_pages = num_bytes/PGSIZE;
    if (num_bytes % PGSIZE != 0){
        num_pages = num_pages + 1; 
    }
    
    printf("Searching for physical memory\n");
    
    if (check_require_avail_pa(num_pages) == false){
        printf("Physical memory not found\n");
        return NULL;
    }

    printf("Searching for virtual memory\n");

    int va_EntryNumber;
    va_EntryNumber = get_next_avail_va(num_pages);   // check null condition in pointer
    if (va_EntryNumber == -1){
        printf("virtual memory is not available\n");
        return NULL;
    }
    
    int pgDirEntryNumber;
    pgDirEntryNumber = va_EntryNumber / pageTableEntriesPerBlock; 

    int pgTableEntryNumberInBlock = va_EntryNumber % pageTableEntriesPerBlock; 

    // calculate 32- bit VA
    //void * innerPageTableEntryAddr = innerPagetable + va_EntryNumber*sizeof(int);
   unsigned int va_int = pgDirEntryNumber;

   int firstTenbitsVA = va_int >> (offsetLength + innerLength);
   int nextTenbitsVA = ((1 << innerLength) - 1)  &  (va_int >> (offsetLength));
   int offset = ((1 << offsetLength) - 1)  &  (va_int);

   printf("S VA initial 10 bits: %d\n", firstTenbitsVA);
   printf("S VA next 10 bits: %d\n", nextTenbitsVA);
   printf("S offset %d\n", offset);

    // unsigned int va_int = pgDirEntryNumber;
    // va_int = va_int << 10;
    // va_int = va_int | pgTableEntryNumberInBlock;
    // va_int = va_int << 12;  // last 12 bits for offset from 32 bit VA
    // //unsigned int firstTenBits = 4290772992;
    
    // printf("VA initial 10 bits: %u\n", va_int >> 22);
    // printf("VA next 10 bits: %u\n", (va_int & secondTenBitsMask)>> 12); 
    void *va = va_int;

    pde_t *pgDir = pgDirEntryNumber; // Assuming page directory entry number is same as inner page block number
    void *pa;

    for (int i = 0; i < num_pages; i++) {
        //checking for next free pages and getting the physical address of that page.
        pa = get_next_avail_pa(num_pages);   // check null condition in pointer
        if (pa == NULL){
           printf("This should never happen\n");
        }
        PageMap(pgDir, va, pa);
    }
    return va;
}

/* Responsible for releasing one or more memory pages using virtual address (va)
*/
void myfree(void *va, int size) {

    //Free the page table entries starting from this virtual address (va)
    // Also mark the pages free in the bitmap
    //Only free if the memory from "va" to va+size is valid
    //free(va);
    printf("My Free started\n");
    int num_pages = size/PGSIZE;
    if (size % PGSIZE != 0){
        num_pages = num_pages + 1; 
    }
    unsigned int va_int = va; 
    unsigned int firstTenbitsVA = va_int >> 22;
    int pgdirVal = outerPageTable[firstTenbitsVA];
    
    unsigned int nextTenbitsVA = (va_int & secondTenBitsMask) >> 12;
    int addressInnerPgTable = pgdirVal *pageTableEntriesPerBlock + nextTenbitsVA;

    for (int i = 0; i < num_pages; i++) {
        void *pa = innerPagetable[addressInnerPgTable + i];
        innerPagetable[addressInnerPgTable + i] = NULL;
        physicalCheckFree[(pa - physicalMemory)/PGSIZE] = true;
        virtualCheckFree[addressInnerPgTable + i] = true;  
    }
}


/* The function copies data pointed by "val" to physical
 * memory pages using virtual address (va)
*/
void PutVal(void *va, void *val, int size) {

    /* HINT: Using the virtual address and Translate(), find the physical page. Copy
       the contents of "val" to a physical page. NOTE: The "size" value can be larger
       than one page. Therefore, you may have to find multiple pages using Translate()
       function.*/

    printf("Put value\n");

    // int num_pages = size/PGSIZE;
    // if (size % PGSIZE != 0){
    //     num_pages = num_pages + 1; 
    // }
    // printf("Put value 2\n");

    pte_t * physicalAddress;
    for (int i = 0; i < size; i++) {
        physicalAddress = Translate(NULL, (char*) va + i );  // TO-DO check va 
        printf("After translate\n");
        printf("Val:    %u\n", (char*)val+i);
        //setting value to a address(physicalAddress) 
        memcpy(physicalAddress, (char*)val+i, 1);
    }
}


/*Given a virtual address, this function copies the contents of the page to val*/
void GetVal(void *va, void *val, int size) {

    /* HINT: put the values pointed to by "va" inside the physical memory at given
    "val" address. Assume you can access "val" directly by derefencing them.
    If you are implementing TLB,  always check first the presence of translation
    in TLB before proceeding forward */
    printf("Get value\n");
    // int num_pages = size/PGSIZE;
    // if (size % PGSIZE != 0){
    //     num_pages = num_pages + 1; 
    // }
    // printf("Get value 2 \n");

    pte_t * physicalAddress;
    for (int i = 0; i < size; i++) {
        physicalAddress = Translate(NULL, (char*) va + i);

        printf("Val:    %u\n", (char*)val+i);
        //setting value located at physicalAddress to val
        memcpy((char*)val+i, physicalAddress, 1);
    }
    printf("Get value done\n");
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
