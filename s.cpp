#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <semaphore.h>
#include <string.h>
#include <ctime>
#include <cerrno>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <climits>
 


using namespace std;

#define NUM_THREADS 5
#define BUFFER_SIZE 5


//Global variables
int sleep_time;  //sleep time
int num_prod;    // number of producers
int num_cons;    //number of consumers
int buffer[BUFFER_SIZE];
int in = 0;
int count = 0;
int id;
int koke = 0;



// semaphores declared
sem_t full, empty;
pthread_mutex_t mutex;

void *producer(void*);
void *consumer(void*);
void insert_item();
void remove_item();
/* the thread runs in this function */
//void *runner(void *param); 





int main (int argc, char*argv[])
{

    cout << "---Main Thread Running----"<< endl;
    int rc;

    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        cout<<"Enter 3 arguments in order: sleep time, num of producers, num of consumers. \n";
        return -1;

    }

    // if three integers are passed put into the variables
    sleep_time = atoi(argv[1]);
    num_prod = atoi(argv[2]);
    num_cons = atoi(argv[3]);
    //cout << num_prod;


    // declare the semaphores needed
    srand(time(NULL));
	pthread_t tid[NUM_THREADS]; 	/* the thread identifier */
    //pthread_attr_t attr;/* set of attributes for the thread */

	/* get the default attributes */
	//pthread_attr_init(&attr);
    
    
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, NUM_THREADS);
    sem_init(&full, 0, 0);
    //int i;

    //create a producer thread
    pthread_t producers[num_prod];
    for ( int i = 0; i < num_prod; i++)
    {
       //pthread_create(&tid[i],&attr,runner,(void *) &i); 
       pthread_create(&producers[i],NULL, producer,NULL);
        //pthread_attr_t p_attr; 		/* set of attributes for the thread */
        /* get the default attributes */
        //pthread_attr_init(&p_attr);
        //printf("Created child %d with thread id = %lu\n", i, tid[i]);
        unsigned long int uid = producers[i];
         koke = uid & INT_MAX;
        //cout << koke << "asdasdasdas";
    }


    //create a consumer thread
    pthread_t consumers[num_cons];
    for( int i = 0; i < num_cons; i++)
    {
       //pthread_create(&tid[i],&attr,runner,(void *) &i); 
       pthread_create(&consumers[i],NULL, consumer,NULL);
       //printf("Created child %d with thread id = %lu\n", i, tid[i]);

        //pthread_attr_t c_attr; 		/* set of attributes for the thread */
        
        /* get the default attributes */
        //pthread_attr_init(&c_attr);
        unsigned long int uid = consumers[i];
        koke = uid & INT_MAX;
    }
    //int xy = num_cons + num_prod;
    

    usleep(1);


}


void *producer(void *param){
    
    
    // cout << "asd";
    while (1)
    {
            int a = (rand() % 4 + 1);
            usleep(a);       
            
            // enter the critical section
            sem_wait(&empty);
            pthread_mutex_lock(&mutex);
            //int x = *((int*)(&param));
            //insert
            cout<< "Producer Thread "<< koke <<" sleeps for "<< a <<" second" << endl;
            insert_item();
            // exit the critical section
            pthread_mutex_unlock(&mutex);
            sem_post(&full);


    }


   
   //return 0;

}
void *consumer(void *param){

   //cout << "xxxxasd";
   
    
    // cout << "asd";
    while (1)
    {
            int a = (rand() % 4 + 1);
            usleep(a);            // enter the critical section
            sem_wait(&empty);
            pthread_mutex_lock(&mutex);
            //int x = *(int *) (param);
            //remove
            cout<< "Consumer Thread "<< koke<<" sleeps for "<< a <<" second" << endl;

            remove_item();
            // exit the critical section
            pthread_mutex_unlock(&mutex);
            sem_post(&full);


    }

}
void insert_item(){
    
    
        if( count != BUFFER_SIZE){
        int x = rand() % 100;
        
        buffer[in] = x;
        in++;
        //printf("Producer %d: Insert Item %d at %d\n", *((int *)pno),buffer[in],in);
        cout << "Producer produced an item at thread: "<< koke <<" with value: "<<x << endl;
        cout << "Insert_item inserted item: "<<x<<" at position " << count<<endl;
        int loop;
        for(loop = 0; loop < 5; loop++)
        {
            printf("%d ", buffer[loop]);
        }
        cout << endl;
        count++;
        }
}
void remove_item(){

    if( count != 0){
    int y; 
    y = buffer[in - 1];
    if (y != 0){
    in--;
    count--;
    cout << "Consumer consumes an item at thread: "<< koke <<" with value: "<<y << endl;
    cout << "Remove_item removed item: "<<y<<" at position " << count<<endl;
    buffer[count] = 0;
    int loop;

    for(loop = 0; loop < 5; loop++)
      {
          printf("%d ", buffer[loop]);
      }
    cout << endl;
    }
    else {
        cout << "Error" << endl;
    }

    }
}

// void *runner(void *tid) 
// {
// 	int i;
// 	i= *((int *) tid);
// 	//printf("Thread ID %d doing some work\n", i);

// 	/* do some work ... */

// 	pthread_exit(0);
    
// }
