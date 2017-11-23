#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "threads.h"
#include <pthread.h>
#define NUM_THREADS	502

void print_usage();
void print_stat(struct file_dir *str);
char* get_cwd(char* var);
void print_cwd(char* var);
int check_oDir(char* output); // 0 = Exists 1 = Does not exist 3 = Couldn't create
int check_wDir(char* output); // 0 = Exists 1 = Does not exist
void *parse_dir(void *st);
void *parse_dir2(void *st);
pthread_t pthread_self(void);
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
		void *(*start)(void *), void *arg);

void *parse_dir2(void *st) {
	void *ans;
	pthread_t self_id;
	self_id = pthread_self();
	printf("\nID = %ld\n", self_id);
	struct file_dir *str;
	str = (struct file_dir *) st;
	print_stat(str);
	DIR *dir;
	struct dirent *dir_entry;

	int tc = 0, i = 0; //thread check & loop count int i
	pthread_t *threads;   // Array of threads
	threads = malloc(sizeof(pthread_t) * NUM_THREADS);
	char * wDir = (char *) malloc(sizeof(char) * 20); //new working dir
	printf("--------\n");
	printf("wDir = %s\n", str->wDir);
	printf("--------\n");

	if (!(dir = opendir(str->wDir)))
		return 24;
	while ((dir_entry = readdir(dir)) != NULL) {
		if (dir_entry->d_type == DT_DIR) { //skipping the '.' & '..'
			if (strcmp(dir_entry->d_name, ".") == 0
					|| strcmp(dir_entry->d_name, "..") == 0) {
				continue;
			}
			print_cwd(str->wDir);
			printf("[%s]\n", dir_entry->d_name);
			struct file_dir str1;
			str1 = *str;
			char* curr_dir = (char*) malloc(sizeof(char) * 100); //current working directory
			curr_dir = get_cwd(curr_dir);
			strcpy(wDir, curr_dir);
			//strcat(wDir, "/");
			//strcat(wDir, dir_entry->d_name);
			strcpy(wDir, dir_entry->d_name);
			str1.wDir = wDir;
			print_stat(&str1);

			//printf("i = %d\n", i);
			//tc = pthread_create(&threads[i], NULL, parse_dir, (void *)&str1);
			//i++;
		}   //if found a directory
		else {
			printf("CSV!\n");
			char *point = dir_entry->d_name;
			if ((point = strrchr(point, '.')) != NULL) { //Check if the file is CSV
				if (strcmp(point, ".csv") == 0) {
					printf("%s\n", dir_entry->d_name);
				}
			}
		}
		if (i == 501) { //if threads are reaching the limit
			threads = realloc(threads, sizeof(pthread_t) * (NUM_THREADS * 2));
			i = NUM_THREADS * 2;
		}

	} // end while
	  //pthread_join(&threads[0],&ans);
	  //free(wDir);
	printf("--------------------end\n");
	return 0;
}
void *PrintHello(void *threadid) {
	long tid;
	tid = (long) threadid;
	printf("Hello World! It's me, thread #%ld!\n", tid);
	fflush(stdout);
	pthread_exit(NULL);
}

void *parse_dir(void *st) {

	void *ans;
	pthread_t self_id;
	self_id = pthread_self();
	printf("\nID = %ld\n", self_id);

	struct file_dir *str;
	str = (struct file_dir *) st;
	//print_stat(str);
	DIR *dir;
	struct dirent *dir_entry;

	int tc = 0, i = 0; //thread check & loop count int i
	pthread_t *threads;   // Array of threads
	threads = malloc(sizeof(pthread_t) * NUM_THREADS);

	printf("--------\n");
	printf("wDir = %s\n", str->wDir);
	printf("--------\n");
	//get current working directory
	 char cwd[256];
	 if (getcwd(cwd, sizeof(cwd)) == NULL)
	 perror("getcwd() error");
	 else
	 printf("current working directory2 is: %s\n", cwd);

	if (!(dir = opendir(str->wDir)))
		return (void*) 24;

	while ((dir_entry = readdir(dir)) != NULL) {
		if (dir_entry->d_type == DT_DIR) { //if found a directory
			if (strcmp(dir_entry->d_name, ".") == 0
					|| strcmp(dir_entry->d_name, "..") == 0) {
				continue;
			}

			printf("[%s]\n", dir_entry->d_name);
			struct file_dir str1;
			str1 = *str;
			char * wDir = (char *) malloc(sizeof(char) * 20); //new working dir
			//char* curr_dir = (char*) malloc(sizeof(char) * 100); //current working directory
			//curr_dir = get_cwd(curr_dir);
			//strcpy(wDir, curr_dir);
			strcat(wDir, str->wDir);
			strcat(wDir, "/");
			strcat(wDir, dir_entry->d_name);
			//strcpy(wDir, dir_entry->d_name);
			str1.wDir = wDir;
			printf("STAT 1: \n");
			//print_stat(&str1);
			chdir(str1.wDir);
			tc = pthread_create(&threads[i], NULL, parse_dir, (void *) &str1);
			if (tc) {
				printf("ERROR; return code from pthread_create() is %d\n", tc);
				exit(-1);
			}
			i++;
			//free(wDir);
		} else {
			char *point = dir_entry->d_name;
			if ((point = strrchr(point, '.')) != NULL) { //Check if the file is CSV
				if (strcmp(point, ".csv") == 0) {
					printf("%s\n", dir_entry->d_name);
				}
			}
		}
		if (i == 501) { //if threads are reaching the limit
			threads = realloc(threads, sizeof(pthread_t) * (NUM_THREADS * 2));
			i = NUM_THREADS * 2;
		}

	} // end while
	while ((i - 1) >= 0) {
		i--;
		pthread_join(threads[i], (void *) &ans);
		printf("Return value %ld\n", (long)ans);

	}
	//free(wDir);
	printf("--------------------end1\n");
	return 0;
}
int check_wDir(char* input) {
	DIR* dir = opendir(input);
	if (dir) {
		/* Directory exists. */
		closedir(dir);
		return 0;
	} else if (ENOENT == errno) {
		/* Directory does not exist. */
		return 1;
	}
	return 0;
}
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
void print_cwd(char* var) {
	printf("Current directory : %s\n", get_cwd(var));
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
void print_stat(struct file_dir *str) {

	printf("------ The sF is %s\n", str->sF);
	printf("The wDir name is %s\n", str->wDir);
	printf("The oDir name is %s\n", str->oDir);
}
void print_usage() {
	fprintf(stderr, "Usage : ./sorter -c movie_title -d thisdir -o thatdir\n");
	fprintf(stderr, "Usage : ./sorter -d thisdir -o thatdir -c movie_title\n");
	fprintf(stderr, "Usage : ./sorter -d thisdir -c movie_title -o thatdir\n");
}

int main(int argc, char* argv[]) {
//Check Arguments!
	if (argc <= 2) {
		print_usage();
		exit(0);
	}
//getopt method to get the input
	int option = 0;
	char * sF = (char *) malloc(sizeof(char) * 25); //sF = sortingField. The arg after "-c"
	char * wDir = (char *) malloc(sizeof(char) * 40); //wDir arg after -d
	char * oDir = (char *) malloc(sizeof(char) * 40); //oDir arg after -o
	//char* fullPath = (char*) malloc(sizeof(char) * 40);
	char* curr_dir = (char*) malloc(sizeof(char) * 100); //current working directory
	strcpy(wDir, ".");
	strcpy(oDir, ".");
	curr_dir = get_cwd(curr_dir);

	while ((option = getopt(argc, argv, "-c:-d:-o:")) != -1) {
		switch (option) {
		case 'c':
			strcpy(sF, optarg);
			break;
		case 'd':
			strcpy(wDir, optarg);
			break;
		case 'o':
			strcpy(oDir, optarg);
			break;
		default:
			print_usage();
			exit(0);
		} //end switch
	} //end while
	  //check if sorting field was provided
	if (sF[0] == '\0') {
		print_usage();
		exit(0);
	}
	//check if the Input Directory exists
	if (check_wDir(wDir)) {
		printf("Given input directory doesn't exist. Exiting!\n");
		exit(0);
	}
	//check if the Ouput Directory exists *if it does then continue else create a new one(if couldnt create a new one then exit)
	if ((check_oDir(oDir)) == 3) {
		exit(0);
	}
	// created a struct since when creating pthreads we cannot pass multiple parameters
	struct file_dir dir_struct;
	dir_struct.wDir = wDir;
	dir_struct.oDir = oDir;
	dir_struct.sF = sF;
	//print_stat(&dir_struct);
	printf("----------------------------------------------------\n");
	//ONTO parsing directories
	printf("%ld\n",(long) parse_dir(&dir_struct));
	return 0;
}
