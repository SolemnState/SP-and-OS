#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#define BUFFER_SIZE 4096

int main(int argc, char *argv[])
{
    if (argc != 4 )
    {
        printf("Usage: \n");
        return EXIT_FAILURE;
    }
    int fd_from;
    int out_fd;
    char buffer[BUFFER_SIZE];
    int permission;
    ssize_t nread;
    errno=0;
    fd_from=open(argv[1],O_RDONLY);
    if (fd_from<0)
        perror("Problem in opening the file");
    size_t size=strlen(argv[3]);
    if (size==3)
        permission=S_IRWXU;
    else if (size==1)
    {
        if (*argv[3]=='r') permission=S_IRUSR;
        else if (*argv[3]=='w') permission=S_IWUSR;
        else if (*argv[3]=='x') permission=S_IRUSR;
    }
    out_fd=open(argv[2],O_CREAT|O_RDWR,permission);
    if (out_fd<0)
        perror("Problem in opening the file");
    while (nread = read(fd_from, buffer, sizeof buffer), nread > 0)
    {
        char* out_ptr=buffer;
        ssize_t nwritten;
        do
        {
            nwritten = write(out_fd, out_ptr, nread);
            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
        } while (nread>0);
        
    }
    close(fd_from);
    close(out_fd);
    pid_t pid = fork();
    if (pid==0)
    {
        printf("ls command after copy: \n"); 
        char *path="/bin/ls";
        char *parameter="-l";
        char *Args[]={path,parameter,NULL};
        execvp(path,Args);
    }
    wait(&pid);

    return 0;
}