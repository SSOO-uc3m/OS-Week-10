#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*******************************************************
Exercise 02

What execution secuence will create a deadlock?

********************************************************/

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* print_ssoo(void *ptr) {

    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex2);
    
    printf("SSOO");
    
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    
    pthread_exit(0);

}

void* print_new_line(void *ptr) {

    pthread_mutex_lock(&mutex2);
    pthread_mutex_lock(&mutex1);
    
    printf("\n");
    
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    
    pthread_exit(0);
}

int exercise02() {

    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, print_ssoo, NULL);    
    pthread_create(&t2, NULL, print_new_line, NULL);
    
    pthread_join(t1,NULL);    
    pthread_join(t2,NULL);
}


/*******************************************************
Exercise 04

Create a program that creates 2 ”siesta" threads, leaving 2 seconds between the first and the second. The first must always be executed before the second.

The function of “siesta” thread is to sleep 4 seconds.

********************************************************/

sem_t mutexBed;

void* siesta(void* arg)

{

    //wait
    
    sem_wait(&mutexBed);
    
     printf("\n I'm going to sleep...\n");
    
    //critical section
    
    sleep(4);
    
    //signal
    
    printf("\n I wake up.\n");
    
    sem_post(&mutexBed);
}


int exercise04(){
  
  sem_init(&mutexBed, 0, 1);

	pthread_t t1,t2;

	pthread_create(&t1,NULL,siesta,NULL);

	sleep(2);

	pthread_create(&t2,NULL,siesta,NULL);

	pthread_join(t1,NULL);

	pthread_join(t2,NULL);

	sem_destroy(&mutexBed);
	
	return 0;
  
  }

/******************************************************* 
Exercise 05

Analyze the following program and say what the value of "total_amount" will be. If there is any problem, fix it with semaphores.

********************************************************/
/* compile as "gcc -pthread -o main.c main" */
/* execute as "taskset 0x01 main" to run on a single processor */

#define NUMBER_ADDED 10000
#define NUMBER_TIMES 10000

sem_t semaphore;

/** 
* Global variable with the accumulated value where the sum is made.
* Acts as a shared variable between the two threads.
*/
long total_amount = 0;

/**
* function that returns accumulator + n. Makes the
* sum one by one with a for loop.
*/
long addN(long accumulator, int n) {
    int i;
    long total = accumulator;
    
    for (i=0;i<n;i++){
       total += 1;
       
    }
    // to force the race condition, we put to sleep the thread
    sleep(0.01);
    return total;
}

/**
* The function to be executed through a thread. Call
* NUMBER_TIMES addN function to add NUMBER_ADDED to the total_amount variable.
*/
void run() {
    int i;
    
    for (i=1;i<= NUMBER_TIMES;i++){
      //wait
		  sem_wait(&semaphore);
		
		  //critical section
      total_amount = addN (total_amount, NUMBER_ADDED);
		 
		   //signal        
		 // sem_post(&semaphore);
       
       
    }
   
   
}

/* Main program. Runs simultaneously on two different threads.
    Wait for both to finish and look at the final summed value, 
    checking if it is the expected.
*/
     
int exercise05() {    
    
    pthread_t th1, th2;
    long expected_result;

    sem_init(&semaphore, 0, 1);
        
    // create 2 threads
    pthread_create(&th1,NULL,(void*)run, NULL);
    pthread_create(&th2,NULL,(void*)run, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    expected_result = NUMBER_ADDED * NUMBER_TIMES * 2;
   
    printf("The final result is %ld\n" ,total_amount);
    printf("We expected %ld\n" , expected_result);

        if (total_amount != expected_result)
            printf("DO NOT MATCH!!!\n");
        
   sem_destroy(&semaphore);
} 

/********************************************************** 
Exercise 06
The following code implements an application with two threads: one prints the even numbers on the screen and the other prints the odd numbers on the screen.

Producer-consumer problem 

*******************************************************/


const int MAX_NUMBER = 200;
int shared_data = 0;

enum numbers {EVEN, ODD};

pthread_mutex_t mutex;
pthread_cond_t waitEven;
pthread_cond_t waitOdd;
enum numbers turn= EVEN;

void *oddThread(void *arg) {
  int counter=1;

   while (counter <=MAX_NUMBER){
	  pthread_mutex_lock (&mutex);

	  while (turn!=ODD) {
         pthread_cond_wait(&waitEven, &mutex);
	  }
	  
      printf ("OddThread = %d\n", shared_data++);
      counter=counter+2;
	  
	  turn=EVEN;
	  pthread_cond_signal(&waitOdd);
      pthread_mutex_unlock (&mutex);
   }
   pthread_exit(0);
}

void *evenThread(void *arg) {
   int counter=2;   

   while (counter <=MAX_NUMBER){
	   
	  pthread_mutex_lock (&mutex);

	  while (turn!=EVEN) {
         pthread_cond_wait(&waitOdd, &mutex);
	  }
	  
    printf ("EvenThread = %d\n", shared_data++);
    counter=counter+2;
	  
	  turn=ODD;
	  pthread_cond_signal(&waitEven);
      pthread_mutex_unlock (&mutex);

  }
   pthread_exit(0);
}

int exercise06(){
    int i;
    pthread_t idth[2];

	  pthread_mutex_init (&mutex, NULL);
    pthread_cond_init (&waitEven, NULL);
    pthread_cond_init (&waitOdd, NULL);
  
    pthread_create(&idth[0],NULL,oddThread,NULL);
    pthread_create(&idth[1],NULL,evenThread,NULL);
	
    // I wait for the completion of the thread
    for (i=0; i<2; i++) 
      pthread_join(idth[i],NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&waitEven);
    pthread_cond_destroy(&waitOdd);

    return(0);
}


/********************************************************** 
Exercise 07
The following code implements an application with two threads: one prints the even numbers on the screen and the other prints the odd numbers on the screen.

Producer-consumer problem 

*******************************************************/

// maximum items a producer can produce or a consumer can consume
#define MAX_ITEMS 10000 

 // Size of the buffer
#define BUFFER_SIZE 100
#define NUMBER_PRODUCERS 1
#define NUMBER_CONSUMERS 1

int in = 0;
int out = 0;
int buffer[BUFFER_SIZE];

pthread_cond_t non_full; /* can we add more elements? */
pthread_cond_t non_empty; /* can we remove elements? */
int n_elements; /* number of elements in buffer */

pthread_mutex_t mutex;

void *producer(void *threadid){     
  
		int pno = (intptr_t) threadid;
    int item;
    srand(time(0));
  
    for(int i = 0; i < MAX_ITEMS/NUMBER_PRODUCERS; i++) {		
      item = rand() % 100 + 1; // Produce an random item between 1 and 100		
		  pthread_mutex_lock(&mutex); /* access to buffer*/
		  while (n_elements == BUFFER_SIZE) /* when buffer is full*/{
    	  printf("\tBuffer Full\n");
    	  pthread_cond_wait(&non_full, &mutex); 		
		  }

      buffer[in] = item;
		  n_elements ++;
		
      printf("Producer %d: Insert Item %d at %d\n", pno,buffer[in],in);
      in = (in+1)%BUFFER_SIZE;
		
		  pthread_cond_signal(&non_empty); /* buffer is not empty */
		  pthread_mutex_unlock(&mutex);
    }
	
	return NULL;
}

void *consumer(void *threadid){   
  
    int cno = (intptr_t) threadid;
    int item;
  
    for(int i = 0; i < MAX_ITEMS/NUMBER_CONSUMERS; i++) {
      
		  pthread_mutex_lock(&mutex); /* access to buffer*/
      
		  while (n_elements == 0) /* when buffer is empty*/{
			  printf("\tBuffer tEmpty\n");
			  pthread_cond_wait(&non_empty, &mutex); 
		  }
      
      item = buffer[out];
      printf("Consumer %d: Remove Item %d from %d\n", cno,item, out);
      out = (out+1)%BUFFER_SIZE;
      
		  n_elements --;		
		  pthread_cond_signal(&non_full); /* buffer is not full */
      
		  pthread_mutex_unlock(&mutex);
    }
	
	return NULL;
}

int exercise07(){   
   
  pthread_t pro[NUMBER_PRODUCERS],con[NUMBER_CONSUMERS];

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&non_full, NULL);
	pthread_cond_init(&non_empty, NULL);
	
  for(int i = 0; i < NUMBER_PRODUCERS; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *) (intptr_t) i+1);
    }
  
  for(int i = 0; i < NUMBER_CONSUMERS; i++) { 
        pthread_create(&con[i], NULL, (void *)consumer, (void *) (intptr_t) i+1);
    }

  for(int i = 0; i < NUMBER_PRODUCERS; i++) {
        pthread_join(pro[i], NULL);
    }
  
  for(int i = 0; i < NUMBER_CONSUMERS; i++) {
        pthread_join(con[i], NULL);
    }

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&non_full);
	pthread_cond_destroy(&non_empty);

  return 0;
    
}