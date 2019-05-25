#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fstream>
#include <chrono>
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
    int sockfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
     char* ptr;
    time_t begin,end;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8001);
    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(sockfd,10);
    int out_fd;
    out_fd=open("New_Test.txt",O_CREAT|O_RDWR,0777);
    int size=fileSize("test.txt");
    char* receiverBuff= new char(1024);
    cout<<"server onliune: "<<endl; 
    
    int counter;
    connfd = accept(sockfd, (struct sockaddr*)NULL, NULL);
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    time (&begin);
    while (fileSize("New_Test.txt")<=size)
    {      
        counter=recv(connfd, receiverBuff, 1024,0);
        char* out_ptr=receiverBuff;
        ssize_t nwritten;
        do
        {
            nwritten = write(out_fd, out_ptr, counter);
            if (nwritten >= 0)
            {
                counter -= nwritten;
                out_ptr += nwritten;
            }
        } while (counter>0);
    }
    time (&end);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
    cout <<"Time spent:"<< duration<<endl;
    delete [] receiverBuff;
    close(connfd);
}