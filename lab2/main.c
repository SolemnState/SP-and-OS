#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

int SIZE;

typedef struct Args_tag {
    int** matrix;
    int size;
} Args_t;


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
                args->matrix[i][j]=rand()%10;
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

void zeroing(Args_t* M)
{
    for (int i=0;i<SIZE;i++)
    {
        for (int j=0;j<SIZE;j++)
        {
            M->matrix[i][j]=0;
        }
    }
}

void  makeEqual(Args_t* left, Args_t* right)
{
    for (int i=0;i<SIZE;i++)
    {
        for (int j=0;j<SIZE;j++)
        {
            left->matrix[i][j]=right->matrix[i][j];
        }
    }
}

void calculation(Args_t* array,pthread_t* threads,int n,Args_t* Result)
{
    for (int i=0;i<n+1;i++)
    {
        initialize(&array[i],'r');
        printMatrix(array[i]);
    }
    Args_t* temp=(Args_t*)malloc(sizeof(Args_t));
    temp->size=SIZE;
    initialize(temp,'z');
    for (int i=0;i<n;i++)
    {  
            if (i==0)
            {
                Args_t arr[3]={array[i],array[i+1],*Result};
                int status=pthread_create(&threads[i],NULL,mulMatrix,(void*) &arr);
                if (status!=0){
                    printf("Error! Can't create thread");
                    exit(EXIT_FAILURE);
                    }
                status = pthread_join(threads[i], NULL);
            } 
            else 
            {
                makeEqual(temp,Result);
                zeroing(Result);
                Args_t arr[3]={*temp,array[i+1],*Result};
                int status=pthread_create(&threads[i],NULL,mulMatrix,(void*) &arr);
                if (status!=0){
                    printf("Error! Can't create thread");
                    exit(EXIT_FAILURE);
                    }
                status = pthread_join(threads[i], NULL);
            } 
    }
    freeMemory(*temp);
    free(temp);
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
    if (n<1)
    {
        printf("Number of threads must be above zero!\n");
        exit(EXIT_FAILURE);
    }

    Args_t* array=(Args_t*)malloc((n+1)*sizeof(Args_t));
    Args_t* Result=(Args_t*)malloc(sizeof(Args_t));
    Result->size=SIZE;
    initialize(Result,'z');
    pthread_t* threads=(pthread_t*)malloc(n*sizeof(pthread_t));
    calculation(array,threads,n,Result);
    printf("\nResult: \n");
    printMatrix(*Result);

    for (int i=0;i<n;i++)
    {
        freeMemory(array[i]);
    }
    free(array);
    free(threads);
    freeMemory(*Result);
    free(Result);
    return 0;
}