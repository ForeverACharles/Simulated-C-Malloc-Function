#include "mymalloc.h"

static char myblock[4096];          //4096 byte memory block to simulate malloc and free
/*
memblock metadata structure
1st byte - free 'f' and in-use 'u' flags
2nd & 3rd byte - used to denote size allocated for user's request
4th & 5th byte - used to denote index of the next block in myblock, a value of -1 represents no blocks remaining
*/

void initBlock();                   //initialize the first block in myblock
int setBlock(int size);             //mymalloc makes call to this main function to allocate requested memory
int getSize(int i);                 //retrieves size of current block from current block metadata
int getNext(int i);                 //retrieves next block index from current block metadata
void setSize(int i, int size);      //sets current block size metadata
void setNext(int i, int size);      //sets the current block's next metadata to the index of the next block
void combineBlocks(int i);          //combines all adjacent freed blocks in myblock, starting at i
void printMyBlock();                //prints all memory blocks, used and freed in myblock for debugging purposes

void* mymalloc(size_t size, char* fileName, int line)
{   
    //handles case for when negative input passed in
    if((int)size < 0)
    {
        printf("\033[1;31mError:\t\033[0mCannot allocate space of negative size in file: %s line: %d\n\n", fileName, line);
        return NULL;
    }

    //initialize myblock to have 1 free region of 4091 bytes
    if(myblock[0] != 'f' && myblock[0] != 'u')
    {
        initBlock();
    }

    //if allocation successful --> return the address to beginning of user's block
    int returnIndex = setBlock(size);           
    if(returnIndex > 0)                     
    {
        //printf("Returning malloc of size %d bytes starting at: %p\n", (int)size,  &(myblock[returnIndex]));
        return &(myblock[returnIndex]);
    }

    //if allocation failed --> tell user that there is not enough space in myblock to allocate requested size
    printf("\033[1;31mError:\t\033[0mInsufficient memory for requested allocation of %d bytes\n\tin file: %s: line: %d\n\n", (int)size, fileName, line);
    return NULL;
}

void initBlock()    //initialize the first block in myblock as a single 4091-sized free block
{
    myblock[0] = 'f';
    myblock[1] = 40;
    myblock[2] = 91;
    myblock[3] = 0;
    myblock[4] = -1;        //-1 indicates end of myblock
}

int setBlock(int size)      //mymalloc makes call to this main function to allocate requested memory
{
    int i = 0;
    while(i < 4096 && i != -1)     //main loop for malloc, to look for a valid free block to allocate user requested memory
    {
        if(myblock[i] == 'f' && size <= getSize(i))
        {
            int blockStart = i + 5;                     //index of myblock to return to the user
            int remainingSize = getSize(i) - size;      //size remaining in the current block after a malloc attempt
           
            //give the user the full block if there isnt enough space to make another free block afterwards
            //otherwise split the block into a used and free partition

            myblock[i] = 'u';                   //set flag from free to in-use
            if(remainingSize > 4)               //handle case for when this free block can be split
            {                                   //meaning: the user requested size is less than current block's total size
                setSize(i, size);  
                if(getNext(i) == -1)            //handle case for when current block to be split is at the end of myblock
                {
                    setNext(i, size);                       //set metadata for current block to be allocated and next block
                    myblock[getNext(i)] = 'f';                      
                    setSize(getNext(i), remainingSize - 5); 
                    setNext(getNext(i), -1);
                    return blockStart;
                }
                setNext(i, size);                           //set metadata for current block to be allocated and next block
                myblock[getNext(i)] = 'f';
                setSize(getNext(i), remainingSize - 5);
                setNext(getNext(i), remainingSize - 5);
            }
            return blockStart;                              //return index of user allocated space in myblock
        }
        i = getNext(i);
    }
    return 0;                   //no free blocks available to fit the requested size
}

int getSize(int i)              //retrieves the size of current block from current block metadata
{
    return (myblock[i + 1] * 100) + myblock[i + 2]; 
}

int getNext(int i)              //retrieves the next block index from current block metadata
{
    return (myblock[i + 3] * 100) + myblock[i + 4];
}

void setSize(int i, int size)   //sets the current block's size metadata based on requsted size
{
    myblock[i + 1] = size / 100;
    myblock[i + 2] = size % 100;
}

void setNext(int i, int size)   //sets the current block's next metadata to the index of the next block
{                               
    if(size == -1)              //handle case for end of myblock reached
    {
        myblock[i + 3] = 0;
        myblock[i + 4] = -1;
        return;
    }
    if(size == 0)               //handle case for when enough size is available to allocate only 5 bytes of metadata
    {                           //for the next block
        myblock[i + 3] = 0;
        myblock[i + 4] = 5;
    }
    myblock[i + 3] = (i + 5 + size) / 100;     //use the requested size to set the next index
    myblock[i + 4] = (i + 5 + size) % 100;
}

void myfree(void* mem_addr, char* fileName, int line)       //attempts to free the requested address
{
    int i = 0;                      //main loop to search for the requested address in myblock
    while(i < 4096)                 //if the address is found and belongs to a block in-use, free that block
    {
        if(myblock[i] == 'u' && &(myblock[i + 5]) == mem_addr)
        {
            myblock[i] = 'f';       //set flag from in-use to free
            //printf("Succesfully freed %d bytes via myblock[%d] at: %p\n", getSize(i), i, &(myblock[i]));
            break;
        }
        if(getNext(i) == -1)        //-1 indicates end of myblock reached, so address requested was not able to be freed
        {
            printf("\033[1;31mError: \033[0mInvalid free of %p in file: %s: line: %d\n\n", mem_addr, fileName, line);
            return;
        }
        i = getNext(i);
    }

    i = 0;
    while(getNext(i) != -1)                                     //combine all adjacent blocks in myblock
    {
        if(myblock[i] == 'f' && myblock[getNext(i)] == 'f')     //check for adjacent freed blocks
        {
            combineBlocks(i);                                   //combine this free block with the next one
        }
        else
        {
            i = getNext(i);                                     
        }
    }
}

void combineBlocks(int i)                   //combines all adjacent freed blocks in myblock, starting at i
{
    int newSize = getSize(i) + 5 + getSize(getNext(i));     //new block size becomes the sum of current size and the next block's size
    int newNext = getNext(getNext(i));                      //current block's next becomes the next block's next index

    myblock[getNext(i)] = 0;                                //set all metadata of the adjacent free block to null
    myblock[getNext(i) + 1] = 0;
    myblock[getNext(i) + 2] = 0;
    myblock[getNext(i) + 3] = 0;
    myblock[getNext(i) + 4] = 0;

    setSize(i, newSize);                                   
    if(newNext == -1)                   //handle case for when the new next index is the end of myblock
    {
        setNext(i, -1);                 //next will be set to indicate the end of myblock
        return;
    }
    setNext(i, newSize);
}

void printMyBlock()             //prints all memory blocks, used and freed, in myblock for debugging purposes
{
    printf("Printing all blocks...\n");
    for(int i = 0; i < 4096; i++)
    {
        if(myblock[i] == 'u' || myblock[i] == 'f')
        {
             printf("myblock[%d]\t= [%c|%d|%d]\tat: %p\n", i, myblock[i], getSize(i), getNext(i), &(myblock[i]));
        }
    }
    printf("\n...done\n\n");
}

