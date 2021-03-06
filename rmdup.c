#include "rmdup.h"


int loadFileEntry (fileEntry *fe, FILE* fp);
int equals(fileEntry *fe1, fileEntry *fe2);
void copy (fileEntry *dst, fileEntry *src);


int main(int argc, char** argv) 
{
    int status;
    
    fileEntry fe1;
    fileEntry fe2;
    
    pid_t pid;
    FILE *fp;
    
    FILE *hl;
    char hlinksPath[2 * BUFFER_LEN];
    
    
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
    else 
    {
        wait(&status);
        
        if (status != 0) 
        {
            fprintf(stderr, "Exiting program.\n");
            exit(7);
        }
        
        
        /* Opening file with the regular files, sorted */
        if( (fp = fopen("files.txt", "r")) == NULL )
        {  
            perror("files.txt");
            fclose(fp);
            exit(4);
        }
        
        
        /* Opening file where hardlinks' information will be stored */
        sprintf(hlinksPath, "%s/%s", argv[1], "hlinks.txt");
        if ( (hl = fopen(hlinksPath, "w")) == NULL )
        {
            perror("hlinks.txt");
            fclose(fp);
            fclose(hl);
            exit(5);
        }
        
        
        
        if (loadFileEntry(&fe1, fp) == 0)           /* load fe1, pivot file */
        {
            
            while (loadFileEntry(&fe2, fp) == 0)    /* load fe2, candidate file */
            {
                
                int r;
                if ( (r = equals(&fe1, &fe2)) == -1 ) 
                {
                    fclose(fp);
                    fclose(hl);
                    fprintf(stderr, "An error occured reading files.\n");
                    exit(6);   
                }
                else if (r == 0) 
                {
                     /* if two files are equal, then an hardlink is created */
                    unlink(fe2.url);
                    link(fe1.url, fe2.url);
                    fprintf(hl, "%-30s %-8s %-25s\n", fe2.url, "--->", fe1.url);
                }
                else if (r == 1)
                {
                    /* else if no other file can be equal, copy content from candidate to pivot */
                    copy(&fe1, &fe2); 
                }
            }
        }
        
        fclose(fp);
        fclose(hl);
    }
    
    return 0;
}


int loadFileEntry (fileEntry *fe, FILE* fp) {
    
    /* Loads regular file's info into a struct, reading with from a file, with a specific format */
    
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
    
    /* Checks if two files are equal, in their name, permissions, and content */
    
    FILE *fp1, *fp2;
    int ch1, ch2;
    
    if (strcmp(fe1->name, fe2->name) || strcmp(fe1->permissions, fe2->permissions))
        return 1;
    
    
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
        return 0;
    else
        return 2;
}


void copy (fileEntry *dst, fileEntry *src) {
    
    strcpy(dst->name, src->name);
    strcpy(dst->permissions, src->permissions);
    strcpy(dst->date, src->date);
    strcpy(dst->path, src->path);
    strcpy(dst->url, src->url);
}

