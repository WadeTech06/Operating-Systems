Lakiel Wade 2601283

Assignment Description
-----------------------
-----------------------
The goal of this lab is to write a simple memory management simulator based on the topics covered in class.
You must write a memory manager that supports both segmentation and paged memory allocation.

holes are stored in std:List this container is a doubly linked list
that allocate and deallocate memory dynamically for new holes. the List is sorted by pid;
-----------------------
Compiling instruction
-----------------------
----------------------
File 1
run -> g++ -g -std=c++11 MM.cpp
-> ./a.out page.txt


File 2
run -> ./a.out frag.txt
------------------------
A sample test run
------------------------
-----------------------
File 1
/cis345/Project2$ ./a.out page.txt
Memory size = 1024, total pages = 32
allocated pages = 21, free pages = 11
There are currently 4 active process
Process list:
Process id=1, size=259bytes, number of pages=9
Virt Page 0 -> Phys Page 0x1078f10 used: 32 bytes
Virt Page 1 -> Phys Page 0x1078fc0 used: 32 bytes
Virt Page 2 -> Phys Page 0x10790a0 used: 32 bytes
Virt Page 3 -> Phys Page 0x1079180 used: 32 bytes
Virt Page 4 -> Phys Page 0x1079260 used: 32 bytes
Virt Page 5 -> Phys Page 0x1079340 used: 32 bytes
Virt Page 6 -> Phys Page 0x1079420 used: 32 bytes
Virt Page 7 -> Phys Page 0x1079500 used: 32 bytes
Virt Page 8 -> Phys Page 0x10795e0 used: 3 bytes
Process id=4, size=32bytes, number of pages=1
Virt Page 9 -> Phys Page 0x107a610 used: 32 bytes
Process id=5, size=36bytes, number of pages=2
Virt Page 10 -> Phys Page 0x107a6f0 used: 32 bytes
Virt Page 11 -> Phys Page 0x107a7a0 used: 4 bytes
Process id=6, size=257bytes, number of pages=9
Virt Page 12 -> Phys Page 0x107a7e0 used: 32 bytes
Virt Page 13 -> Phys Page 0x107a890 used: 32 bytes
Virt Page 14 -> Phys Page 0x107a940 used: 32 bytes
Virt Page 15 -> Phys Page 0x107a9f0 used: 32 bytes
Virt Page 16 -> Phys Page 0x107aaa0 used: 32 bytes
Virt Page 17 -> Phys Page 0x107ab50 used: 32 bytes
Virt Page 18 -> Phys Page 0x107ac00 used: 32 bytes
Virt Page 19 -> Phys Page 0x107acb0 used: 32 bytes
Virt Page 20 -> Phys Page 0x107ad60 used: 1 bytes

Failed allocations (No memory) = 1
Deleted allocations = 2


File 2
/cis345/Project2$ ./a.out frag.txt
allocated memory size is smaller than request: 4
Memory size = 1024 bytes, Allocated = 545 bytes, Free = 479 bytes
There are currently 3 active process
Process list:
Proccess id = 3 Size = 512
 text start = 0xb23540, size = 300
 data start = 0xb23a00, size = 98
 heap start = 0xb23b90, size = 114

Proccess id = 5 Size = 257
 text start = 0xb24640, size = 200
 data start = 0xb24970, size = 40
 heap start = 0xb234b0, size = 17

Proccess id = 6 Size = 257
 text start = 0xb24ab0, size = 200
 data start = 0xb24de0, size = 40
 heap start = 0xb24a20, size = 17


Failed allocations (No memory) = 1
Deleted allocations = 2


