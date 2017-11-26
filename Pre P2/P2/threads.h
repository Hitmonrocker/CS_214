#include <pthread.h>

struct file_dir{
	char *wDir;
	char *oDir;
	char *sF;
	char *curr_dir;
	int compare_size;
	int *compareArr;
};
struct wdir{
	char wDir[100];
};

struct Counter {
        int count;
        pthread_mutex_t lock;
};


void print_usage();
void print_stat(struct file_dir *str);
char* get_cwd(char* var);
void print_cwd(char* var);
int check_oDir(char* output); // 0 = Exists 1 = Does not exist 3 = Couldn't create
int check_wDir(char* output); // 0 = Exists 1 = Does not exist
