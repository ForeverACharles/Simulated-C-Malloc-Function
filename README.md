# Simulated C Malloc Function

C's malloc() and free() dynamic memory allocation functions simulated using fixed-length 4096 byte array

### Implementation Details

Metadata is used to keep track of the different memory blocks in the array. The metadata portion of each block consists of 5 bytes. These 5 bytes are prepended to each malloc()ed memory block, regardless of the requested size. Metadata is split into 3 parts:

- The first byte is either ‘f’ or ‘u’ and indicates whether the memory block is free or used.
- The next two bytes represent the size of the requested memory block
- The final two bytes represent the location of the next memory block (the beginning of the metadata belonging to the next block)

#### The following is an example of the metadata required to represent an allocated section of memory with a region of 1234 bytes starting at index 0 and ending at index 1239. 

| Byte 1 | Byte 2 | Byte 3 | Byte 4| Byte 5 | Allocated Region |
|--------|--------|--------|-------|--------|------------------|
|    u   |  12    |   34   |  12   |  40    |                  |

The next memory region will begin at index 1240 and its first byte will have assigned the free flag to it if the rest of the space is unused.

Depending on the size of the requested memory allocation, the location of the next memory block can be derived from either the user’s requested size or the size of a given memory block. This value is stored as an index that points to the beginning of the next block’s metadata. Storing the index of the next block allows us to traverse the array quickly. 

However, it’s also possible to implement this flattened linked list metadata without storing a pointer to the next block, using only 2 bytes for storing the size of the current requested block. Determining the start of the next block can be extrapolated using pointer arithmetic.

### Program Details

The executable runs 6 individual workloads to test the robustness of the custom malloc() function. Statiscial data is recorded and reported after all iterations of each workload are finished.

### How to run

1. Open the contents in a directory
2. Compile with gcc `make`
3. Run the `memgrind.o` exectuable
