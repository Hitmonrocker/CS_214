#include "find.c"
#define MAX_NUM 300

int getSubDirect(char** fileList, char * dirName,int fileCheck,int dirCheck,int arrayPos);
int addLOF(char ** list,char * file,int i);

int main(int argc, char const *argv[])
{
	//if(strcmp(argv[3],"-d"))
  
  int dirSize = getDirSize (argv[4]); //gets number of different directories
  char** dr = (char **) malloc (sizeof (char *) * dirSize); //Array of directories 
  getDir(dr,argv[4],dirSize); // function that gets the array of directories

  char ** fileList = (char**) malloc(sizeof(char*)*MAX_NUM); // List of Files
  int fileListSize = getSubDirect(fileList,*dr,0,16,0); // Size of List of Files

  int c = 0;
  while(c<fileListSize)
  {
    printf("%s\n",fileList[c]);
    c++;
  }
	return 0;
}

int checkName(char *needCheck)
{
  int size = strlen(needCheck); // get length of our string

  char * csv = (char*) malloc(sizeof(char*)*4); // make a string for the comparison
  sprintf(csv,"%s",needCheck+size-4); // Print the last file extension we want to comapre
  int comp = strcmp(csv,".csv"); // comparison

  free(csv); // free our created test string
  csv = NULL; // set it to NULL to protect ourself

  return comp;
}


int getSubDirect(char** fileList, char * dirName,int fileCheck,int dirCheck,int arrayPos)
{ 
  DIR *directory; // Create directory
    struct dirent *dp; // Create directory struct
   // printf("%s\n",dirName );
    if ((directory = opendir (dirName)) == NULL) { // See if we can open our directory
        perror ("Cannot open");
        exit (1);
    }


   while ((dp = readdir(directory)) != NULL) { // while reading the directory as long as it is not NULL continue
            //printf ("%d: %s\n", dp->d_type,dp->d_name); 
            if(dp->d_type == dirCheck && strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") !=0 ) // Check to see if the directory is not . or .. and is directory
            { 
             // system("PAUSE");
              char * new_str ;
                  if((new_str = (char*) malloc(strlen(dirName)+strlen(dp->d_name)+2)) != NULL){
                    
                      new_str[0] = '\0';   // ensures the memory is an empty string
                      strcat(new_str,dirName);// directory name
                      strcat(new_str,"\\");// forward slash for folder level
                      strcat(new_str,dp->d_name);// new Directory name
                 
                      arrayPos =  getSubDirect(fileList,new_str,fileCheck,dirCheck,arrayPos); // Recursive call to scan through DFS
                  
                      free(new_str);
                      new_str = NULL;
                  }
              }
              else if(dp->d_type == fileCheck) // See if it is a file by user
              {
                if(checkName(dp->d_name) == 0) // && validFile Check if it is a csv then check if it is a valid csv
                {
                char * new_str ;
                  if((new_str = (char*) malloc(strlen(dirName)+strlen(dp->d_name)+2)) != NULL){ //same as before append our string with dirname to get full location
                      new_str[0] = '\0';   // ensures the memory is an empty string
                      strcat(new_str,dirName);
                      strcat(new_str,"\\");// forward slash for folder level
                      strcat(new_str,dp->d_name);
                      
                      if(arrayPos != -1)
                      arrayPos = addLOF(fileList,new_str,arrayPos);
                      
                      free(new_str);
                      new_str = NULL;
                } 
                else {
                      fprintf(stdout,"malloc failed!\n");
                }
             //printf ("%d: [%s]\n", dp->d_type,new_str);
            }
      
          } 
      
        }
      
  closedir (directory); // Close our directory
      
  return arrayPos; // size of directory
}

int addLOF(char ** list,char * file,int i) // add to our total list of files
{
  if(i<MAX_NUM)//less than max size stated in parameters
  {
    //printf("[%s]\n",file );
    list[i] = (char*)malloc(sizeof(char)*strlen(file)); //Add to list of files
    strcpy(list[i],file);//copy string over
    return i+1; //increase size
      
   }
    
  return -1; //if it is negative one we are too big stop taking input
}


//C:\\MinGw\bin\argv.exe stuff hi -d D1