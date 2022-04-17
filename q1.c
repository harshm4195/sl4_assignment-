
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#define N 3
#define SIZE 8
struct node
{
    int data;
    bool mark;
    int refCount;
    struct node *link[N];
};
struct heap
{
    struct node memory;
    struct heap *next;
};
struct heap *head=NULL;
struct node *ptr[SIZE];
struct node* insertNode(int size,struct node *link1,struct node *link2,struct node *link3)
{
    struct heap *ptr=malloc(sizeof(struct heap));
    ptr->memory.data=size;
    ptr->memory.mark=false;
    ptr->memory.link[0]=link1;
    ptr->memory.link[1]=link2;
    ptr->memory.link[2]=link3;
    ptr->memory.refCount=0;
    ptr->next=head;
    head=ptr;
    struct node *temp=&(ptr->memory);
    if(link1 != NULL)
    {
        link1->refCount +=1;
    }
    if(link2 != NULL)
    {
        link2->refCount +=1;
    }
    if(link3 != NULL)
    {
        link3->refCount +=1;
    }
    
    return temp;
}
void printList()
{
    struct heap *temp=head;

    while(temp!=NULL)
    {
        printf("(%d , count=%d , mark=%d) ->",temp->memory.data,temp->memory.refCount,temp->memory.mark);
        temp=temp->next;
    }
    printf("NULL");
}
void allocate(struct node *root)
{
    (root)->refCount +=1;
}
void printRef(struct node *root)
{
    if(root!=NULL)
    {
        printf("%d(count=%d) ",root->data,root->refCount);
        printRef(root->link[0]);
        printRef(root->link[1]);
        printRef(root->link[2]);
    }
}
void garbageCollect(struct node *ptr[],int n)
{
    int j=n-1;
    struct heap *temp=head;
    struct heap *prev=NULL;
    
    while(temp!=NULL)
    {
        if(temp->memory.refCount==0)
        {
            for(int i=0;i<N;i++)
            {
                if(temp->memory.link[i] != NULL)
                {
                    temp->memory.link[i]->refCount -=1;
                }
            }
            prev->next=temp->next;
            free(temp);
            temp=prev;
            ptr[j]=NULL;
        }
        prev=temp;
        temp=temp->next;
        j--;
    }
}
void markNodes(struct node *root)
{
    if(root!=NULL)
    {
        markNodes(root->link[0]);
        markNodes(root->link[1]);
        markNodes(root->link[2]);
        root->mark=true;
    }
}
void sweepalgo()
{
    int j=SIZE-1;
    struct heap *temp=head;
    struct heap *prev=NULL;
    
    while(temp!=NULL)
    {
        if(temp->memory.mark==false)
        {
            for(int i=0;i<N;i++)
            {
                if(temp->memory.link[i] != NULL)
                {
                    temp->memory.link[i]->refCount -=1;
                }
            }
            prev->next=temp->next;
            free(temp);
            temp=prev;
            ptr[j]=NULL;
        }
        prev=temp;
        temp=temp->next;
        j--;
    }
}
int main()
{
    ptr[0]=insertNode(10,NULL,NULL,NULL);
    ptr[1]=insertNode(9,NULL,NULL,NULL);
    ptr[2]=insertNode(2,NULL,NULL,NULL);
    ptr[3]=insertNode(8,ptr[1],NULL,NULL);
    ptr[4]=insertNode(1,ptr[2],ptr[1],ptr[0]);
    ptr[5]=insertNode(3,ptr[3],ptr[0],NULL);
    ptr[6]=insertNode(7,ptr[4],ptr[3],NULL);
    ptr[7]=insertNode(5,ptr[4],NULL,NULL);
    struct node *r1,*r2;
    r1=ptr[7];
    allocate(r1);
    r2=ptr[4];
    allocate(r2);
    for(int i=SIZE-1;i>=0;i--)
    {
        printRef(ptr[i]);
        printf("\n");
    }
    printf("\n-------------------------\n");
    printf("Heap connected to root node is : \n");
    printRef(r1);
    printf("\n");
    printRef(r2);
    printf("\nGC COLLECTOR : \n");
    garbageCollect(ptr,SIZE);
    for(int i=SIZE-1;i>=0;i--)
    {
        printRef(ptr[i]);
        if(ptr[i]!=NULL)
            printf("\n");
    }
    printf("Complete List is : \n");
    printList();
    markNodes(r1);
    markNodes(r2);
    sweepalgo();
    printf("\nComplete List is : \n");
    printList();

    return 0;
}
