#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

typedef struct args
{
    int a;
    int b;
    int result;
}args_t;

int* square(args_t* args)
{   
    printf("Counting square...\n");
    int* res=(int*)malloc(sizeof(int));
    *res=args->a*args->b;
    return res;
}

void print(args_t* args)
{   
    printf("incoming message: %d,%d, %d\n",args->a,args->b,args->result);
}

char* serialize_int(args_t* args,char* sendBuff)
{
    for (int i = 1; i <=4; i++)
    {
        sendBuff[i-1]=args->result>>(32-8*i);
    }
    return sendBuff+4;
}

void deserialize_int(args_t* args, char* receiveBuff)
{
    for (int  i = 1; i <= 4; i++)
    {
        args->a|=receiveBuff[i-1]<<(32-8*i);
        args->b|=receiveBuff[3+i]<<(64-8*i);
    }
}


int main(int argc, char *argv[]) {
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    char* sendBuff=(char*)malloc(4*sizeof(char));
    char* receiveBuff=(char*)malloc(8*sizeof(char));
    char* ptr;
    args_t* args=(args_t*)malloc(sizeof(args_t));
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, 0, sizeof(sendBuff));
    memset(args, 0, sizeof(args));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8001);
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);
    printf("Server started working.\n");
    int i=1;
    while(i!=3) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        int n=recv(connfd,receiveBuff,8,0);
        if (n==-1)
            printf("recv error\n");
            else printf("%d bytes received from client \n", n);
        deserialize_int(args,receiveBuff);   
        print(args);                                                                                                                                                      
        int* res=square(args);
        args->result=*res;
        printf("Result: %d\n",*res);
        ptr=serialize_int(args,sendBuff);
        n=send(connfd,sendBuff,4,0);
        if (n==-1)
            printf("SEnd to client error\n");
        else printf("%d bytes sent to client \n", n);
        printf("-----------------------------\n");
        close(connfd);
        sleep(1);
        i++;
        memset(args, 0, sizeof(args_t));
        close(connfd);
    }
    free(args);
    free(receiveBuff);
    free(sendBuff);
}