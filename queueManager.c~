#include <stdio.h>
#define MAX 256

typedef struct Queues{
    char data[MAX];
    int front;
    int rear;
}Queue;

Queue queue;

void CreatQueue(){
    queue.front = queue.rear = 0;
}
inline void PushQueue(char value){
    if((queue.rear+1)%MAX!=queue.front) {
        queue.rear = (queue.rear+1)%MAX;
        queue.data[queue.rear] = value;
    } else {
        //队列已满
        printf("PushQueue Is Full\n");
    }
}
inline char PopQueue(){
    if(queue.front!=queue.rear) {
        int value = queue.data[queue.front];
        queue.front = (queue.front+1)%MAX;
        return value;
    } else {
        //队列为空
        printf("PopQueue Is Empty\n");
    }
    return -1;
}
void testQueue() {
    int i;
    for(i = 1 ; i<=250 ; i++) {
        PushQueue(i);
    }	
    for(i = 1 ; i<=MAX ; i++){
        printf("%d ",PopQueue());
    }
}
