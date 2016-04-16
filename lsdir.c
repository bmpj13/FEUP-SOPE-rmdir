#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h> 
#include <unistd.h>


#define BUFFER_LEN        256

int main (int argc, char** argv) {
    
    DIR *dirp;
    struct dirent *direntp;
    struct stat stat_buf;
    char url[BUFFER_LEN];
    pid_t pid;
    
    
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s dir_name\n", argv[0]);
        exit(1);
    }
    
    
    
    if ( (dirp = opendir(argv[1])) == NULL ) 
    {
        perror(argv[1]);
        exit(2);
    }
    
    
    while ( (direntp = readdir(dirp)) != NULL ) 
    {
        
        sprintf(url, "%s/%s", argv[1], direntp->d_name);
        
        /* read file information */
        if (lstat(url, &stat_buf) < 0) 
        {
            perror(direntp->d_name);
            closedir(dirp);
            exit(3);
        }
        
        
        /* if regular, write its info */
        if (S_ISREG(stat_buf.st_mode)) 
        {
            
            fprintf(stdout, "%s/%d %d %s\n",
                    direntp->d_name,
                    (int) stat_buf.st_mode,
                    (int) stat_buf.st_mtime,
                    argv[1]
            );
            
        }
        /* if directory, instantiate a new 'lsdir' */
        else if (S_ISDIR(stat_buf.st_mode) && 
            strcmp(direntp->d_name, ".") && 
            strcmp(direntp->d_name, ".."))
        {
            pid = fork();
            
            
            if (pid < 0) 
            {
                perror("Fork failed");
                exit(4);
            }
            else if (pid == 0) 
            {
                execl("lsdir", "lsdir", url, NULL);
                perror(url);
                closedir(dirp);
                exit(5);
            }
        }
    }
    
    
    closedir(dirp);
    return 0;
}

