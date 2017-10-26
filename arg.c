#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "sorter.c"

//Global vars
int process_count = 0;

//Function Definition
char* get_oDir(char* output);
char* get_cwd(char* var);
int csv_line_count(FILE *fp);
int validate_csv(FILE *fp);
void print(const char *name, const char *output);

//Functions

char* get_oDir(char* output){

}

char* get_cwd(char *var) {
	//get working dir
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		perror("getcwd() error");
	else
		//	printf("Current working directory3 is: %s\n", cwd);
		strcpy(var, cwd);

	return var;
}

int validate_csv(FILE *fp) {

	int count; // this will act as an index counter for each row
	char *line = NULL;								// pointer for each line
	size_t len = 0;
	ssize_t read;

	//this while loop will break each line into tokens
	read = getline(&line, &len, fp);

	//printf("Retrieved line of length %zu\n", read);
	//printf("Entered string is %s \n", line);
	char *p;
	count = 0;
	while ((p = strsep(&line, ",")) != NULL) {
		count++;
	}
	//printf("value of count is %d\n",count);

	if (read == 419 && count == 28) {	//vaild csv
		return 1; //vaild
	} else {
		return 0; //invalid
	}
}

int csv_line_count(FILE *fp) {

	char *line = NULL;								// pointer for each line
	size_t len = 0;
	ssize_t read;
	int ctotal = 1;						// count the total number of tokens

	//this while loop will break each line into tokens
	while ((read = getline(&line, &len, fp)) != -1) {
		char *p;
		while ((p = strsep(&line, ",")) != NULL) {
		}
		ctotal++;
	}
	//printf("Ctotal = %d\n", ctotal);

	return ctotal;
}

void print(const char * name, const char *output) {

	//Print the input
	//printf("Inputs: %s and %s\n", name, output);

	DIR *dir;
	FILE *fp;
	int counter;
	int valid; // 0 = no 1 = yes
	struct dirent *entry;
	char * wDir = (char *) malloc(sizeof(char) * 20); //save a copy of the working dir
	pid_t child_pid; // for directory

	//int status;

	if (!(dir = opendir(name)))
		return;

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type == DT_DIR) {

			if (strcmp(entry->d_name, ".") == 0
					|| strcmp(entry->d_name, "..") == 0) {
				continue;
			}

			//copied from argv.c
			/*char * new_str;
			 new_str = (char*) malloc(
			 strlen(entry->d_name) + strlen(entry->d_name) + 2);

			 new_str[0] = '\0';   // ensures the memory is an empty string
			 strcat(new_str, name);   // directory name
			 strcat(new_str, "\\");   // forward slash for folder level
			 strcat(new_str, entry->d_name);   // new Directory name
			 printf("dir name :%s\n", new_str);
			 printf("hello\n");*/

			/*	//get current working directory
			 char cwd[256];
			 if (getcwd(cwd, sizeof(cwd)) == NULL)
			 perror("getcwd() error");
			 else
			 printf("current working directory2 is: %s\n", cwd);

			 */

			// found a directory; so fork
			child_pid = fork();

			if (child_pid >= 0) /* fork succeeded */
			{
				if (child_pid == 0) /* fork() returns 0 for the child process */
				{
					//printf("child process!\n");
					process_count++;
					//printf("child PID =  %d, parent pid = %d\n", getpid(),getppid());
					//	 printf("child PID =  %d, parent pid = %d\n", getpid(), getppid());
					printf("[%s]\n", entry->d_name);
					strcpy(wDir, entry->d_name);
					print(entry->d_name, output);    // recursive call
					_exit(0);
				} else /* parent process */
				{
					//printf("parent process!\n");
					//	 printf("parent PID =  %d, child pid = %d\n", getpid(), child_pid);
					//	wait(&status); /* wait for child to exit, and store child's exit status */
					//exit(0);  /* parent exits */
				}
			} else {
				printf("error creating process\n");
				exit(0);
			}

		} else { //Found a File
			pid_t child2_pid;					// for csv
			char *point = entry->d_name;
			if ((point = strrchr(point, '.')) != NULL) {  	//Check if the file is CSV
				if (strcmp(point, ".csv") == 0) {
					//ends with csv
					printf("%s\n", entry->d_name);

					//change the working directory
					chdir(name);

					char* curr_dir = (char*) malloc(sizeof(char) * 100);
					curr_dir = get_cwd(curr_dir);

					printf("Current directory: %s\n", curr_dir);

					child2_pid = fork();

					if (child2_pid >= 0) /* fork succeeded */
					{
						if (child2_pid == 0) /* fork() returns 0 for the child process */
						{
							//printf("child process!\n");
							process_count++;

							//printf("child PID =  %d, parent pid = %d\n", getpid(),getppid());
							//printf("child PID =  %d, parent pid = %d\n", getpid(), getppid());

							//opening file for checking if valid or invalid.
							fp = fopen(entry->d_name, "r");
							valid = validate_csv(fp);
							// Close the file
							fclose(fp);
							if (valid) {
								//printf("The file is valid\n");

								//opening file for getting the total lines
								fp = fopen(entry->d_name, "r");
								if (fp == NULL) {
									printf("Could not open file %s\n",
											entry->d_name);
									exit(0);
								}
								//printf("The output name is %s\n",output);
								counter = csv_line_count(fp);
								printf("The file %s has %d lines\n ",
										entry->d_name, counter);
								// Close the file
								fclose(fp);

								//open file for sorting
								fp = fopen(entry->d_name, "r");
								if (fp == NULL) {
									printf(
											"Could not open file %s for sorting.\n",
											entry->d_name);
									exit(0);
								}
								file_sort("duration", fp, output,entry->d_name);

								_exit(0);

							} else {
								printf("The file is invalid\n");
								exit(0);
							}

						} else /* parent process */
						{
							//printf("parent process!\n");
							//	 printf("parent PID =  %d, child pid = %d\n", getpid(), child_pid);
							//wait(&status); /* wait for child to exit, and store child's exit status */
							//exit(0);  /* parent exits */
						}
					} else {
						printf("error creating process\n");
						exit(0);
					}

				}
			}

		}
	}    //end while
	pid_t pid;
	while ((pid = waitpid(-1, NULL, 0))) {
		if (errno == ECHILD) {
			break;
		}
	}
	closedir(dir);

}

int main(int argc, char* argv[]) {

	/*printf("number of args = %d\n", argc);
	 int i = 0;
	 for (i; i < argc; i++) {
	 printf("argc[%d] = %s\n", i, argv[i]);
	 }*/

	if (argc <= 2) {
		printf("Error. Not enough arguments provided.\n");
		exit(1);
	}
// getting the "-c" field
	char * sF = (char *) malloc(sizeof(char) * 25); //sF = sortingField. The arg after "-c"
	strcpy(sF, argv[2]);
	//printf("sF = %s\n", sF);

// checking for "-d"
	char * wDir = (char *) malloc(sizeof(char) * 20);
	char * oDir = (char *) malloc(sizeof(char) * 20);
	strcpy(wDir, ".");
	strcpy(oDir, ".");
	//printf("The wDir name is %s\n",wDir);
	//printf("The oDir name is %s\n",oDir);
	//DIR *wDir = opendir("."); 	// workingDirectory
	//DIR *oDir = opendir(".");   // outputDirectory

	if (argc > 4) {
		if (!(strcmp("-d", argv[3]))) { 	// working directory was given
			//wDir = opendir(argv[4]);

			// opening the directory given
			strcpy(wDir, argv[4]);
			//printf("opened %s\n", argv[4]);
		} else if (!(strcmp("-o", argv[3]))) {
			//oDir = opendir(argv[4]);
			// saving the directory given
			strcpy(oDir, argv[4]);
			//printf("saved %s\n", argv[4]);
		}
	}
// checking for "-o"
	if (argc > 5) {
		if (!(strcmp("-o", argv[5]))) {
			//oDir = opendir(argv[6]);
			// opening the directory given
			strcpy(oDir, argv[6]);
			//printf("saved 2 %s\n", argv[6]);
		}
	}

	/*
	 // manually check the csv_line_count function
	 FILE *fp;
	 int counter;
	 char *file_name = "Dir2/mData.csv";
	 int valid;  // 0 = no 1 = yes;
	 fp = fopen(file_name, "r");
	 if (fp == NULL) {
	 printf("Could not open file \n");
	 exit(0);
	 }
	 //printf("The output name is %s\n",output);
	 counter = csv_line_count(fp);
	 // Close the file
	 fclose(fp);
	 fp = fopen(file_name, "r");
	 valid = validate_csv(fp);
	 printf("The file has %d lines\n ", counter);
	 if(valid){
	 printf("The file is valid\n");
	 }else {
	 printf("The file is invalid\n");
	 }
	 // Close the file
	 fclose(fp);
	 */
	printf("Given directory: %s\n",wDir);
	printf("Ouput directory: %s\n",oDir);

	print(wDir, oDir);

	return 0;

}
