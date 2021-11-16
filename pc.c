#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#define maxSize 10
int buffer[maxSize];
int count = 0;
pthread_cond_t full, empty;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
void put(int data){
 buffer[count] = data;
 count = count + 1;
}
int get(){
 count = count - 1;
 return buffer[count];
}
void *producer(void *arg){
 int loops = (int)*(int *)arg;
 for(int i = 0; i < loops; i++){ 
   pthread_mutex_lock(&m);
 while (count == maxSize){
 printf("The Buffer is full, waiting for the consumer to consume\n");
 pthread_cond_wait(&empty, &m);
 }
 put(i);
 printf("Placed: %d\n", i);
 pthread_cond_signal(&full);
 pthread_mutex_unlock(&m);
 }
}
void *consumer(void *arg){
 int id = (int)*(int *)arg;
 printf("Thread %d entred\n", id);
 for (int i=0; i<2; i++){
 pthread_mutex_lock(&m);
   int temp;
 while (count == 0){
 printf("Waiting for producer to fill\n");
 pthread_cond_wait(&full, &m);
 }
 temp = get();
 printf("Thread %d Removed: %d\n", id, temp);
 pthread_cond_signal(&empty);
 pthread_mutex_unlock(&m);
 }
}



int main(){
 pthread_t p, *t;
 pthread_cond_init(&full, NULL);
 pthread_cond_init(&empty, NULL);
 int m, c;
 printf("Enter the m value (number of items to produce): ");
 scanf("%d", &m);
 printf("Enter the number of consumers (C): ");
 scanf("%d", &c);
 printf("Buffer Size: %d\n", maxSize);
 printf("NOTE: EACH WORKER CAN CONSUME ATMOST 2 ITEMS \n");
 t = (pthread_t *)malloc(sizeof(pthread_t)*c);
 pthread_create(&p, NULL, producer, &m);
 int arr[c];
 for (int i=0; i<c; i++){
 arr[i] = i;
 pthread_create(&t[i], NULL, consumer, &arr[i]);
 }
 for (int j=0; j<c; j++){
 pthread_join(t[j], NULL);
 }
 free(t);
 return 0;
}
