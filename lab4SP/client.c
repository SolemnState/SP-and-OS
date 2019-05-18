#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

typedef struct args
{
    int a;
    int b;
    int result;
}args_t;

char* serialize_int(args_t* args, char* sendBuff)
{
    for (int  i = 1; i <= 4; i++)
    {
        sendBuff[i-1]=args->a>>(32-8*i);
        sendBuff[3+i]=args->b>>(64-8*i);
    }
    return sendBuff+8;
}

void deserialize_int(args_t* args, char* receiveBuff)
{
    //for (int i = 1; i < 4; i++)
    //{
     //   args->result|=receiveBuff[i-1]<<(32-8*i);
   // }
    args->result|=receiveBuff[0]<<24;
   args->result|=receiveBuff[1]<<16;
   args->result|=receiveBuff[2]<<8;
   args->result|=receiveBuff[3];
}

void print(args_t* args)
{   
    printf("My: %d, %d, %d \n",args->a,args->b,args->result);
}
int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }
    int sockfd = 0, n = 0;
    args_t* args=(args_t*)malloc(sizeof(args_t));
    char* sendBuff=(char*)malloc(8*sizeof(char));
    char* receiveBuff=(char*)malloc(4*sizeof(char));
    char* ptr;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, 0, sizeof(sendBuff));
    memset(receiveBuff, 0, sizeof(receiveBuff));
    memset(args, 0, sizeof(args));
    printf("Enter a: ");
    scanf("%d",&args->a);
    printf("Enter b: ");
    scanf("%d",&args->b);
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8001);
    print(args);
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }
    ptr=serialize_int(args,sendBuff);
    n=send(sockfd,sendBuff,8,0);
    if (n==-1)
            printf("SEnd to client error\n");
            else printf("%d bytes sent to server \n", n);
    n = recv(sockfd, receiveBuff, 4,0);
    if (n==-1)
            printf("recv error\n");
            else printf("%d bytes received from server \n", n);
    deserialize_int(args,receiveBuff);
      
   printf("Result: %s\n",receiveBuff);
    printf("Result: %d\n",args->result);
    if(n==-1)
    {
        printf("\n Read error \n");
    }
    free(args);
    free(receiveBuff);
    free(sendBuff);
    return 0;
}