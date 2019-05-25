#include <iostream>
#include <string>
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
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8001);
    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
     if(inet_pton(AF_INET, argv[2], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }
    int fd_from;
    fd_from=open(argv[1],O_RDONLY);
    int size=fileSize(argv[1]);
    char* senderBuff= new  char(1024);
    int nread;
    int counter=1024;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    while (size > 0)
    {
        nread = read(fd_from, senderBuff, 1024);
        size-=1024;
        if(send(sockfd,senderBuff,1024,0)==-1)
            cout<<"ERRor while sending !!"<<endl;

    }
    if (counter!=0)
    {
        if(send(sockfd,senderBuff,counter,0)==-1)
                cout<<"ERRor while sending !!"<<endl;
            counter==0;
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
    cout <<"Time spent:"<< duration<<endl;
    delete [] senderBuff;
    close(connfd);
}