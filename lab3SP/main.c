#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

void* listener (int sig)
{
    printf("Listener\n");
    exit(0);
}
void* listener2 (int sig)
{
    printf("Listener2\n");
    exit(0);
}

int main()
{
    int pid2,pid3;
    int status=signal(SIGUSR1,listener);
    FILE* file;
    int* stat=(int*)malloc(sizeof(int));
    printf("Pid: %d \n",getpid());
    pid2=fork();
    if (pid2==0)
    {
        sleep(2); 
        int status=signal(SIG_DFL,listener2);  
        char *pythonIntrepreter="python3";
        char *calledPython="./number.py";
        char *pythonArgs[]={pythonIntrepreter,calledPython,NULL};
        execvp(pythonIntrepreter,pythonArgs);
        raise(SIGUSR1);
    }
    else{
        wait(pid2);
        pid3=fork();
        if (pid3==0)
        {
         sleep(2);
         signal(SIGUSR1,listener2); 
         raise(SIGUSR1);
        }
        else
        {
        wait(pid3); 
        }
    }
    free(stat);
    return EXIT_SUCCESS;
}