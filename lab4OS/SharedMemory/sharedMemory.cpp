#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <chrono>
#include <fstream>
using namespace std::chrono;
using namespace std;
int fileSize(const char *add){
    ifstream mySource;
    mySource.open(add, ios_base::binary);
    mySource.seekg(0,ios_base::end);
    int size = mySource.tellg();
    mySource.close();
    return size;
}
#define BUF_SIZE 4096
int main(int argc, char* argv[]){
    if(argc<2){
        cout<<"Reboot with file name";
        exit(0);
    }
    int shmid;
    key_t key;
    char *shm;
    key=2221;
    shmid=shmget(key, 7000000, IPC_CREAT | 0777);
    if(shmid<0){
        perror("Error:\n");
        exit(1);
    }
     high_resolution_clock::time_point t1 = high_resolution_clock::now();
    int pid;
    pid=fork();
    if(pid==0){
        int newText;
        shm=(char*)shmat(shmid, NULL, 0);
        int size=fileSize(argv[1]);
        newText=open("new.txt",O_CREAT|O_RDWR,0777);
        int i=size/BUF_SIZE;
        int mod=size%BUF_SIZE;
       
        for(int j=0; j<i;j++){
            write(newText, shm, BUF_SIZE);
            shm=shm+BUF_SIZE;
        }
        char *s;
        for(s=shm;*s!=0;s++){
            write(newText, s, 1);
        }
        *s='*';
        close(newText);
        exit(0);
    }
    else{
        int text;
        int nread;
        char str[BUF_SIZE];
        text=open(argv[1],O_RDONLY);
        shm=(char*)shmat(shmid, NULL, 0);
        while(nread=read(text,str, BUF_SIZE),nread>0){
           memcpy(shm, str, BUF_SIZE);
           shm=shm+BUF_SIZE;
        }
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
        cout <<"Time spent:"<< duration<<endl; 
        *shm=0;
        while(*shm!='*'){
           sleep(1);
        }
        close(text);
    }
    return 0;
}