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


int exercise04(){

	return 0;
  
  }

/******************************************************* 
Exercise 02

Analyze the following program and say what the value of "total_amount" will be. If there is any problem, fix it with semaphores.

********************************************************/
/* compile as "gcc -pthread -o main.c main" */
/* execute as "taskset 0x01 main" to run on a single processor */

#define NUMBER_ADDED 10000
#define NUMBER_TIMES 10000

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
        total_amount = addN (total_amount, NUMBER_ADDED);
       
    }
   
   
}

/* Main program. Runs simultaneously on two different threads.
    Wait for both to finish and look at the final summed value, 
    checking if it is the expected.
*/
     
int exercise05() {    
    
    pthread_t th1, th2;
    long expected_result;
        
    // creamos 2 hilos
    pthread_create(&th1,NULL,(void*)run, NULL);
    pthread_create(&th2,NULL,(void*)run, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    expected_result = NUMBER_ADDED * NUMBER_TIMES * 2;
   
    printf("The final result is %ld\n" ,total_amount);
    printf("We expected %ld\n" , expected_result);

        if (total_amount != expected_result)
            printf("DO NOT MATCH!!!\n");
        

} 

/********************************************************** 
Exercise 06
The following code implements an application with two threads: one prints the even numbers on the screen and the other prints the odd numbers on the screen.

Producer-consumer problem 

*******************************************************/


const int MAX_NUMBER = 200;
int shared_data = 0;

enum numbers {EVEN, ODD};

void *oddThread(void *arg) {
   int counter=1;

   while (counter <=MAX_NUMBER){
      printf ("OddThread = %d\n", shared_data++);
      counter=counter+2;

   }
   pthread_exit(0);
}

void *evenThread(void *arg) {
   int counter=2;

   while (counter <=MAX_NUMBER){
      printf ("EvenThread = %d\n", shared_data++);
      counter=counter+2;


  }
   pthread_exit(0);
}

int exercise06(){
    int i;
    pthread_t idth[2];


    pthread_create(&idth[0],NULL,oddThread,NULL);
    pthread_create(&idth[1],NULL,evenThread,NULL);
	
    // I wait for the completion of the thread
    for (i=0; i<2; i++) 
      pthread_join(idth[i],NULL);


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


void *producer(void *threadid){     
		
}

void *consumer(void *threadid){   
   
}

int exercise07(){   
   
    return 0;
    
}