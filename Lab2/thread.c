#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int array[100];

void * thread_func(void *arg)
{
	int array_index=*((int*)arg);
	array[array_index]=5*array_index;
}

int main (void)
{   int l;
    int index[100];
    for(l=0; l<100; l++)
    {
        index[l]=l;
    }
    int i,j,k;
	pthread_t thread[100];
	for(i=0; i<100; i++)
	{
		pthread_create(&thread[i], NULL, thread_func, &index[i]);
	}
	for(j=0; j<100; j++)
	{
		pthread_join(thread[j], NULL);
	}
	for(k=0; k<100; k++)
	{
		printf("%d\n", array[k]);
	}
	return 0;
}