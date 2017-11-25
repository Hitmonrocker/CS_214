#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

void print_usage();
void print_stat(char *w, char *o, char*s);
char* get_cwd(char* var);
void print_cwd(char* var);

void print_cwd(char* var) {
	get_cwd(var);
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
void print_stat(char *w, char *o, char*s) {
	printf("------ The sF is %s\n", s);
	printf("The wDir name is %s\n", w);
	printf("The oDir name is %s\n", o);
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
	char * wDir = (char *) malloc(sizeof(char) * 40);
	char * oDir = (char *) malloc(sizeof(char) * 40);
	//char* fullPath = (char*) malloc(sizeof(char) * 40);
	char* curr_dir = (char*) malloc(sizeof(char) * 100);
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
	print_stat(wDir, oDir, sF);
	return 0;
}
