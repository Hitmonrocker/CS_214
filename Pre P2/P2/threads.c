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

//global struct counter & lock
struct Counter t_counter;

/*------------Function prototypes-------------*/
//counter struct
int counter_get(struct Counter *st);
void counter_inc(struct Counter *st);
void counter_init(struct Counter *st);
//Misc
void print_usage();
void print_stat(struct file_dir *str);
char* get_cwd(char* var);
void print_cwd(char* var);
int check_oDir(char* output); // 0 = Exists 1 = Does not exist 3 = Couldn't create
int check_wDir(char* output); // 0 = Exists 1 = Does not exist
//Parsing
void *parse_dir(void *st);
void *parse_dir2(void *st);
//pthread's
pthread_t pthread_self(void);
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
		void *(*start)(void *), void *arg);


void counter_init(struct Counter *st)
{
        st->count = 0;
        pthread_mutex_init(&st->lock, NULL);
}
void counter_inc(struct Counter *st)
{
        int val;

        pthread_mutex_lock(&st->lock);

        val = st->count;
        val = val + 1;
        st->count = val;

        pthread_mutex_unlock(&st->lock);
}
int counter_get(struct Counter *st)
{
        int val;

        pthread_mutex_lock(&st->lock);
        val = st->count;

        pthread_mutex_unlock(&st->lock);

        return val;
}
void *PrintHello(void *threadid) {
	long tid;
	tid = (long) threadid;
	printf("Hello World! It's me, thread #%ld!\n", tid);
	fflush(stdout);
	pthread_exit(NULL);
}
void *csv(void* st) {
	struct file_dir *str;
	str = (struct file_dir *) st;
	FILE *fp;
	char name[256];
	strcpy(name,str->wDir);
	strcat(name,".txt");
	fp = fopen(name, "w+");
	fclose(fp);
	pthread_exit(0);
}
void *parse_dir(void *st) {
	void *ans; // to catch the return value from pthread_exit()
	//convert void to struct
	struct file_dir *str;
	struct file_dir* str1 = malloc(NUM_THREADS * sizeof *str1); // for new threads
	struct wdir* wDir = malloc(NUM_THREADS * sizeof *wDir);
	str = (struct file_dir *) st;

	DIR *dir;
	struct dirent *dir_entry;

	int tc = 0, i = 0; //thread check & loop count int i
	pthread_t *threads;   // Array of threads
	threads = malloc(sizeof(pthread_t) * NUM_THREADS);

	//get current working directory
	/*	char cwd[256];
	 if (getcwd(cwd, sizeof(cwd)) == NULL)
	 perror("getcwd() error");
	 else
	 printf("current working directory2 is: %s\n", cwd);
	 */
	if (!(dir = opendir(str->wDir))){
		printf("return from %s\n",str->wDir);
		pthread_exit((void*) 24);
	}

	//chdir(str->wDir);

	//print_stat(str);
	while ((dir_entry = readdir(dir)) != NULL) {
		if (dir_entry->d_type == DT_DIR) { //if found a directory
			if (strcmp(dir_entry->d_name, ".") == 0
					|| strcmp(dir_entry->d_name, "..") == 0) {
				continue;
			}
			pthread_t self_id;
			self_id = pthread_self();
			fflush(stdout);
			//printf("Dir: Thread ID = %ld\n", self_id);
			printf("[%s], %ld\n", dir_entry->d_name, self_id);
			//save new struct info
			str1[i] = *str;
			//print_stat(&str1);
			//printf("This %s\n",wDir[i].wDir);
			strcpy(wDir[i].wDir, str->wDir);
			strcat(wDir[i].wDir, "/");
			strcat(wDir[i].wDir, dir_entry->d_name);
			str1[i].wDir = wDir[i].wDir;
			//printf("WOrking Dir = %s\n",str1[i].wDir);
			//print_stat(&str1);
			//parse_dir((void *) &str1);
			//printf("i = %d\n", i);
			tc = pthread_create(&threads[i], NULL, parse_dir, &str1[i]);
			i++;
			counter_inc(&t_counter);
			//free(wDir);
			if (tc) {
				printf("ERROR; return code from pthread_create() is %d\n", tc);
				pthread_exit((void*) -1);
			}
		} else {
			char *point = dir_entry->d_name;
			if ((point = strrchr(point, '.')) != NULL) { //Check if the file is CSV
				if (strcmp(point, ".csv") == 0) {
					//chdir(str1.wDir);
					pthread_t self_id;
					self_id = pthread_self();
					//printf("CSV Thread ID = %ld\n", self_id);
					//printf("found a CSV!\n");
					fflush(stdout);
					printf("(%s), %ld\n", dir_entry->d_name, self_id);
				//	memset(&(wDir[i]), 0, 100);
					strcpy(wDir[i].wDir, str->wDir);
					strcat(wDir[i].wDir, "/");
					strcat(wDir[i].wDir, dir_entry->d_name);
					str1[i].wDir = wDir[i].wDir;
					//printf("WDIR IS = %s\n",wDir);
				//	tc = pthread_create(&threads[i], NULL, PrintHello, &str1[i]);
				//	i++;
					counter_inc(&t_counter);
				}
			}
		}
		if (i == (i-1)) { //if threads are reaching the limit
			threads = realloc(threads, sizeof(pthread_t) * (NUM_THREADS * 2));
			wDir = realloc(wDir, sizeof(*wDir) * (NUM_THREADS * 2));
			str1 = realloc(str1,sizeof(*str1)* (NUM_THREADS * 2));
		}

	} // end while
	fflush(stdout);
	//printf("Value of i = %d\n", i);

	int k = 0;

	for (k = 0; k < i; k++) {
		pthread_join(threads[k], &ans);
		//printf("Return value %ld, %d\n", (long) ans, k);
	}
	//free(wDir);
	//printf("--------------------end1\n");
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
	char * wDir = (char *) malloc(sizeof(char) * 100); //wDir arg after -d
	char * oDir = (char *) malloc(sizeof(char) * 100); //oDir arg after -o
	//char* fullPath = (char*) malloc(sizeof(char) * 40);
	char* curr_dir = (char*) malloc(sizeof(char) * 100); //current working directory

	curr_dir = get_cwd(curr_dir);
	strcpy(wDir, curr_dir);
	strcpy(oDir, curr_dir);
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
	if ((wDir[0]) != '/') {//checks if it is a absolute path; if not then make it
		char * temp = (char *) malloc(sizeof(char) * 100);
		strcpy(temp, curr_dir);
		strcat(temp, "/");
		strcat(temp, wDir);
		strcpy(wDir, temp);
		//printf("%s\n", wDir);
	}
	// created a struct since when creating pthreads we cannot pass multiple parameters
	struct file_dir dir_struct;
	dir_struct.wDir = wDir;
	dir_struct.oDir = oDir;
	dir_struct.sF = sF;
	//print_stat(&dir_struct);
	printf("----------------------------------------------------\n");
	counter_init(&t_counter);
	//ONTO parsing directories
	fflush(stdout);
	parse_dir(&dir_struct);
	printf("The total number of threads %d\n",counter_get(&t_counter));
	return 0;
}
