#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h> 
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
    pid_t cpid;
    char buf;
    char* buffer=new char(SIZE);
    int size = fileSize(argv[1]);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int fifo=mkfifo("fifo",S_IRWXU);
    if (fifo==-1)
        exit(EXIT_FAILURE);
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        int out_fd=open("New_Test.txt",O_CREAT|O_RDWR,0777);
        int fifo_fd=open("fifo",O_RDONLY);
        int counter;
        int counter_ass=0;
        while (counter=read(fifo_fd, buffer, SIZE), counter > 0)
        {
                char* out_ptr=buffer;
                int nwritten;
                counter_ass+=counter;
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
                if (counter_ass==6488666) exit(EXIT_SUCCESS);
                } while (counter>0);

        }
        cout<<"1"<<endl;
        close(fifo_fd);
        close(out_fd);
        exit(EXIT_SUCCESS);

    } else {        
        int fd=open(argv[1],O_RDONLY);
        int fifo_fd=open("fifo",O_WRONLY);
        while (size>0)
        {
            if (size>1024)
            {
                size-=SIZE;
                int nread = read(fd, buffer, SIZE);
                write(fifo_fd, buffer, SIZE);
            }
            else 
            {
                int nread = read(fd, buffer, size);
                write(fifo_fd, buffer, size);
                size-=size;
            }
            
        }
        cout<<"Size after send"<<size<<endl;
        close(fd);
        wait(NULL);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        close(fifo_fd);
        auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
        cout <<"Time spent for sending message using pipe:"<< duration<<endl;
        delete[] buffer;
        remove("fifo");
        exit(EXIT_SUCCESS);
    }
}