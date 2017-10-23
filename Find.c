#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include "sorter.h"
int 
getDirSize (const char *directory) 
{
  
 
int count = 1;
  
 
while (*directory)
    
 
    {
      
 
if ((int) '/' == (int) *directory)
	
count++;
      
 
directory++;
    
} 
 
return count;

 
}
int getSize(const char* s)
{   
    int count = 0;
    while(*s)
    {
    count++;
    s++;
    }
    return count;
}

 
char* appendNull(const char * dir)
{
    int size =getSize(dir);
char* ret = (char*)malloc(1+sizeof(char) * (size));
    memset(ret,'\0',size+1);
    strncpy(ret,dir,size);
return ret;
}

void
getDir (char** dir,const char *directory,int i) 
{
  
char * newDir = appendNull(directory);


 
char *temp;
temp = strtok (newDir, "/\\");
 char * buffer;
 
int counter = 1;
int size;  
 
while (temp != NULL && counter < i)
    
    {
size = getSize(temp);
*dir = (char*)malloc(sizeof(char)*size);
temp = strtok (NULL, "/");
strcpy(*dir,temp);
dir++;
counter++;
    
}
}


 
 
 
/*int 
main (int argc, char const *argv[]) 
{
  
 
 
char *test = "hello/this/is/a/test/for/Directory";
int i = getDirSize (test);
char** dir = (char **) malloc (sizeof (char *) * i);
getDir(dir,test,i);
//printf("%s",dir[0]);//,*(stuff+1),*(stuff+2));
  
return 0;

 
}*/


