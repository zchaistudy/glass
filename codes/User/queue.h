#include <stdio.h>  
#include <stdlib.h>  
typedef char QElemType;   
typedef struct QNode  
{  
        QElemType data;  
        struct QNode *next;  
}QNode, *QueuePtr;  
  
typedef struct  
{  
    QueuePtr front;  
    QueuePtr rear;  
}LinkQueue;  
void en_Queue(LinkQueue *q, QElemType e) ;
void init_Queue(LinkQueue *q) ;
void de_Queue(LinkQueue *q, QElemType *e) ;
int is_Empty(LinkQueue *q)  ;
void print_Queue(LinkQueue *q) ;
int getlength_Queue(LinkQueue *q)  ;
void top_Queue(LinkQueue *q, QElemType *e) ;
int is_Empty(LinkQueue *q) ;
