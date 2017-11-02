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
#include "is.c"

//Global vars

//Function Definition
int check_oDir(char* output); // 0 = Exists 1 = Does not exist 3 = Couldn't create
char* get_cwd(char* var);
int csv_line_count(FILE *fp);
int validate_csv(FILE *fp);
int print(const char *name, const char *output, char *sF, int* compare,
		int compare_size);

//Functions

int check_oDir(char* output) {
	DIR* dir = opendir(output);
	if (dir) {
		/* Directory exists. */
		closedir(dir);
		return 0;
	} else if (ENOENT == errno) {
		/* Directory does not exist. */
		mkdir(output, 0000700);
		return 1;
	} else {
		/* opendir() failed for some other reason. */
		printf("Couldn't create new Directory at location : %s\n.", output);
		return 3;
	}

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

	char *line = NULL;					// pointer for each line
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

int print(const char * name, const char *output, char *sF, int* compare,
		int compare_size) {

	//Print the input
	//printf("Inputs: %s and %s\n", name, output);


	DIR *dir;
	FILE *fp;
	int counter = 0;  //csv line counter
	int process_count = 0;
	int valid; // 0 = no 1 = yes
	struct dirent *dir_entry;
	char * wDir = (char *) malloc(sizeof(char) * 20); //save a copy of the working dir
	pid_t child_pid; // for directory
	int status;   // used to get the return value from function.

	if (!(dir = opendir(name)))
		return 24;

	while ((dir_entry = readdir(dir)) != NULL) {
		if (dir_entry->d_type == DT_DIR) {

			if (strcmp(dir_entry->d_name, ".") == 0
					|| strcmp(dir_entry->d_name, "..") == 0) {
				continue;
			}

/*			//get current working directory
			char cwd[256];
			if (getcwd(cwd, sizeof(cwd)) == NULL)
				perror("getcwd() error");
			else
				printf("current working directory2 is: %s\n", cwd);*/

			// found a directory; so fork
			child_pid = fork();

			if (child_pid >= 0) /* fork succeeded */
			{
				if (child_pid == 0) /* fork() returns 0 for the child process */
				{
					//printf("child process!\n");
				//	printf("1 Child PID =  %d, Parent pid = %d\n", getpid(), getppid());

					//printf("[%s]\n", dir_entry->d_name);
					strcpy(wDir, dir_entry->d_name);
					process_count++;
					chdir(name);
					//printf("wDir : %s, oDir : %s \n", wDir, output);
					int temp = print(wDir, output, sF, compare, compare_size); // recursive call
					//	printf("temp is :%d\n",temp);
					process_count += temp;
					//printf("func returned %d,%d\n",print(dir_entry->d_name, output),process_count);
					printf("%d, ", getpid());
					fflush(stdout);
					_exit(process_count);
				} else {/* parent process */
					//printf("parent process!\n");
					//printf("1 parent PID =  %d, child pid = %d\n", getpid(),child_pid);
				}
			} else {
				printf("\n");
				printf("error creating process!\n");
				exit(process_count);
			}

		} else { //Found a File
			pid_t child2_pid;					// for csv
			char *point = dir_entry->d_name;
			if ((point = strrchr(point, '.')) != NULL) { //Check if the file is CSV
				if (strcmp(point, ".csv") == 0) {
					//ends with csv
					//printf("%s\n", dir_entry->d_name);

					//change the working directory
					chdir(name);
					//fflush(stdout);
					child2_pid = fork();
					//printf("2 wDir : %s, oDir : %s \n",wDir,output);

					if (child2_pid >= 0) { /* fork succeeded */
						if (child2_pid == 0) { /* fork() returns 0 for the child process */
							//printf("child process!\n");
							process_count++;
							/*printf("Child PID =  %d, Parent pid = %d ; filename: %s : process_count %d\n",
							 getpid(), getppid(),dir_entry->d_name,process_count);*/
							//printf("Child PID =  %d, Parent pid = %d\n", getpid(), getppid());
							printf("%d, ", getpid());
							//opening file for checking if valid or invalid.
							fp = fopen(dir_entry->d_name, "r");
							valid = validate_csv(fp);
							// Close the file
							fclose(fp);
							if (valid) {
								//printf("The file is valid\n");
								//opening file for getting the total lines
								fp = fopen(dir_entry->d_name, "r");
								if (fp == NULL) {
									printf("\n");
									printf("Could not open file! %s\n",
											dir_entry->d_name);
									exit(process_count);
								}
								//printf("The output name is %s\n",output);
								counter = csv_line_count(fp);
								//printf("The file %s has %d lines\n ",dir_entry->d_name, counter);
								// Close the file
								fclose(fp);
								//open file for sorting
								fp = fopen(dir_entry->d_name, "r");
								if (fp == NULL) {
									printf("\n");
									printf(
											"Could not open file %s for sorting.\n",
											dir_entry->d_name);
									exit(process_count);
								}
								//printf("output being sent: %s\n",output);
								file_sort(sF, fp, output,dir_entry->d_name,compare,compare_size);

							} else {
								//printf("The file %s is invalid\n",dir_entry->d_name);
								exit(process_count);
							}
							fflush(stdout);
							_exit(process_count);
						} else /* parent process */
						{
						}
					} else {
						printf("\n");
						printf("error creating process!\n");
						exit(process_count);
					}
				}
			}
		}
	}    //end while

	pid_t pid;  // wait for all created in this pass
	while ((pid = waitpid(-1, &status, 0)) && pid != -1) {
		//printf("waited on : %d\n",pid);
		//printf("exit stat : %d : %d  \n",pid,WEXITSTATUS(status));
		process_count += WEXITSTATUS(status);
		//printf("Pcount = %d : ",process_count);
		if (errno == ECHILD) {
			break;
		}
	}
	closedir(dir);
	//Free all the malloc calls
	free(wDir);
	//printf("END process_counter = %d\n", process_count);
	return process_count;
}

int main(int argc, char* argv[]) {

	/*printf("number of args = %d\n", argc);
	 int i = 0;
	 for (i; i < argc; i++) {
	 printf("argc[%d] = %s\n", i, argv[i]);
	 }*/

	int process_counter = 0;
	int if_dir = -1;

	if (argc <= 2) {
		printf("Error. Not enough arguments provided.\n");
		exit(1);
	}
//--------
	char*cat[] = { "color", "director_name", "num_critic_for_reviews",
			"duration", "director_facebook_likes", "actor_3_facebook_likes",
			"actor_2_name", "actor_1_facebook_likes"\
, "gross", "genres",
			"actor_1_name", "movie_title", "num_voted_users",
			"cast_total_facebook_likes", "actor_3_name", "facenumber_in_poster",
			"plot_keywords", "movie_imdb_link"
					"num_user_for_reviews", "language", "country",
			"content_rating", "budget", "title_year", "actor_2_facebook_likes",
			"imdb_score", "aspect_ratio", "movie_facebook_likes" };

	int compare_size = getInputSize(argv[2]);
	int *compareArr = (int*) malloc(sizeof(int) * compare_size);
	getInput(compareArr, argv[2], cat, 27, compare_size);
//--------
	//getting the "-c" field
	char * sF = (char *) malloc(sizeof(char) * 25); //sF = sortingField. The arg after "-c"
	strcpy(sF, argv[2]);
	//printf("sF = %s\n", sF);

// checking for "-d"
	char * wDir = (char *) malloc(sizeof(char) * 40);
	char * oDir = (char *) malloc(sizeof(char) * 40);
	char* fullPath = (char*) malloc(sizeof(char) * 40);
	strcpy(wDir, ".");
	strcpy(oDir, ".");
	char* curr_dir = (char*) malloc(sizeof(char) * 100);
	curr_dir = get_cwd(curr_dir);

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
		} else if (!(strcmp("-o", argv[3]))) { //case 3 where only output DIR is given
			//oDir = opendir(argv[4]);
			// saving the directory given
			const char * first_ptr = argv[4];
			//printf("ptr in: %c\n", first_ptr[0]);
			if (first_ptr[0] == '/') {			//absolute path
				//printf("%s and %c are equal.\n", first_ptr, slash);
				strcpy(oDir, argv[4]);
				if_dir = check_oDir(oDir);
			} else {
				strcat(curr_dir, "/");
				strcat(curr_dir, argv[4]);
				strcpy(oDir, curr_dir);
				if_dir = check_oDir(oDir);
			}
			//printf("saved %s\n", argv[4]);
		}
	}
// checking for "-o"
	if (argc > 5) {
		if (!(strcmp("-o", argv[5]))) {
			const char * first_ptr = argv[6];
			//printf("ptr in: %c\n", first_ptr[0]);
			if (first_ptr[0] == '/') {							//absolute path
				//printf("%s and %c are equal.\n", first_ptr, slash);
				strcpy(oDir, argv[6]);
				if_dir = check_oDir(oDir);
			} else {
				strcat(curr_dir, "/");
				strcat(curr_dir, argv[6]);
				strcpy(oDir, curr_dir);
				if_dir = check_oDir(oDir);
			}
			//printf("saved 2 %s\n", argv[6]);
		}
	}

	//printf("Current directory: %s\n", curr_dir);

	/*if (!(strcmp(oDir, "."))) {
	 strcpy(oDir, curr_dir);
	 }else{

	 strcat(curr_dir, "/");
	 strcat(curr_dir, oDir);
	 strcpy(oDir, curr_dir);
	 //printf("oDir in cmp : %s\n", oDir);
	 }*/

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

	//printf("Curr_dir directory: %s\n", curr_dir);
	//printf("Given directory: %s\n", wDir);
	//printf("Ouput directory: %s\n", oDir);
	int initial_pid = getpid();
	printf("Initial PID: %d\n", initial_pid);
	printf("PIDS of all child processes: ");

	fflush(stdout);
	process_counter = print(wDir, oDir, sF, compareArr, compare_size);
	printf("\n");
	process_counter++;
	printf("Total number of Processes: %d\n", process_counter);

	//free all the malloc calls
	//free(sF);
	free(wDir);
	free(oDir);
	free(fullPath);
	free(curr_dir);

	return 0;

}
