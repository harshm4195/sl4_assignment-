//BT20CSE052-Siddheshkumar Jain
//CPL-3 Assignment

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct free_list{
    struct free_list* next;             //Pointer to next free block
    struct free_list* prev;             //Pointer to prev free block
    int size;                           //Size of free block
    int start;                          //Starting point of free block
};

struct allocated_list{
    struct allocated_list* next;        //Pointer to next allocated block
    struct allocated_list* prev;        //Pointer to previous allocated block
    int size;                           //Size of process that is allocated 
    int start;                          //Starting point of allocated block
    char processID[30];                 //ID of allocated  block
};

struct free_list* FreeListHead;         //head of free list
struct allocated_list* allocatedHead;   //head of allocated process list
int HEAPSIZE = 3000;                    //Heap storage available

void allocate_memory();
void deallocate_memory(char []);
void merge_free_list(struct free_list*);
void check_ID(char []);
void print_allocated_list();
void print_free_list();

int main()
{
    int choice=1;
    char processID[30];
    
    FreeListHead = (struct free_list*)malloc(sizeof(struct free_list));
    FreeListHead->next = FreeListHead->prev = NULL;
    FreeListHead->size = HEAPSIZE;
    FreeListHead->start = 0;
    allocatedHead = NULL;
    
    printf("------------------------------------------------------------------------------------------------\n");
    printf("\t Memory allocation using FIRST FIT STRATEGY with memory of 3000 units available.\n");
    printf("\t Please take care that memory asked for allocation is in integer format.\n");
    printf("------------------------------------------------------------------------------------------------\n\n");
    
    do
    {
        printf("Choose any one of the following options\n\n");
        printf("1. Allocate Memory\n");
        printf("2. Deallocate Memory\n");
        printf("3. Allocated memory status\n");
        printf("4. Free list status\n");
        printf("0: EXIT\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);
        switch(choice)
        {
            
            case 1:
                allocate_memory();
                break;
                
            case 2:
                printf("Please enter the ID of the process to be deallocated from heap memory: ");
                scanf("%s",processID);
                deallocate_memory(processID);
                break;
            
            case 3:
                print_allocated_list();
                break;
                
            case 4:
                print_free_list();
                break;
                
            case 0:
                break;    

            default:
                printf("Wrong option entered\n");
                break;
                
        }
        
        printf("-------------------------------------------------------------------------------------------\n");
        printf("Live Status\n");
        printf("\nAllocated List is :\n");
        print_allocated_list();
        printf("\nFree List is :\n");
        print_free_list();
        printf("--------------------------------------------------------------------------------------------\n");
        
        
    }while(choice!=0);
    
    return 0;

}

void allocate_memory()
{
    int processMemory;                          //To keep track of size to be allocated
    char processID[30];                         //Labelling block
    
    printf("Enter size you want to allocate : ");
    scanf("%d", &processMemory);
    printf("Enter the process ID: ");
    scanf("%s", processID);
    
    check_ID(processID);
    struct free_list* freeTemp = FreeListHead;
    
    /* Traverse till freeTemp->size< processMemory ,
        as we are using First-fit alogrith      */
        
    while (freeTemp!=NULL && freeTemp->size<processMemory)  
    {
        freeTemp = freeTemp->next;
    }
    
    /* If we reach end of list but can't find suitable free memory block */
    
    if (freeTemp == NULL)                            
    {
        printf("\n!!Memory can't be allocated!!\n\n");
    }
    else
    {

        /* If size of free list block is greater than required process size,
           It means free list block will be there with reduced size(freeTemp->size - processMemory) */
        
        if (freeTemp->size - processMemory > 0)
        {
            struct free_list* temp = (struct free_list*)malloc(sizeof(struct free_list)); 
            temp->start = freeTemp->start + processMemory;
            temp->next = NULL;
            temp->prev = NULL;
        
            if (freeTemp == FreeListHead)
            {
                temp->next = FreeListHead->next;
                FreeListHead = temp;
            }
            else
            {
                (freeTemp->prev)->next = temp;
                temp->next = freeTemp->next;
                temp->prev = freeTemp->prev;
                if(freeTemp->next)
                {
                    freeTemp->next->prev = temp;
                }
            }
            temp->size = freeTemp->size - processMemory;
        }
        else
        {   /*If size of free block equals required size,
              It means we free list block should be deleted */
              
            if (freeTemp->prev)
            {
                freeTemp->prev->next = freeTemp->next;
                if(freeTemp->next)
                {
                    freeTemp->next->prev = freeTemp->prev;
                }
            }
            else
            {
                FreeListHead = freeTemp->next;
                if(FreeListHead)
                {
                    FreeListHead->prev = NULL;
                }
            }
            
        }

        /* Allocating memory and adding node in allocated list */

        struct allocated_list* alloc_temp = (struct allocated_list*)malloc(sizeof(struct allocated_list));
        alloc_temp->next = NULL;
        alloc_temp->prev = NULL;
        alloc_temp->size = processMemory;
        strcpy(alloc_temp->processID , processID);
        alloc_temp->start = freeTemp->start;
        free(freeTemp);
        
        /* If allocated node is first one */
        
        if (allocatedHead == NULL)
        {
            allocatedHead = alloc_temp;
        }
        else
        {
            struct allocated_list* temp = allocatedHead;
            
            /* Finding the position where to allocate */
            
            while (temp->next && temp->start < alloc_temp->start)
            {
                temp = temp->next;
            }
            
            if (temp == allocatedHead && temp->start > alloc_temp->start)   // If node is to be allocated at first position itself 
            {
                alloc_temp->next = temp;
                temp->prev = alloc_temp;
                allocatedHead = alloc_temp;
            }
            else if (temp->next == NULL && temp->start < alloc_temp->start)  // If node is to be allocated at last position itself 
            {
                temp->next = alloc_temp;
                alloc_temp->prev = temp;
                alloc_temp->next = NULL;
            }
            else                                                            // If node is to be allocated in between
            {
                (temp->prev)->next = alloc_temp;
                alloc_temp->prev = temp->prev;
                alloc_temp->next = temp;
                temp->prev = alloc_temp;
            }
        }
        printf("Allocated from index: %d to %d \n" ,alloc_temp->start ,alloc_temp->start + alloc_temp->size-1);
    }
    return;
}

void deallocate_memory(char processID[])
{
    struct allocated_list* allocateTemp = allocatedHead;
    while (allocateTemp!=NULL && strcmp(allocateTemp->processID ,processID))             // Finding node with given processID 
    {
        allocateTemp = allocateTemp->next;
    }
    if (allocateTemp == NULL)
    {
        printf("Not found\n");
    }
    else
    {
        /* the node that is to be inserted in freelist after being deallocated */
        
        struct free_list* fptr = (struct free_list*)malloc(sizeof(struct free_list));
        fptr->next = NULL;
        fptr->prev = NULL;
        fptr->size = allocateTemp->size;
        fptr->start = allocateTemp->start;
        
        if (allocateTemp == allocatedHead)                           //If memory to be deallocated is first node itself
        {
            allocatedHead = allocateTemp->next;
        }
        else
        {
            allocateTemp->prev->next = allocateTemp->next;
            if (allocateTemp->next)
            {
                allocateTemp->next->prev = allocateTemp->prev;
            }
        }
        
        struct free_list* temp = FreeListHead;
        if (FreeListHead == NULL)
        {
            FreeListHead = fptr;
        }
        else
        {
            while (temp && temp->next && temp->start < fptr->start)
            {
                temp = temp->next;
            }
    
            if (temp && temp->next == NULL && temp->start < fptr->start)     //If freelist node is to be inserted at end
            {
                temp->next = fptr;
                fptr->prev = temp;
            }
            else                                                            //If freelist node is to be inserted at start
            {
                if (temp && temp == FreeListHead)
                {
                    fptr->next = temp;
                    temp->prev = fptr;
                    FreeListHead = fptr;
                }
                else if(temp)                                               //If freelist node is to be inserted in between
                {
                    fptr->prev = temp->prev;
                    temp->prev->next = fptr;
                    temp->prev = fptr;
                    fptr->next = temp;
                }

            }
        }
        merge_free_list(fptr);                                      //Merging continguous free blocks
    }
    return;
}


void merge_free_list(struct free_list* fptr)
{
    if(fptr->prev)                                                 //if previous node exists and is adjacent
    {
        if (fptr->prev->start + fptr->prev->size == fptr->start)
        {
            fptr->prev->size = fptr->prev->size + fptr->size;
            fptr->prev->next = fptr->next;
            if(fptr->next)
            {
                fptr->next->prev = fptr->prev;
            }
            fptr = fptr->prev;
        }
    }
    if(fptr->next)                                             //If next node exists and is adjacent
    {
        if (fptr->start + fptr->size == fptr->next->start)
        {
            fptr->size += fptr->next->size;
            fptr->next = fptr->next->next;
            if(fptr->next)
            {
                fptr->next->prev = fptr;
            }
        }
    }
    return;
}

void check_ID(char s[])
{
    int flag = 1;
    struct allocated_list* allocateTemp = allocatedHead;
    while (allocateTemp && flag)
    {
        if (strcmp(allocateTemp->processID , s)==0)
        {
            printf("\n!!Process with same ID found!!\nHence deallocating previous one\n");
            deallocate_memory(s);
            flag = 0;
        }
        allocateTemp = allocateTemp->next;
    }
    return;
}

void print_allocated_list()
{
    struct allocated_list* allocateTemp = allocatedHead;
    while(allocateTemp)
    {
        printf("%4d <---> %4d for process with processID ---> %s \n",allocateTemp->start,allocateTemp->start + allocateTemp->size-1,allocateTemp->processID);
        allocateTemp = allocateTemp->next;
    }
    return;
}

void print_free_list()
{
    struct free_list* freeTemp = FreeListHead;
    while (freeTemp)
    {
        printf("%4d< ---> %4d \n",freeTemp->start,freeTemp->start + freeTemp->size-1);
        freeTemp = freeTemp->next;
    }
    return;
}
