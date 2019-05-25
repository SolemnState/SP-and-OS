#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <chrono>

#define SIZE 4096 // PIPE_BUF == 4096
using namespace std;
using namespace std::chrono;

int fileSize(const char *add){
    ifstream mySource;
    mySource.open(add, ios_base::binary);
    mySource.seekg(0,ios_base::end);
    int size = mySource.tellg();
    mySource.close();
    return size;
}
int main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t cpid;
    char buf;
    char* buffer=new char(SIZE);
    int size = fileSize(argv[1]);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        close(pipefd[1]);
        int out_fd=open("New_Test.txt",O_CREAT|O_RDWR,0777);
        int counter;
        while (counter=read(pipefd[0], buffer, SIZE), counter > 0)
            {
                char* out_ptr=buffer;
                int nwritten;
                if (counter>1024)
                    nwritten=write(out_fd, out_ptr, SIZE);
                else nwritten=write(out_fd, out_ptr, counter);
                do
                {
                    if (nwritten >= 0)
                    {
                        counter -= nwritten;
                        out_ptr += nwritten;
                    }
                } while (counter>0);

            }

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS);

    } else {
        close(pipefd[0]);         
        int fd=open(argv[1],O_RDONLY);
        while (size>0)
        {
            if (size>1024){
                size-=SIZE;
                int nread = read(fd, buffer, SIZE);
                write(pipefd[1], buffer, SIZE);
            }
            else {
                int nread = read(fd, buffer, size);
                write(pipefd[1], buffer, size);
                size-=size;

            }
        }
        
        close(pipefd[1]);
        wait(NULL);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
        cout <<"Time spent for sending message using pipe:"<< duration<<endl;
        delete[] buffer;
        exit(EXIT_SUCCESS);
    }
}