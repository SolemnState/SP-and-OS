#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

int M;

void* handle_sigalrm (int sig)
{
    printf("Handled SIGALRM, begin Proc2\n");
}

int deleteLines(char* fileName)
{
    FILE* file=fopen("file.txt","r");
    int lineCounter=0;
    file=fopen("file.txt","r");
    while (! feof(file))
    {
        if (fgetc(file) == '\n')
            lineCounter++;
    }
    fclose(file);
    if (lineCounter<M)
        return -1;
    file=fopen("file.txt","r");
    FILE* temp=fopen("temp.txt","w");
    char str[256];
    for ( int i = 0; i < lineCounter-M; i++)
        {
            strcpy(str, "\0");
            fgets(str, 256, file);
            fprintf(temp, "%s", str);
        }
    fclose(file);
    fclose(temp);
    remove("file.txt"); 
    rename("temp.txt", "file.txt"); 
    return 0;

}

int main(int argc,char* argv[])
{
    if (argc==0)
    {
        printf("Not enough arguments!\n");
        exit(EXIT_FAILURE);
    }
    printf("Enter number of strings:");
    scanf("%d",&M);
    if (M<0)
    {
        printf("CAn't delete negative number of lines! \n");
        exit(EXIT_FAILURE);
    }
    
    int pid2,pid3;
    int* stat=(int*)malloc(sizeof(int));
    pid2=fork();
    if (pid2==0)
    {
        char *pythonIntrepreter="python3";
        char *calledPython="./number.py";
        char *pythonArgs[]={pythonIntrepreter,calledPython,NULL};
        execvp(pythonIntrepreter,pythonArgs);
    }
    wait(pid2);
    pid3=fork();
    signal(SIGALRM,handle_sigalrm);
    if (pid3==0)
    {
        alarm(1);
        pause();
        if (deleteLines(argv[0])==-1)
            printf("Error! Not enough lines in file.\n");
    }
    else
    {
        wait(pid3);
        printf("INIT waiting is done\n");
        printf("Reading from file:\n");
        FILE* file=fopen("file.txt","r");
        char* str[256];
        while (!feof(file))
        {
            strcpy(str, "\0");
            fgets(str, 256, file);
            printf("%s",str);
        }
        
        fclose(file);
    }
    free(stat);
    return EXIT_SUCCESS;
}