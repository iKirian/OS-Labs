#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NTHREADS 100
#define MUL 5
#define COLUMNS 2
#define COLS 5

enum columns
{
        Index, Shmid
};

void * thread_func(void *arg)
{
        int* shm;
        int array_index=*((int*)arg);
        int shmid = *((int*)arg+1);
        shm =(int*) shmat(shmid, NULL, 0);
        if (shm ==(int*) -1)
        {
                perror("shm");
                exit (1);
        }
        shm[array_index]=MUL*array_index;
        printf("Thread #%d: work completed\n", array_index);
        return NULL;
}

int main (void)
{
        int* shm;
        int l;
        int index[NTHREADS][COLUMNS]; //2 columns for index and shmid
        int key = 1; //First SHMK
        int size = sizeof(int)*NTHREADS;
        int i,j,k;
        int shmid;
        pthread_t thread[NTHREADS];
        shmid = shmget(key, size, IPC_CREAT | 0666);
        if(shmid<0)
        {
                perror("shmget");
                exit(1);
        }
        for(l=0; l<NTHREADS; l++)
        {
                index[l][Index]=l;
                index[l][Shmid]=shmid;
        }

        for(i=0; i<NTHREADS; i++)
        {
                pthread_create(&thread[i], NULL, thread_func, &index[i]);
                printf("Thread #%d created\n", i);
        }
        for(j=0; j<NTHREADS; j++)
        {
                pthread_join(thread[j], NULL);
                printf ("Thread #%d: terminated\n", j);
        }
        shm = (int*)shmat(shmid, NULL, 0);
        if(shm == (int*)-1)
        {
                perror("shm");
                exit(1);
        }
        printf("Results:\n");
        for(k=0; k<NTHREADS; k++)
        {
                        printf("%d by %d is %d\n", MUL , k, shm[k]);
        }
        shmctl (shmid, IPC_RMID, 0);
        return 0;
}
