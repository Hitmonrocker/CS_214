#include <pthread.h>

struct file_dir{
	char *wDir;
	char *oDir;
	char *sF;
	char *curr_dir;
	int compare;
	struct mData *final;

};
struct wdir{
	char wDir[200];
};

struct Counter {
        int count;
        pthread_mutex_t lock;
};

struct Sorter{
	//struct
	char *wDir;
	char *oDir;
	int k;
	int comp_ptr;
	pthread_mutex_t *lock;
	struct mData *final;

};


