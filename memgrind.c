#include "mymalloc.c"
#include <time.h>
#include <math.h>
#include <sys/time.h>

long int runWorkload(char c);                               //runs 1 instance of a given workload, A to F, and returns its runtime in microseconds
void initPointers(char** pointers, int length);             //sets all pointers contained within the requested pointer array to NULL
void printPointers(char** pointers, int length);            //outputs all pointers in the pointer array, for debugging purposes
void freePointers(char** pointers, int length);             //for every address that still remains in pointer array, free that pointer from myblock
long int elapsed(struct timeval start, struct timeval end); //returns the elapsed time in microseconds from start to end, used for each workload
long int getAverage(long int* runtime);                     //returns the average of 50 iterations of a given workload using runtime array

//workloads A-C are described in Asst1 instructions
//workloads D-F are described in testcases.txt
void workloadA();
void workloadB();
void workloadC();
void workloadD();
void workloadE();
void workloadF();

int main(int argc, char** argv)         //output the results of running all workloads A to F
{
    printf("\n4096 bytes made available.\nNow running through all test cases.\n\n");

    long int aRuntimes[50];             //create arrays to store 50 runtimes per workload
    long int bRuntimes[50];
    long int cRuntimes[50];
    long int dRuntimes[50];
    long int eRuntimes[50];
    long int fRuntimes[50];

    for(int i = 0; i < 50; i++)         //run each workload, one after another, 50 times and store each runetime in their respective array
    {
        
        aRuntimes[i] = runWorkload('A');
        bRuntimes[i] = runWorkload('B');
        cRuntimes[i] = runWorkload('C');
        dRuntimes[i] = runWorkload('D');
        eRuntimes[i] = runWorkload('E');
        fRuntimes[i] = runWorkload('F');     
    }

    //print average runtimes for 50 iterations of each workload A to F
    
    printf("50 iterations of Workload A took on average: %ld microseconds\n\n", getAverage(aRuntimes));
    printf("50 iterations of Workload B took on average: %ld microseconds\n\n", getAverage(bRuntimes)); 
    printf("50 iterations of Workload C took on average: %ld microseconds\n\n", getAverage(cRuntimes)); 
    printf("50 iterations of Workload D took on average: %ld microseconds\n\n", getAverage(dRuntimes));
    printf("50 iterations of Workload E took on average: %ld microseconds\n\n", getAverage(eRuntimes));
    printf("50 iterations of Workload F took on average: %ld microseconds\n\n", getAverage(fRuntimes));

    return 0;
}

long int runWorkload(char c)            //runs 1 instance of a given workload, A to F, and returns its runtime in microseconds
{
    struct timeval start, end;
    switch(c)
    {
        case 'A':
            gettimeofday(&start, NULL);
            workloadA();
            gettimeofday(&end, NULL);
            break;
        case 'B':
            gettimeofday(&start, NULL);
            workloadB();
            gettimeofday(&end, NULL);
            break;
        case 'C':
            gettimeofday(&start, NULL);
            workloadC();
            gettimeofday(&end, NULL);
            break;
        case 'D':
            gettimeofday(&start, NULL);
            workloadD();
            gettimeofday(&end, NULL);
            break;
        case 'E':
            gettimeofday(&start, NULL);
            workloadE();
            gettimeofday(&end, NULL);
            break;
        case 'F':
            gettimeofday(&start, NULL);
            workloadF();
            gettimeofday(&end, NULL);
            break;
    }
    printf("Workload %c took: %ld microseconds\n\n", c, elapsed(start, end));
    return elapsed(start, end);
}

void initPointers(char** pointers, int length)      //sets all pointers within the given pointer array to NULL
{                                                   //for establishing a clean slate for workloads that depend on it
    for(int i = 0; i < length; i++)
    {
        pointers[i] = NULL;
    }
}

void printPointers(char** pointers, int length)    //outputs all pointers in the pointer array, for debugging purposes
{
    printf("\nPrinting pointerArray ...\n");
    int i = 0;
    while(i < length)
    {
        if(pointers[i] != NULL)
        {
             printf("pointerArray[%d] at %p\n", i, pointers[i]);
        }
        i++;
    }
    printf("\n...done\n\n");
}

void freePointers(char** pointers, int length)      //for every address that still remains in pointer array, free that pointer from myblock
{
    for(int i = 0; i < length; i++)
    {
        if(pointers[i] != NULL)
        {
            free(pointers[i]);
            pointers[i] = NULL;
        }
    }
    printf("All pointers from pointer array have been freed from myblock\n");
}

long int elapsed(struct timeval start, struct timeval end)      //returns the elapsed time in microseconds from start to end, used for each workload
{   
    return (((end.tv_sec * pow(10, 6)) + end.tv_usec) - ((start.tv_sec * pow(10, 6)) + start.tv_usec));
}

long int getAverage(long int* runtime)          //returns the average of 50 iterations of a given workload using runtime array
{
    long int sum = 0;
    for(int i = 0; i < 50; i++)
    {
        sum += runtime[i];
    }
    return sum / 50;
}

//All workloads A to F are implemented below

void workloadA()
{
    printf("Beginning workload A...\n");
    for(int i = 0; i < 120; i++)
    {
        char* c = (char*)malloc(1);
        free(c);
    }
    printf("Workload A completed\n");
}

void workloadB()
{
    printf("Beginning workload B...\n");

    char* pointerArray[120];
    for(int i = 0; i < 120; i++)
    {   
        pointerArray[i] = (char*)malloc(1);
    }
    freePointers(pointerArray, 120);

    printf("Workload B completed\n");
}

void workloadC()
{
    printf("Beginning workload C...\n");

    char* pointerArray[120];
    int mallocNum = 0;
    int freeNum = 0;
    int top = 0;

    while(mallocNum + freeNum < 240)
    {
        int coinFlip = rand() % 2;
        if(mallocNum < 120 && coinFlip == 1)
        {
            pointerArray[top] = (char*)malloc(1);
            top++;
            mallocNum++;
        }
        if(top - 1 >= 0 && coinFlip == 0)
        {
            free(pointerArray[top - 1]);
            pointerArray[top - 1] = NULL;
            top--;
            freeNum++;
        }
    }

    printf("Workload C completed\n");
}

void workloadD()
{
    printf("Beginning workload D...\n");

    char* pointerArray[240];
    initPointers(pointerArray, 240);
    int mallocAttempt = 0;
    int freeAttempt = 0;
    int top = 0;

    while(mallocAttempt + freeAttempt < 240)
    {
        int coinFlip = rand() % 2;
        if(coinFlip == 1)
        {
            int size = rand() % 512;
            char* c = (char*)malloc(size);
            if(c != NULL)
            {
                pointerArray[top] = c;
                top++;
            }
            mallocAttempt++;
        }
        if(coinFlip == 0)
        {
            if(top - 1 >= 0)
            {
                free(pointerArray[top - 1]);
                pointerArray[top - 1] = NULL;
                top--;
            }
            freeAttempt++;
        }
    }  
    freePointers(pointerArray, 240);

    printf("Workload D completed\n");
}

void workloadE()
{   
    printf("Beginning workload E...\n");

    char *pointerArray[4];
    initPointers(pointerArray, 4);

    pointerArray[0] = malloc(100);
    pointerArray[1] = malloc(50);
    pointerArray[2] = malloc(200);

    free(pointerArray[0]);
    pointerArray[0] = NULL;
    free(pointerArray[1]);
    pointerArray[1] = NULL;

    pointerArray[3] = malloc(130);

    free(pointerArray[2]);
    pointerArray[2] = NULL;

    freePointers(pointerArray, 4);
    printf("Workload E completed\n");
}

void workloadF()
{
    printf("Beginning workload F...\n");

    char *pointerArray[50];
    initPointers(pointerArray, 50);

    for (int i = 0; i < 40; i++)
    {
        pointerArray[i] = (char*)malloc(95);
    }

    //frees the odd numbered mallocs
    for (int i = 0; i < 40; i++)
    {
        if (i % 2 == 1)
        {
            free(pointerArray[i]);
            pointerArray[i] = NULL;
        }
    }

    //frees the even numbered malloc pointers
    //this will merge the free blocks in memory
    for (int i = 0; i < 40; i++)
    {
        if (i % 2 == 0)
        {
            free(pointerArray[i]);
            pointerArray[i] = NULL;
        }
    }
    freePointers(pointerArray, 50);

    printf("Workload F completed\n");
}