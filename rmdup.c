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


#define BUFFER_LEN        256
#define PERMS_LEN         11
#define DATE_LEN          20
#define FILE_LINE_LEN     (2*BUFFER_LEN + PERMS_LEN + DATE_LEN)   


typedef struct {
    
    char name[BUFFER_LEN];
    char permissions[PERMS_LEN];
    char date[DATE_LEN];
    char path[BUFFER_LEN];
    char url[2 * BUFFER_LEN];
} fileEntry;




int loadFileEntry (fileEntry *fe, FILE* fp);
int equals(fileEntry *fe1, fileEntry *fe2);
void copy (fileEntry *dst, fileEntry *src);


int main(int argc, char** argv) 
{
    
    pid_t pid;
    FILE *fp;
    
    
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s dir_name\n", argv[0]);
        exit(1);
    }
    
    
    
    pid = fork();
    
    if (pid < 0) 
    {
        perror("Fork failed");
        exit(2);
    }
    else if (pid == 0) 
    {
        execl("lssort", "lssort", argv[1], NULL);
        perror(argv[1]);
        exit(3);
    }
    else {
        wait(NULL);
        
        if( (fp = fopen("files.txt", "r")) == NULL )
        {  
            perror("files.txt");
            fclose(fp);
            exit(4);
        }
        
        
        fileEntry *fe1 = (fileEntry *) malloc(sizeof(fileEntry));
        fileEntry *fe2 = (fileEntry *) malloc(sizeof(fileEntry));
        
        
        if (loadFileEntry(fe1, fp) == 0) 
        {
            
            while (loadFileEntry(fe2, fp) == 0) 
            {
                
                int r;
                if ( (r = equals(fe1, fe2)) == 1 ) 
                {
                    unlink(fe2->url);
                    link(fe1->url, fe2->url);
                }
                else if (r == 0) 
                {
                    copy(fe1, fe2);
                }
                else
                {
                    free(fe1);
                    free(fe2);
                    fclose(fp);
                    exit(5);   
                }
            }
        }
        
        free(fe1);
        free(fe2);
        fclose(fp);
    }
    
    return 0;
}


int loadFileEntry (fileEntry *fe, FILE* fp) {
    
    char buffer[FILE_LINE_LEN];
    char *token;
    
    
    if (fgets(buffer, FILE_LINE_LEN, fp) == NULL)
        return -1;
    
    
    token = strtok(buffer, "/");
    strcpy(fe->name, token);
    
    token = strtok(NULL, " ");
    strcpy(fe->permissions, token);
    
    token = strtok(NULL, " ");
    strcpy(fe->date, token);
    
    token = strtok(NULL, "\n");
    strcpy(fe->path, token);
    
    sprintf(fe->url, "%s/%s", fe->path, fe->name);
    
    return 0;
}



int equals(fileEntry *fe1, fileEntry *fe2) {
    
    FILE *fp1, *fp2;
    int ch1, ch2;
    
    if (strcmp(fe1->name, fe2->name) || strcmp(fe1->permissions, fe2->permissions))
        return 0;
    
    
    if( (fp1 = fopen(fe1->url, "r")) == NULL )
    {  
        perror(fe1->url);
        fclose(fp1);
        return -1;
    }
    
    
    if( (fp2 = fopen(fe2->url, "r")) == NULL )
    {  
        perror(fe2->url);
        fclose(fp1);
        fclose(fp2);
        return -1;
    }
    
    
    do 
    {
        ch1 = getc(fp1);
        ch2 = getc(fp2);
    }  while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2));
    
    
    if (ch1 == ch2)
        return 1;
    else
        return 0;
}


void copy (fileEntry *dst, fileEntry *src) {
    
    strcpy(dst->name, src->name);
    strcpy(dst->permissions, src->permissions);
    strcpy(dst->date, src->date);
    strcpy(dst->path, src->path);
    strcpy(dst->url, src->url);
}