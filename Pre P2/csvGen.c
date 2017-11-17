#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "arg.c"

char* createChar(int charCount)
{
	if(charCount==0)
		return NULL;

	char* c = (char *) malloc(sizeof(char)*charCount);
	for(int i = 0;i<charCount-1;i++)
	{
		c[i] = (char) (rand()%(122-97))+97;
	}
	c[charCount-1] = '\0';
 return c;
}
float createFloat()
{
	return rand()+1.1;
}
int createInt()
{
	return (int)  rand()%(122-97)+97;
}	
void createCSV(char* dirName, int numberOfRows)
{
	
	char * filename = malloc(strlen(dirName)+9);
	strcpy(filename,dirName);
	strcat(filename,"/");
	strcat(filename,createChar(4));
	strcat(filename,".csv");
	FILE *fp;
	fp = fopen(filename,"wb");
	for (int i = 0; i < numberOfRows; ++i)
		{
				fprintf(fp, "%s,",createChar(17));
				fprintf(fp, "%s,",createChar(27));
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%s,",createChar(27));
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%s,",createChar(45));
				fprintf(fp, "%s,",createChar(28));
				fprintf(fp, "%s,",createChar(219));
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%s,",createChar(28));
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%s,",createChar(76));
				fprintf(fp, "%s,",createChar(250));
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%s,",createChar(14));
				fprintf(fp, "%s,",createChar(19));
				fprintf(fp, "%s,",createChar(7));
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%d,",createInt());
				fprintf(fp, "%f,",createFloat());
				fprintf(fp, "%f,",createFloat());
				fprintf(fp, "%d\n",createInt());	

	}
	if(fp!=NULL)
	{
		fclose(fp);
	}
	fp = NULL;


}
char * createDir(char* dirName, int avgNumofDir)
{

if(strlen(dirName)<=26)
{
	DIR *dir;
    struct dirent *dent;
    char* Newdir = dirName;
	dir = opendir(dirName);
	for (int i = 0; i <avgNumofDir ; ++i)
	{
	int changeDir =  (int) rand()%30 + 1;
	
	if(changeDir % 2 == 0)
	{
		int avgNumofRows = (int) rand() %5444 +1;
		createCSV(Newdir,avgNumofRows);
	}

	if (changeDir == 1)
	{
		
		 Newdir = malloc(strlen(dirName)+2);
		if (Newdir != NULL)
			{	
			close(dir);
			strcpy(Newdir,dirName);
			strcat(Newdir,"/");
			strcat(Newdir,createChar(4));
 			int succ = mkdir(Newdir, 0700);
 			if(succ != -1)
 				{
 					dir = opendir(Newdir);
 			}
 		}
 		else
 		{
 			exit(01);
 		}
	}
	if (changeDir == 15)
	{
		char * Newdir = malloc(strlen(dirName)+2);
		if (Newdir != NULL)
			{	
			close(dir);
			strcpy(Newdir,dirName);
			strcat(Newdir,"/");
			strcat(Newdir,createChar(4));
 			return Newdir;
 		}

	}

	}
close(dir);
return Newdir;
}
return dirName;
}


int main(int argc, char const *argv[])
{
	clock_t start, end;
	double times;

	srand(time(NULL));

int* counter = 0;

//Makes the Directories
struct stat st;
	char* newDir = argv[1]; 
if(stat(newDir,&st) != 0)
{
	int succ = mkdir(newDir, 0700);
	if(succ == -1)
		exit(01);
}


for (int i = 0; i < atoi(argv[2])/10; ++i)
	{	
		char* nextDir = createDir(newDir,atoi(argv[2]));
		if(nextDir != NULL)
			{
			newDir = malloc(strlen(nextDir));
			strcpy(newDir,nextDir);
			int succ = mkdir(newDir, 0700);
		}
		else
			{
				exit(01);
				}
}
end = clock() - start;
times = ((double)end)/CLOCKS_PER_SEC;
char * args[] = {"-c","-c","colors","-d",argv[1],"-o","Test"};
start = clock();
//mainProject(6,args);
end = clock() - start;
times = ((double)end)/CLOCKS_PER_SEC;
printf("This took %f seconds to execute\n",times);
	return 0;
}
