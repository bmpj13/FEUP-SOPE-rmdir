#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>


#define READ 0
#define WRITE 1


int main(int argc, char** argv) {
    
    int pipefd[2];
    int finalfd;
    pid_t pid;
    pid_t subpid;
    
    
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s dir_name\n", argv[0]);
        exit(1);
    }
    
    
    if ( pipe(pipefd) < 0 ) 
    {
        perror("Pipe failed");
        exit(2);
    }
    
    
    if ( ( finalfd = open("files.txt", O_WRONLY | O_CREAT | O_TRUNC, 
        S_IRUSR | S_IWUSR | S_IXUSR) ) == -1 ) 
    {
        perror("files.txt");
        close(finalfd);
        exit(3);
    }
    
    
    
    pid = fork();
    
    if (pid < 0) 
    {
        
        perror("Fork failed");
        close(finalfd);
        exit(4);
    }
    else if (pid == 0) 
    {
        close(pipefd[READ]);
        
        if( dup2(pipefd[WRITE], STDOUT_FILENO) < 0)
        {
            perror("dup error");
            exit(5);
        }
        
        execl("lsdir", "lsdir", argv[1], NULL);
        perror(argv[1]);
        close(finalfd);
        exit(6);
    } 
    else 
    {
        
        close(pipefd[WRITE]);
        
        subpid = fork();
        
        if (subpid < 0) 
        {
            perror("Fork failed");
            close(finalfd);
            exit(7);
        }
        else if (subpid == 0) 
        {
            if(dup2(pipefd[READ], STDIN_FILENO) < 0)
            {
                perror("dup error");
                exit(8);
            }
            
            if( dup2(finalfd, STDOUT_FILENO) < 0)
            {
                perror("dup error");
                exit(9);
            }
            
            execlp("sort", "sort", "-V", NULL);
            perror("sort failed");
            close(finalfd);
            exit(10);
        }
        else 
        {
            waitpid(subpid, NULL, 0);
        }
    }
    
    close(finalfd);
    exit(0);
}