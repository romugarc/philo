#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#ifndef NUM_THREADS
# define NUM_THREADS 4
#endif

typedef struct thread_data
{
   int	thread_id;
   int  sum;
   //char *message;
}       t_data;

//struct thread_data thread_data_array[NUM_THREADS];

void *PrintHello(void *threadarg)
{
   int taskid, sum;
   char *hello_msg;
   t_data *my_data;

   sleep(1);
   my_data = (t_data *) threadarg;
   taskid = my_data->thread_id;
   sum = my_data->sum;
   //hello_msg = my_data->message;
   printf("Thread %d:  Sum=%d\n", taskid, sum);
   pthread_exit(NULL);
}

void	*start_routine(void *arg)
{
	t_data *a;
	int		b;

	usleep(1000);
	a = (t_data *)arg;
	b = a->thread_id;

	//take_fork(a);
	/*eating((t_philo *)arg);
	sleeping((t_philo *)arg);
	thinking((t_philo *)arg);
	*/
	printf("%d\n", b);
}

int main(int argc, char *argv[])
{
t_data *thread_data_array;   
pthread_t threads[NUM_THREADS];
int *taskids[NUM_THREADS];
int rc, t, sum;

sum=0;
thread_data_array = malloc(sizeof(t_data) * NUM_THREADS);
/*messages[0] = "English: Hello World!";
messages[1] = "French: Bonjour, le monde!";
messages[2] = "Spanish: Hola al mundo";
messages[3] = "Klingon: Nuq neH!";
messages[4] = "German: Guten Tag, Welt!"; 
messages[5] = "Russian: Zdravstvytye, mir!";
messages[6] = "Japan: Sekai e konnichiwa!";
messages[7] = "Latin: Orbis, te saluto!";*/

for(t=0;t<NUM_THREADS;t++) {
  sum = sum + t;
  thread_data_array[t].thread_id = t;
  thread_data_array[t].sum = sum;
  //thread_data_array[t].message = messages[t];
  printf("Creating thread %d\n", t);
  rc = pthread_create(&threads[t], NULL, start_routine, (void *) 
       &thread_data_array[t]);
  if (rc) {
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
    }
  }
pthread_exit(NULL);
}