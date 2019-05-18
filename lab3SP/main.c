#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

int M;

void* handle_sigalrm (int sig)
{
    printf("Handled SIGALRM, waking up init process\n");
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

void numberLines()
{
    FILE* file=fopen("file.txt","r");
    FILE* temp=fopen("temp.txt","w");
    int lineCounter=1;
    while (! feof(file))
    {
        if (fgetc(file) == '\n')
            lineCounter++;
    }
    fclose(file);
    file=fopen("file.txt","r");
    char str[256];
    for ( int i = 1; i <= lineCounter-1; i++)
        {
            strcpy(str, "\0");
            fgets(str, 256, file);
            fprintf(temp, "%d. %s",i, str);
        }
    fclose(file);
    fclose(temp);
    remove("file.txt"); 
    rename("temp.txt", "file.txt"); 
}

int main(int argc,char* argv[])
{
    sigset_t block_mask;
    sigemptyset(&block_mask);
    struct sigaction SA;
    SA.sa_handler=handle_sigalrm;
    sigaction(SIGALRM,&SA,0);
    int parent=getpid();
    printf("Parent: %d\n",getpid());
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
    pid_t pid2=fork();
    if (pid2==0)
    {
        printf("Process 2(numbering lines)\n");
        numberLines();
        kill(parent,SIGALRM);
        exit(0);
        /*char *pythonIntrepreter="python3";
        char *calledPython="./number.py";
        char *pythonArgs[]={pythonIntrepreter,calledPython,NULL};
        signal(SIGALRM,handle_sigalrm);
        execvp(pythonIntrepreter,pythonArgs);*/
    }
    sigsuspend(&block_mask);
    pid_t pid3=fork();
    if (pid3==0)
    {
        printf("Process 3(Deleting lines from file)\n");
        if (deleteLines(argv[0])==-1)
        {
            printf("Error! Not enough lines in file.\n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        int status;
        waitpid(pid3,&status,0);
        if (status==0)
        {
            printf("Init process(Reading changed file)\n");
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
        else
        {
            printf("Process terminated with error\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}