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


Args_t initialize(Args_t args)
{
    args.size=SIZE;
    args.matrix=(int**)malloc(args.size*sizeof(int*));
    for (int i=0;i<args.size;i++)
    {
        args.matrix[i]=(int*)malloc(args.size*sizeof(int));
        for (int j=0;j<args.size;j++)
        {
            args.matrix[i][j]=rand()%10;
        }
    }
    printf("\n");
    return args;
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
        array[i]=initialize(array[i]);
        printMatrix(array[i]);
    }
    Args_t* temp=(Args_t*)malloc(sizeof(Args_t));
    temp->size=SIZE;
    temp=init(temp);
    for (int i=0;i<n-1;i++)
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
                temp=makeEqual(temp,Result);
                *Result=zeroing(*Result);
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