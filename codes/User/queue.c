#include"queue.h"

void en_Queue(LinkQueue *q, QElemType e)  
{  
    QueuePtr temp = (QueuePtr)malloc(sizeof(QNode));  
    if(temp)  
    {  
        temp->data = e;  
        temp->next = NULL;  
        q->rear->next = temp;  
        q->rear = temp;  
    }  
}  
  
  
 
void init_Queue(LinkQueue *q)  
{  
        q->front = q->rear = (QNode *)malloc(sizeof(QNode));  
        q->front->next = NULL;  
}  
  
void de_Queue(LinkQueue *q, QElemType *e)  
{  
		QueuePtr temp;
    if(q->front == q->rear)  
            return;  
    temp = q->front->next;  
    if(q->front->next == q->rear)  
            q->rear = q->front;  
    *e = temp->data;  
    q->front->next = temp->next;  
    free(temp);  
}  
  

int is_Empty(LinkQueue *q)  
{  
    if(q->front == q->rear)  
            return 1;  
    return 0;  
}  
  

int getlength_Queue(LinkQueue *q)  
{  
    QueuePtr temp = q->front;  
    int i = 0;  
    while(temp != q->rear)  
    {  
        ++i;  
        temp = temp->next;  
    }  
    return i;  
}  
  
 
void clear(LinkQueue *q)  
{  
    QueuePtr temp = q->front->next;  
    while(temp)  
    {  
        QueuePtr tp = temp;  
        temp = temp->next;  
        free(tp);  
    }  
    temp = q->front;  
    q->front = q->rear = NULL;  
    free(temp);  
}  
  
 
void print_Queue(LinkQueue *q)  
{  
				QueuePtr temp;
        if(q->front == q->rear)  
                return;  
        temp = q->front->next;  
        while(temp != q->rear)  
        {  
            printf("%c ", temp->data);  
            temp = temp->next;  
        }  
        printf("%c", temp->data);  
        printf("\n");  
}  
  
  

void top_Queue(LinkQueue *q, QElemType *e)  
{  
    if(q->front == q->rear)  
            return;  
    *e = q->front->next->data;  
}  
  
