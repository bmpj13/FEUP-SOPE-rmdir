#ifndef RMDUP_H
#define RMDUP_H


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


int loadFileEntry(fileEntry *fe, FILE* fp);

int equals(fileEntry *fe1, fileEntry *fe2);

void copy(fileEntry *dst, fileEntry *src);

#endif
