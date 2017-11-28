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
#include "is.c"
#include <pthread.h>
#include "sort_starter.c"
#define NUM_THREADS	512

//global struct counter & lock
struct Counter t_counter;
pthread_mutex_t slock;

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
//printing
void print_final(struct mData *records, struct file_dir dir);

void print_final(struct mData *records, struct file_dir dir) {
	FILE *wf;
	//start here by creating the final file to print all this
	//line_number is the total number of entries;

	//print_stat(&dir);
	char *outFile = (char *) malloc(sizeof(char) * 300);
	strcpy(outFile, dir.oDir);
	strcat(outFile, "/");
	strcat(outFile, "AllFiles-sorted-");
	strcat(outFile, dir.sF);
	strcat(outFile, ".csv");
	wf = fopen(outFile, "w+");
	//printf("W = %s\n", outFile);
	//printf("ptr in: %c\n", first_ptr[0]);

	fprintf(wf,
			"color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,"
					"actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,"
					"actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,"
					"title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
	int i = 0;
	int size = records[0].line_number;
	for (; i < size; i++) {
		//printf(" Movie %s. \n",  records[i].mTitle);

		fprintf(wf,
				"%s,%s,%d,%d,%d,%d,%s,%d,%d,%s,%s,%s,%d,%d,%s,%d,%s,%s,%d,%s,%s,%s,%d,%d,%d,%g,%g,%d\n",
				records[i].color, records[i].dName, records[i].review,
				records[i].duration, records[i].dFbLikes, records[i].a3FbLikes,
				records[i].a2Name, records[i].a1FbLikes, records[i].gross,
				records[i].genres, records[i].a1Name, records[i].mTitle,
				records[i].votes, records[i].castFbLikes, records[i].a3Name,
				records[i].facenum, records[i].plot, records[i].movielink,
				records[i].userReview, records[i].language, records[i].country,
				records[i].cRating, records[i].budget, records[i].tYear,
				records[i].a2FbLikes, records[i].imdbScore, records[i].aRatio,
				records[i].movieFbLikes);
		//printf("\n");

	}
	fclose(wf);
}

void counter_init(struct Counter *st) {
	st->count = 0;
	pthread_mutex_init(&st->lock, NULL);
}
void s_lock(pthread_mutex_t lock) {
	pthread_mutex_init(&lock, NULL);
}
void counter_inc(struct Counter *st) {
	int val;
	pthread_mutex_lock(&st->lock);
	val = st->count;
	val = val + 1;
	st->count = val;
	pthread_mutex_unlock(&st->lock);
}
int counter_get(struct Counter *st) {
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
	strcpy(name, str->wDir);
	strcat(name, ".txt");
	fp = fopen(name, "w+");
	fclose(fp);
	pthread_exit(0);
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
	if (!(dir = opendir(str->wDir))) {
		//printf("return from %s\n", str->wDir);
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
			//printf("[%s], %ld\n", dir_entry->d_name, self_id);
			//printf("%ld,", self_id);
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
					fflush(stdout);
					//printf("(%s), %ld\n", dir_entry->d_name, self_id);
					//printf("%ld,", self_id);
					strcpy(wDir[i].wDir, str->wDir);
					strcat(wDir[i].wDir, "/");
					strcat(wDir[i].wDir, dir_entry->d_name);
					//printf("WDIR IS = %s\n",wDir[i].wDir);

					struct Sorter *sort = malloc(sizeof *sort);
					if (sort == NULL) {
						printf("Error allocating memory!\n");
					}
					sort->comp_ptr = str->compare;
					sort->lock = &slock;
					sort->wDir = wDir[i].wDir;
					sort->oDir = str->oDir;
					sort->final = str->final;
					//printf("The wDir name is %s\n", sort->wDir);
					//printf("The oDir T name is %s\n", sort->oDir);

					//printf("WDIR IS = %s\n",wDir);
					tc = pthread_create(&threads[i], NULL, file_sort, sort);
					i++;
					counter_inc(&t_counter);
				}
			}
		}

		if (i == (NUM_THREADS - 1)) { //if threads are reaching the limit
			//	printf("hit thread limit!, adding %d more threads\n",
			//	(NUM_THREADS * (i * 2)));
			void **hold1, **hold2, **hold3;
#undef NUM_THREADS
#define NUM_THREADS (i*2)
			//printf("new threads = %d\n", NUM_THREADS);
			hold1 = realloc(threads, sizeof(pthread_t) * (NUM_THREADS));
			hold2 = realloc(wDir, sizeof(*wDir) * (NUM_THREADS));
			hold3 = realloc(str1, sizeof(*str1) * (NUM_THREADS));
			if (threads == NULL || wDir == NULL || str1 == NULL) { //realloc failed
				printf("Realloc failed. Exiting");
				pthread_exit((void *) -1);
			} else {
				threads = (pthread_t *) hold1;
				wDir = (struct wdir*) hold2;
				str1 = (struct file_dir*) hold3;

			}
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
	struct mData* main_struct = malloc(sizeof *main_struct);
	char * sF = (char *) malloc(sizeof(char) * 25); //sF = sortingField. The arg after "-c"
	char * wDir = (char *) malloc(sizeof(char) * 200); //wDir arg after -d
	char * oDir = (char *) malloc(sizeof(char) * 200); //oDir arg after -o
	//char* fullPath = (char*) malloc(sizeof(char) * 40);
	char* curr_dir = (char*) malloc(sizeof(char) * 200); //current working directory

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
	/*---------------*/
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
	int compare_size = findInt(sF, cat, 27); // converts the sorting field into int
	/*---------------*/
	if ((wDir[0]) != '/') {	//checks if it is a absolute path; if not then make it
		char * temp = (char *) malloc(sizeof(char) * 200);
		strcpy(temp, curr_dir);
		strcat(temp, "/");
		strcat(temp, wDir);
		strcpy(wDir, temp);
		//printf("%s\n", wDir);
		free(temp);
	}
	if ((oDir[0]) != '/') {	//checks if it is a absolute path; if not then make it
		char * temp = (char *) malloc(sizeof(char) * 200);
		strcpy(temp, curr_dir);
		strcat(temp, "/");
		strcat(temp, oDir);
		strcpy(oDir, temp);
		//printf("%s\n", oDir);
		free(temp);
	}
	// created a struct since when creating pthreads we cannot pass multiple parameters
	struct file_dir dir_struct;
	dir_struct.wDir = wDir;
	dir_struct.oDir = oDir;
	dir_struct.sF = sF;
	dir_struct.compare = compare_size;
	dir_struct.final = main_struct;
	//print_stat(&dir_struct);
	pthread_t self_id;
	self_id = pthread_self();
	printf("Initial PID: %ld\n", self_id);
	counter_init(&t_counter);
	//ONTO parsing directories
	s_lock(slock);
	fflush(stdout);
	int tc;
	void * ret;
	pthread_t main_t;
	tc = pthread_create(&main_t, NULL, parse_dir, &dir_struct);
	if (tc) {
		printf(
				"ERROR creating 1st thread to parse; return code from pthread_create() is %d\n",
				tc);
		pthread_exit((void*) -1);
	}
	printf("TIDS of all child threads: ");
	printf("\n");
	pthread_join(main_t, &ret);
	//parse_dir(&dir_struct);
	printf("\n");
	printf("The total number of threads %d\n", counter_get(&t_counter));
	print_final(main_struct, dir_struct);
	return 0;
}
