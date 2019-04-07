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


Args_t* init(Args_t* res)
{
    res->matrix=(int**)malloc(res->size*sizeof(int*));
    for (int i=0;i<res->size;i++)
    {
        res->matrix[i]=(int*)malloc(res->size*sizeof(int));
        for (int j=0;j<res->size;j++)
        {
            res->matrix[i][j]=0;
        }
    }
    return res;
}

void* initializeTest(void* args)
{
    Args_t* arg= (Args_t*) args;
    arg->size=SIZE;
    arg->matrix=(int**)malloc(arg->size*sizeof(int*));
    for (int i=0;i<arg->size;i++)
    {
        arg->matrix[i]=(int*)malloc(arg->size*sizeof(int));
        for (int j=0;j<arg->size;j++)
        {
            arg->matrix[i][j]=rand()%10;
        }
    }
    printf("\n");
    return EXIT_SUCCESS;
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

Args_t* mulMatrix(Args_t M1, Args_t M2, Args_t* result)
{
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
    return result;
}

Args_t zeroing(Args_t M)
{
    for (int i=0;i<SIZE;i++)
    {
        for (int j=0;j<SIZE;j++)
        {
            M.matrix[i][j]=0;
        }
    }
    return M;
}

Args_t* makeEqual(Args_t* left, Args_t* right)
{
    for (int i=0;i<SIZE;i++)
    {
        for (int j=0;j<SIZE;j++)
        {
            left->matrix[i][j]=right->matrix[i][j];
        }
    }
    return left;
}

Args_t* calculation(Args_t* array,pthread_t* threads,int n,Args_t* Result)
{
    for (int i=0;i<n;i++)
    {
        int status=pthread_create(&threads[i],NULL,initializeTest,(void*) &array[i]);
        if (status!=0){
        printf("Error! Can't create thread");
        exit(EXIT_FAILURE);
        }
        status = pthread_join(threads[i], NULL);
        printMatrix(array[i]);
    }
    Args_t* temp=(Args_t*)malloc(sizeof(Args_t));
    temp->size=SIZE;
    temp=init(temp);
    for (int i=0;i<n-1;i++)
    {  
            if (i==0)
                Result=mulMatrix(array[i],array[i+1],Result);  
            else 
            {
                temp=makeEqual(temp,Result);
                *Result=zeroing(*Result);
                Result=mulMatrix(*temp,array[i+1],Result);
            } 
    }
    freeMemory(*temp);
    free(temp);
    return Result;
}

int main(int argc,char* argv[])
{
    srand(time(NULL));
    int n;
    int size;
    printf("Enter size of the matrix: ");
    scanf("%d", &size);
    SIZE=size;
    printf("Enter number of threads (at least 2): ");
    scanf("%d", &n);
    if (n<2)
    {
        printf("Not enough threads!\n");
        exit(EXIT_FAILURE);
    }
    clock_t start=clock();
    Args_t* array=(Args_t*)malloc(n*sizeof(Args_t));
    Args_t* Result=(Args_t*)malloc(sizeof(Args_t));
    Result->size=SIZE;
    Result=init(Result);
    pthread_t* threads=(pthread_t*)malloc(n*sizeof(pthread_t));
    Result=calculation(array,threads,n,Result);
    printf("\nResult: \n");
    printMatrix(*Result);

    clock_t finish=clock();
    double res=(double)(finish - start) / CLOCKS_PER_SEC;
    printf("Time spent: %f \n",res);
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