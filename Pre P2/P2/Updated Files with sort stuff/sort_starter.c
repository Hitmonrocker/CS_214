#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "Sorter.h"
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include "sorterQuickStruct.c"
#define NUM 8192     // its big so we can have enough space

char * checkIf(char * p);
void *file_sort(void *str);

//function to check space
char * checkIf(char * p) {
	int var = p[0];
	int it;
	//makemakprintf("char in is %s\n",p);
	if (isspace(var)) {
		//printf("var = |%c| is a white-space character\n", var);
		char * new = malloc(strlen(p));
		it = 0;
		if (isspace(p[strlen(p) - 1])) {
			it = 2;
			while (isspace(p[strlen(p) - it])) {
				it++;
			}
			//printf("1 It is %d\n", it);
		}
		//printf("It is %d\n", (strlen(p) - it));
		memcpy(new, &p[1], strlen(p) - it);
		return new;
	} else {
		//printf("var = |%c| is not a white-space character\n", var);
		char * new = malloc(strlen(p));
		it = 0;
		if (isspace(p[strlen(p) - 1])) {
			it = 2;
			while (isspace(p[strlen(p) - it])) {
				it++;
			}
			//printf("char in back end %c\n",(p[strlen(p) - it]));
			//printf("2 It is %d\n", it);
		}
		//printf("It is %d\n", (strlen(p) - it));
		memcpy(new, &p[0], strlen(p) - it);
		return new;
	}
}

void Print(struct Sorter *st,struct mData records[]) {

	printf(
			"color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,"
					"actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,"
					"actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,"
					"title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
	int i =0;
	int size = st->final->line_number;
	for (; i < size; i++) {
		//printf(" Movie %s. \n",  records[i].mTitle);
		/*printf(
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
				records[i].movieFbLikes);*/
		//printf("\n");
						strcpy(st->final->color,records[i].color);
						strcpy(st->final->dName,records[i].dName);
						st->final->review = records[i].review;
						st->final->duration = records[i].duration;
						st->final->dFbLikes = records[i].dFbLikes;
						st->final->a3FbLikes = records[i].a3FbLikes;
						strcpy(st->final->a2Name,records[i].a2Name);
						st->final->a1FbLikes = records[i].a1FbLikes;
						st->final->gross = records[i].gross;
						strcpy(st->final->genres,records[i].genres);
						strcpy(st->final->a1Name,records[i].a1Name);
						strcpy(st->final->mTitle,records[i].mTitle);
						st->final->votes = records[i].votes;
						st->final-> castFbLikes = records[i].castFbLikes;
						strcpy(st->final->a3Name, records[i].a3Name);
						st->final->facenum = records[i].facenum;
						strcpy(st->final->plot,records[i].plot);
						strcpy(st->final->movielink,records[i].movielink);
						st->final->userReview = records[i].userReview;
						strcpy(st->final->language,records[i].language);
						strcpy(st->final->country,records[i].country);
						strcpy(st->final->cRating,records[i].cRating);
						st->final->budget = records[i].budget;
						st->final->tYear= records[i].tYear;
						st->final->a2FbLikes = records[i].a2FbLikes;
						st->final->imdbScore = records[i].imdbScore;
						st->final->aRatio = records[i].aRatio;
						st->final->movieFbLikes = records[i].movieFbLikes;

	}
	st->final->line_number = i;
}

void *file_sort(void *str) {

	//printf("File being analyzed is %s\n", filename);

	//char * newFile = (char*) malloc(sizeof(strlen(filename)) + 50);
	//	char * out_copy = (char*) malloc(sizeof(strlen(output_dir)) + 5);

	//printf("ptr in: %c\n", first_ptr[0]);

	//printf("Field being analyzed is %s\n", field);//print the field being analyzed
	// end checking

	//convert
	struct Sorter *st ;
	st = (struct Sorter *)str;
		//printf("------ The sF is %d\n", st->comp_ptr);
		printf("The wDir name is %s\n", st->wDir);
		printf("The oDir name is %s\n", st->oDir);

	//open the file
	FILE *fp;
	fp = fopen(st->wDir, "r");
	if (fp == NULL) {
		printf("\n");
		printf("Could not open file for sorting.\n");
		pthread_exit(0);
	}

	struct mData* records = malloc(NUM * sizeof *records);// struct that hold all the tokens from lines
	int count;// this will act as an index counter for each field of the struct
	char *line = NULL;	// pointer for each line
	size_t len = 0;
	ssize_t read;
	int ctotal = 0;	// count the total number of tokens

	// Skip the first line by reading and ignoring the extra data
	read = getline(&line, &len, fp);
	// end skipping

// Should be at the start of the first line

	//this while loop will break each line into tokens
	while ((read = getline(&line, &len, fp)) != -1) {
		//printf("Retrieved line of length %zu :\n", read);
		//printf("%s", line);
		//printf("Entered string is %s \n", line);
		char *p;

		char * temp = NULL;
		int stoi;
		float stof;
		char *result = (char *) malloc(8);

		count = 0;

		while ((p = strsep(&line, ",")) != NULL) {
			//printf("Separated data is %s.\n", p);
			//printf("counter is %d \n", count);

			switch (count) {

			case 0:
				strcpy(records[ctotal].color, p);
				//strcpy(rec[ctotal]->color, p);
				//printf("the Color (rec[]) is %s->\n", rec[ctotal]->color);
				//printf("the Color is %s.\n", records[ctotal].color);
				//printf("counter is %d \n", count);
				count++;
				break;
			case 1:
				strcpy(records[ctotal].dName, p);
				//strcpy(rec[ctotal]->dName, p);
				//printf("Directors name (rec[]) is %s->\n", rec[ctotal]->dName);
				//printf("Directors name  is %s.\n", records[ctotal].dName);
				count++;
				break;
			case 2:
				stoi = atoi(p);
				records[ctotal].review = stoi;
				//printf("the #reviewers is %d.\n", records[ctotal].review);
				count++;
				break;
			case 3:
				stoi = atoi(p);
				records[ctotal].duration = stoi;
				//printf("the duration is %d.\n", records[ctotal].duration);
				count++;
				break;
			case 4:
				stoi = atoi(p);
				records[ctotal].dFbLikes = stoi;
				//printf("the #of FB Likes is %d.\n", records[ctotal].dFbLikes);
				count++;
				break;
			case 5:
				stoi = atoi(p);
				records[ctotal].a3FbLikes = stoi;
				//printf("the Actor 3 FB likes is %d.\n",
				//	records[ctotal].a3FbLikes);
				count++;
				break;
			case 6:
				strcpy(records[ctotal].a2Name, p);
				//printf("the Actor 2 Name is %s.\n", records[ctotal].a2Name);
				count++;
				break;
			case 7:
				stoi = atoi(p);
				records[ctotal].a1FbLikes = stoi;
				//printf("the Actor 1 FB Likes is %d.\n",
				//records[ctotal].a1FbLikes);
				count++;
				break;
			case 89:
				stoi = atoi(p);
				records[ctotal].gross = stoi;
				//printf("the gross $ is %d.\n", records[ctotal].gross);
				count++;
				break;
			case 9:
				strcpy(records[ctotal].genres, p);
				//printf("the Genre is %s.\n", records[ctotal].genres);
				count++;
				break;
			case 10:
				strcpy(records[ctotal].a1Name, p);
				//printf("the Actor 1 Name is %s.\n", records[ctotal].a1Name);
				count++;
				break;
			case 11:
				//p = checkIf(p);
				strcpy(records[ctotal].mTitle, p);
				//printf("the Movie title is %s.\n", records[ctotal].mTitle);
				count++;
				break;
			case 12:
				stoi = atoi(p);
				records[ctotal].votes = stoi;
				//printf("the #Votes is %d.\n", records[ctotal].votes);
				count++;
				break;
			case 13:
				stoi = atoi(p);
				records[ctotal].castFbLikes = stoi;
				//printf("the Cast FB Likes is %d.\n",
				//records[ctotal].castFbLikes);
				count++;
				break;
			case 14:
				strcpy(records[ctotal].a3Name, p);
				//printf("the Actor 3 Name is %s.\n", records[ctotal].a3Name);
				count++;
				break;
			case 15:
				stoi = atoi(p);
				records[ctotal].facenum = stoi;
				//printf("the poster Face count is %d.\n",
				//	records[ctotal].facenum);
				count++;
				break;
			case 16:
				strcpy(records[ctotal].plot, p);
				//printf("the Plot is %s.\n", records[ctotal].plot);
				count++;
				break;
			case 17:
				strcpy(records[ctotal].movielink, p);
				//printf("the Movie URL is %s.\n", records[ctotal].movielink);
				count++;
				break;
			case 18:
				stoi = atoi(p);
				records[ctotal].userReview = stoi;
				//printf("the User Review is %d.\n", records[ctotal].userReview);
				count++;
				break;
			case 19:
				strcpy(records[ctotal].language, p);
				//printf("the language is %s.\n", records[ctotal].language);
				count++;
				break;
			case 20:
				strcpy(records[ctotal].country, p);
				//printf("the country is %s.\n", records[ctotal].country);
				count++;
				break;
			case 21:
				strcpy(records[ctotal].cRating, p);
				//printf("the Content Rating is %s.\n", records[ctotal].cRating);
				count++;
				break;
			case 22:
				stoi = atoi(p);
				records[ctotal].budget = stoi;
				//printf("the Budget is %d.\n", records[ctotal].budget);
				count++;
				break;
			case 23:
				stoi = atoi(p);
				records[ctotal].tYear = stoi;
				//printf("the Release Year is %d.\n", records[ctotal].tYear);
				count++;
				break;
			case 24:
				stoi = atoi(p);
				records[ctotal].a2FbLikes = stoi;
				//printf("the Actor 2 FB Likes is %d.\n",
				//	records[ctotal].a2FbLikes);
				count++;
				break;
			case 25:
				stof = atof(p);
				records[ctotal].imdbScore = stof;
				//printf("imdb  is %s\n", p);
				//printf("the IMBD score is %g.\n", records[ctotal].imdbScore);
				count++;
				break;
			case 26:
				stof = atof(p);
				records[ctotal].aRatio = stof;
				//printf("aspect ratio is %s\n", p);
				//printf("the Aspect Ratio is %g.\n", records[ctotal].aRatio);
				count++;
				break;
			case 27:
				stoi = atoi(p);
				records[ctotal].movieFbLikes = stoi;
				//printf("the Facebook Moive Likes data is %d.\n",
				//records[ctotal].movieFbLikes);
				count++;
				break;

			default:
				printf("the added data is NULL.\n");
			}

			//break;

			if (count == 11) {
				char *copy = malloc(strlen(line) + 1);
				char *copy2 = malloc(strlen(line) + 1);
				//printf("count = 12\n");
				temp = line;

				//printf("1 gline string in round 12 is %s.\n", temp);
				if (temp[0] == '"') {
					//	printf("Quotes encountered ******************************\n");
					temp[0] = ' ';
					strcpy(copy, line);
					//	printf("1 copy string in round 12 is %s.\n", copy);
					//	printf("temp string in round 12 is %s.\n", temp);
					result = strtok(line, "\"");
					strcpy(copy2, result);
					result = strtok(NULL, "\"");
					//	printf("2copy string in round 12 is %s.\n", copy);
					///	printf("The result string is %s.\n", result);
					result[0] = ' ';
					line = result;
					//	printf("line string in round 12 is %s.\n", line);
					copy2 = checkIf(copy2);
					strcpy(records[ctotal].mTitle, copy2);
					//	printf("the movie data is %s.\n", records[ctotal].mTitle);
					count++;

				}
			}
		}
		ctotal++;
		//printf("\n");
		if (ctotal == (NUM - 1)) {
			void **hold1, **hold2;
#undef NUM
#define NUM (ctotal*2)
			//printf("new threads = %d\n", NUM_THREADS);
			hold1 = realloc(records, sizeof *records * NUM);
			if(st->final->line_number < (NUM-1)){
			hold2 = realloc(st->final, sizeof(st->final) * NUM);
			}
			if (hold1 == NULL || hold2 == NULL) { //realloc failed
				printf("Realloc failed. Exiting");
				fflush(stdout);
				pthread_exit((void *) -1);
			}else{
			records =(struct mData *) hold1;
			st->final = (struct mData *)hold2;
			}
		}
	}					// end while
						//printf("Ctotal = %d\n", ctotal);
	free(line);
	//end opening file
	fclose(fp);
	//MergeSort(records, pray, sF, field, 1, ctotal - 1);
	//split(records, 1, ctotal - 1, compareArr, compareArr_size);

	//insert new sort functions
	int k = ctotal/20;
	quickSort2(st->final,0,ctotal,k,st->comp_ptr);
	bubbleSort(st->final,ctotal,st->comp_ptr);

	pthread_mutex_lock(st->lock);
	//Print(st,records);
	pthread_mutex_unlock(st->lock);

	return 0;
}
