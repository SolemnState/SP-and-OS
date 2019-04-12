#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <malloc.h>
#include <semaphore.h>
int SIZE;
typedef struct Args_tag {
    int** matrix;
    int size;
} Args_t;

typedef struct Args_to_thread {
    Args_t array[3];
    int threadsCount;
    int startRow;
    int endRow;
    int thread_n;
} To_Thread_t;

void initialize(Args_t* args,char flag)
{
    args->size=SIZE;
    args->matrix=(int**)malloc(args->size*sizeof(int*));
    for (int i=0;i<args->size;i++)
    {
        args->matrix[i]=(int*)malloc(args->size*sizeof(int));
        for (int j=0;j<args->size;j++)
        {
            if (flag=='r')
                args->matrix[i][j]=rand()%3;
            else if (flag=='z')
                args->matrix[i][j]=0;
        }
    }
    printf("\n");
}

void freeMemory(Args_t M)
{
    for (int i=0;i<SIZE;i++)
        free(M.matrix[i]);
    free(M.matrix);
}

void printMatrix(Args_t M)
{
    for (int i=0;i<SIZE;i++)
    {
        for (int j=0;j<SIZE;j++)
        {
            printf("%d ",M.matrix[i][j]);
        }
        printf("\n");
    }
}

void* mulMatrix(void* args)
{
    Args_t* arg= (Args_t*) args;
    Args_t M1 = arg[0];
    Args_t M2 = arg[1];
    Args_t* result = &arg[2];
    for (int i=0;i<M1.size;i++)
    {
        for (int j=0;j<M1.size;j++)
        {
            for (int k=0;k<M1.size;k++)
            {
                result->matrix[i][j]+=M1.matrix[i][k]*M2.matrix[k][j];
            }
        }
    }
    return EXIT_SUCCESS;
}

void* multiply(void* args);
void mulThreads(To_Thread_t* arg)
{
     if (SIZE<arg->threadsCount)
        arg->threadsCount=SIZE;
    int count=SIZE/arg->threadsCount; 
    int additional = SIZE % arg->threadsCount;
    pthread_t* threads=(pthread_t*)malloc((arg->threadsCount)*sizeof(pthread_t));
    int start=0;
    arg->thread_n=0;
    sem_t* sem;
    pthread_mutex_t mutex;
    for (int i=0;i<arg->threadsCount;i++)
    {
        arg->thread_n++;
        int cnt=((i == 0) ? count + additional : count);
        arg->startRow=start;
        arg->endRow=start+cnt-1;
        start+=cnt;
        int status=pthread_create(&threads[i],NULL,multiply,(void*) arg);
        if (status!=0)
            printf("Error creating thread! \n");
       pthread_yield();
    }
    for(int i = 0; i < arg->threadsCount; i++)
        pthread_join(threads[i],NULL); 
    
    free(threads);
}

void* multiply(void* args)
{
    To_Thread_t* arg=(To_Thread_t*) args;
    Args_t M1 = arg->array[0];
    Args_t M2 = arg->array[1];
    Args_t* result = &arg->array[2];
    for(int row = arg->startRow; row <= arg->endRow; row++)
    {
        for(int col = 0; col < SIZE; col++)
        {
            for (int k=0;k<SIZE;k++)
            {
                result->matrix[row][col]+=M1.matrix[row][k]*M2.matrix[k][col];
            }
        }   
    }    
}

void calculation(Args_t* array,Args_t* Result)
{
    for (int i=0;i<2;i++)
    {
        initialize(&array[i],'r');
        printMatrix(array[i]);
    } 
            Args_t arr[3];
            arr[0]=array[0];
            arr[1]=array[1];
            arr[2]=*Result;
            mulMatrix((void*) arr);
}

void checkMatrices(Args_t* Res1,Args_t* Res2)
{
    for (int i=0;i<SIZE;i++)
        for(int j = 0; j < SIZE; j++)
        {  
            if (Res1->matrix[i][j]!=Res2->matrix[i][j])
            {
                printf("Matrices are not equal! \n");
                return;
            }
        }
    printf("Matrices are equal! \n");
}

int main(int argc,char* argv[])
{
    srand(time(NULL));
    int n;
    int size;
    printf("Enter size of the matrix: ");
    scanf("%d", &size);
    SIZE=size;
    printf("Enter number of threads (1 or more): ");
    scanf("%d", &n);
    if (n<2)
    {
        printf("Number of threads must be more than one!\n");
        exit(EXIT_FAILURE);
    }

    Args_t* array=(Args_t*)malloc((2)*sizeof(Args_t));
    Args_t* Result=(Args_t*)malloc(sizeof(Args_t));
    Result->size=SIZE;
    initialize(Result,'z');
    calculation(array,Result);
    printf("\nResult (1 thread): \n");
    printMatrix(*Result);

    Args_t* Result_MT=(Args_t*)malloc(sizeof(Args_t));
    Result_MT->size=SIZE;
    initialize(Result_MT,'z');
    To_Thread_t* args=(To_Thread_t*)malloc(sizeof(To_Thread_t));
    args->array[0]=array[0];
    args->array[1]=array[1];
    args->array[2]=*Result_MT;
    args->threadsCount=n;
    mulThreads(args);
    printf("\nResult (%d threads): \n",n);
    printMatrix(args->array[2]);

    checkMatrices(Result,Result_MT);
    for (int i=0;i<2;i++)
    {
        freeMemory(array[i]);
    }
    free(args);
    free(array);
    freeMemory(*Result);
    free(Result);
    freeMemory(*Result_MT);
    free(Result_MT);
    return 0;
}