# User-Level Memory Management Library
# Author: Nandani Patidar & Sreeram Maddineni

The goal of this project is to implement “myalloc()” which will return a virtual address that maps to a physical page.
Here, physical memory is a large region of contiguous memory which can be allocated using mmap() and malloc() 

We have used 32-bit address space which can support up to 4GB of address space.

Part 1 - Implementing a Virtual Memory System -

  The following are the APIs our library have:-
  1. SetPhysicalMem()
  2. Translate()
  3. PageMap()
  4. myalloc()
  5. myfree()
  6. PutVal() / GetVal()
  7. MatMul()
  
Part 2 - Implementation of a TLB - 

  1.Logic
  2.Number of entries in TLB
  3.TLB entry size
  4.TLB Eviction
  5.Expected Output
  
Note: For detailed description please check Project Description - Virtual Menory Implementation file in the main folder.
