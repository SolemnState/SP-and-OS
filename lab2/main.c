#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <malloc.h>

int** initialize(int** matrix,int size)
{
    matrix=(int**)malloc(size*sizeof(int*));
    for (int i=0;i<size;i++)
    {
        matrix[i]=(int*)malloc(size*sizeof(int));
        for (int j=0;j<size;j++)
        {
            matrix[i][j]=rand()%10;
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return matrix;
}

void freeMemory(int** matrix,int size)
{
    for (int i=0;i<size;i++)
        free(matrix[i]);
    free(matrix);
}

int** mulMatrix(int** M1, int** M2,int** result,int size)
{
    for (int i=0;i<size;i++)
    {
        for (int j=0;j<size;j++)
        {
            result[i][j]=0;
            for (int k=0;k<size;k++)
            {
                result[i][j]+=M1[i][k]*M2[k][j];
            }
        }
    }
    return result;
}

void printMatrix(int** M,int size)
{
    for (int i=0;i<size;i++)
    {
        for (int j=0;j<size;j++)
        {
            printf("%d ",M[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    srand(time(NULL));
    int size;
    int **matrix1;
    int **matrix2;
    int **result;
    printf("Enter the size of the matrix:");
    scanf("%d",&size);
    matrix1=initialize(matrix1,size);
    matrix2=initialize(matrix2,size);
    result=initialize(result,size);
    mulMatrix(matrix1,matrix2,result,size);
    printMatrix(result,size);
    freeMemory(matrix1,size);
    freeMemory(matrix2,size);
    freeMemory(result,size);
    return 0;
}