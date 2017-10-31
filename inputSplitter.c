#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>

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

int 
getInputSize (const char *input) 
{
  
 
int count = 1;
  
 
while (*input)
    
 
    {
      
 
if ((int) ',' == (int) *input)
    
count++;
      
 
input++;
    
} 
 
return count;

}

int findInt(char* comp,char* cat[], int size)
 {
    
    int counter = 0;

    while(counter < size )
    {
        if(comp != NULL)
            if(strcmp(comp,cat[counter]) == 0)
              return counter;
        counter++;
     }
    return 2;
 }

void getInput(int* input, const char *directory,char *cat[],int size,int i)
{

char * newDir = appendNull(directory);


 
char *temp;
temp = strtok (newDir, ",");
 char * buffer;
 
int counter = 0;
 
while (temp != NULL && counter < i)
    {
      
       input[counter] = findInt(temp,cat,size);
        temp = strtok (NULL, ",");
        counter++;
    }  

}
 
int 
main (int argc, char const *argv[]) 
{
  
char*cat[] = {"color","director_num","num_critic_for_reviews","duration","director_facebook_likes","actor_3_facebook_likes","actor_2_name","actor_1_facebook_likes"\
    ,"gross","genres","actor_1_name","movie_title","num_voted_users","cast_total_facebook_likes","actor_3_name","facenumber_in_poster","plot_keywords","movie_imdb_link"\
    "num_user_for_reviews","language","country","content_rating","budget","title_year","actor_2_facebook_likes","imdb_score","aspect_ratio","movie_facebook_likes"}; 

if(argv[2] != NULL)
{
int i = getInputSize (argv[2]);

int *compareArr = (int*)malloc(sizeof(int)*i); 
getInput(compareArr,argv[2],cat,27,i);
 int counter = 0;
while(counter<i)
{
    printf("%d: ",compareArr[counter]);
    counter++;
}

}
return 0;

 
}
/*
 Ok so the code doesn't need a main to run it just need what is inside the main to run it returns a comparison array so all you have to do is put this section
 into you main code and it should take argv[2] and sort according to that input.
 You also dont need the while loop
 */
