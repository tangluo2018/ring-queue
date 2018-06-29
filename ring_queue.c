#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define DATA_SIZE 1024

typedef struct{
   int  front;
   int  rear;
   int data[DATA_SIZE];
}ring_queue;

static int ERROR_EMPTY = 0;

void init_queue(ring_queue* queue)
{  
   queue->front = 0;
   queue->rear = 0;
   memset(queue->data, 0, sizeof(queue->data));
}

int is_empty(ring_queue* queue)
{
  if(queue->front == queue->rear)
     return 1;
  else
     return 0;
}

int is_full(ring_queue* queue)
{
  if(((queue->rear + 1)%DATA_SIZE) == queue->front)
     return 1;
  else
     return 0;
}

int en_queue(ring_queue* queue, int element)
{
  if(is_full(queue)){
     printf("queue is full!\n");
     return -1;
  }
  
  queue->data[queue->rear] = element;
  queue->rear = (queue->rear+1)%DATA_SIZE;
  //printf("rear: %d ", queue->rear);
  return 0;
}

int ex_queue(ring_queue *queue)
{
  int element;
  if(is_empty(queue)){
     printf("queue is empty!\n");
     ERROR_EMPTY = 1;
     return -1;
  }
  element = queue->data[queue->front];
  queue->front = (queue->front+1)%DATA_SIZE;
  return element;
}


void* write_data(void* arg)
{
   int i = -100;
   int rc = 0;
   ring_queue *queue= (ring_queue*)arg;
   while(1){
      rc = en_queue(queue, i);
      if(rc){
         printf("queue is full now, waiting...\n");
         usleep(1000*5);
      }else{
         i++;
      }
   }
  return arg;
}

void* read_data(void* arg)
{
   int element;
   ring_queue *queue = (ring_queue*)arg;
   while(1){
     element = ex_queue(queue);
     if((element == -1) && (ERROR_EMPTY == 1)){ 
         printf("\n");
         usleep(1000*5);
     }else{
        printf("%d ", element);                                                                                                                                                    }     
   }
   return arg;
}

void main()
{
  int i = -100;
  int element;
  int rc;
  void *ret;
  ring_queue queue;
  init_queue(&queue);
  printf("main test\n");
  pthread_t write_tid;
  rc = pthread_create(&write_tid, NULL, write_data, (void*)&queue);
  if(rc != 0){
     printf("thread create failed\n");
     exit(-1);
  }
  pthread_t read_tid;
  rc = pthread_create(&read_tid, NULL, read_data, (void *)&queue);
  if(rc != 0){
     printf("thread create failed\n");
     exit(-1);
  }

  pthread_join(write_tid, &ret);
  pthread_join(read_tid, &ret);
  /*
  while(i<1000){
   en_queue(&queue, i);
   i++;
  };

  while(1){
    element = ex_queue(&queue);
    if((element == -1) && (ERROR_EMPTY == 1)){
       printf("\n");
       break;
    }else{
      printf("%d ", element);
    }
  }
  */
 printf("main test end\n");
}
