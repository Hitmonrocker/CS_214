struct file_dir{
	char *wDir;
	char *oDir;
	char *sF;
	char *curr_dir;
	int compare_size;
	int *compareArr;
};



void print_usage();
void print_stat(struct file_dir *str);
char* get_cwd(char* var);
void print_cwd(char* var);
int check_oDir(char* output); // 0 = Exists 1 = Does not exist 3 = Couldn't create
int check_wDir(char* output); // 0 = Exists 1 = Does not exist
