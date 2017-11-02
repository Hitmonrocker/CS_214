#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "Sorter.h"
#include <stdlib.h>
#include<math.h>
#include<stdlib.h>
#define NUM 100     // its big so we can have enough space
char * checkIf(char * p);
int isspace(int c);
int convert(int sF);
int file_sort(char* field, FILE *fp, const char * output_dir, char * filename,
		int* compareArr, int compareArr_size);
void *vPointers(struct mData arr[], int ctotal, int sF);

//global variable
struct mData *temp;

int changePos(struct mData **records, int left, int right) {
	//struct mData *temp = malloc(2 * sizeof *records);
	struct mData temp;
	temp = (*records)[left];
	(*records)[left] = (*records)[right];
	(*records)[right] = temp;
	return 0;
}

//------------

int compare(struct mData leftArr, struct mData rightArr, int comp_ptr) {
	switch (comp_ptr) {
	case 0:
		return strcmp(leftArr.color, rightArr.color);
		break;
	case 1:
		return strcmp(leftArr.dName, rightArr.dName);
		break;
	case 2:
		return (leftArr.review) - (rightArr.review);
		break;
	case 3:
		return (leftArr.duration) - (rightArr.duration);
		break;
	case 4:
		return (leftArr.dFbLikes) - (rightArr.dFbLikes);
		break;
	case 5:
		return (leftArr.a3FbLikes) - (rightArr.a3FbLikes);
		break;
	case 6:
		return strcmp(leftArr.a2Name, rightArr.a2Name);
		break;
	case 7:
		return (leftArr.a1FbLikes) - (rightArr.a1FbLikes);
		break;
	case 8:
		return (leftArr.gross) - (rightArr.gross);
		break;
	case 9:
		return strcmp(leftArr.genres, rightArr.genres);
		break;
	case 10:
		return strcmp(leftArr.a1Name, rightArr.a1Name);
		break;
	case 11:
		return strcmp(leftArr.mTitle, rightArr.mTitle);
		break;
	case 12:
		return (leftArr.votes) - (rightArr.votes);
		break;
	case 13:
		return (leftArr.castFbLikes) - (rightArr.castFbLikes);
		break;
	case 14:
		return strcmp(leftArr.a3Name, rightArr.a3Name);
		break;
	case 15:
		return (leftArr.facenum) - (rightArr.facenum);
		break;
	case 16:
		return strcmp(leftArr.plot, rightArr.plot);
		break;
	case 17:
		return strcmp(leftArr.movielink, rightArr.movielink);
		break;
	case 18:
		return (leftArr.userReview) - (rightArr.userReview);
		break;
	case 19:
		return strcmp(leftArr.language, rightArr.language);
		break;
	case 20:
		return strcmp(leftArr.country, rightArr.country);
		break;
	case 21:
		return strcmp(leftArr.cRating, rightArr.cRating);
		break;
	case 22:
		return (leftArr.budget) - (rightArr.budget);
		break;
	case 23:
		return (leftArr.tYear) - (rightArr.tYear);
		break;
	case 24:
		return (leftArr.a2FbLikes) - (rightArr.a2FbLikes);
		break;
	case 25:
		return (int) (leftArr.imdbScore - rightArr.imdbScore);
		break;
	case 26:
		return (int) (leftArr.aRatio - rightArr.aRatio);
		break;
	case 27:
		return leftArr.movieFbLikes - rightArr.movieFbLikes;
		break;
	default:
		return (leftArr.review) - (rightArr.review);
		break;

	}
}
void mergeData(struct mData *array, int left, int middle, int right,
		int* comp_ptr, int comp_ptr_size) { // Merges the two arrays together returns a combined array
	int size1, size2;
	int counter = 0;
	size1 = middle - left + 1;
	size2 = right - middle;

	struct mData *first = (struct mData*) malloc(sizeof(struct mData) * size1);
	struct mData *second = (struct mData*) malloc(sizeof(struct mData) * size2);
	int i;
	for (i = 0; i < size1; ++i) {
		first[i] = array[left + i];
	}
	int j;
	for (j = 0; j < size2; ++j) {
		second[j] = array[middle + 1 + j];
	}

	int iL = 0;
	int iR = 0;
	int iM = left;

	while (iL < size1 && iR < size2) {
		int comp = compare(first[iL], second[iR], comp_ptr[counter]);
		if (comp < 0) {
			array[iM] = first[iL];
			iL++;
		} else if (comp > 0) {
			array[iM] = second[iR];
			iR++;
		} else {
			int in = 0;
			while ((counter + 1) < comp_ptr_size && in == 0) {
				counter++;
				int comp = compare(first[iL], second[iR], comp_ptr[counter]);
				if (comp < 0) {
					array[iM] = first[iL];
					iL++;
					in = 1;
				} else if (comp > 0) {
					array[iM] = second[iR];
					iR++;
					in = 1;
				} else {
				}
			}

			if (in == 0) {
				array[iM] = first[iL];
				iL++;
			}
		}

		iM++;
	}
	while (iL < size1) {
		array[iM] = first[iL];
		iL++;
		iM++;
	}
	while (iR < size2) {
		array[iM] = second[iR];
		iR++;
		iM++;
	}

}

void split(struct mData *array, int left, int right, int* comp_ptr,
		int comp_ptr_size) {

	if (left < right) {

		int middle = left + (right - left) / 2;
		split(array, left, middle, comp_ptr, comp_ptr_size);
		split(array, middle + 1, right, comp_ptr, comp_ptr_size);
		mergeData(array, left, middle, right, comp_ptr, comp_ptr_size);
	}

}
//------------
void Print(FILE *wf, struct mData records[], int size) {

	fprintf(wf,
			"color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,"
					"actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,"
					"actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,"
					"title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
	int i;
	for (i = 1; i < size; i++) {
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

int file_sort(char* field, FILE *fp, const char * output_dir, char * filename,
		int* compareArr, int compareArr_size) {

	//printf("File being analyzed is %s\n", filename);

	char * newFile = (char*) malloc(sizeof(strlen(filename)) + 50);
	char * out_copy = (char*) malloc(sizeof(strlen(output_dir)) + 5);

	//printf("ptr in: %c\n", first_ptr[0]);

	if (output_dir[0] == '/') {			//absolute path
		strcpy(newFile, output_dir);
		strcat(newFile, "/");
		strcat(newFile, strip(filename));
		strcat(newFile, "-sorted");
		strcat(newFile, "-");
		strcat(newFile, field);
		strcat(newFile, ".csv");
		//printf("newFile is %s\n", newFile);
	} else { //this would save it in the directory it found the file in
		//strcpy(out_copy,output_dir);
		strcpy(newFile, strip(filename));
		strcat(newFile, "-sorted");
		strcat(newFile, "-");
		strcat(newFile, field);
		strcat(newFile, ".csv");
		//printf("newFile is %s\n", newFile);
	}

	//printf("Field being analyzed is %s\n", field);//print the field being analyzed
	// end checking

	char word;	// just a variable
	struct mData* records = malloc(NUM * sizeof *records);// struct that hold all the tokens from lines
	int count;// this will act as an index counter for each field of the struct
	char *line = NULL;	// pointer for each line
	size_t len = 0;
	ssize_t read;
	int ctotal = 1;	// count the total number of tokens
	int sF = 0;	// field being sorted.

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

		count = 1;

		while ((p = strsep(&line, ",")) != NULL) {
			//printf("Separated data is %s.\n", p);
			//printf("counter is %d \n", count);

			switch (count) {

			case 1:
				strcpy(records[ctotal].color, p);
				//strcpy(rec[ctotal]->color, p);
				//printf("the Color (rec[]) is %s->\n", rec[ctotal]->color);
				//printf("the Color is %s.\n", records[ctotal].color);
				//printf("counter is %d \n", count);
				count++;
				if (!strcmp(field, "color")) {
					sF = 1;
				}
				break;
			case 2:
				strcpy(records[ctotal].dName, p);
				//strcpy(rec[ctotal]->dName, p);
				//printf("Directors name (rec[]) is %s->\n", rec[ctotal]->dName);
				//printf("Directors name  is %s.\n", records[ctotal].dName);
				count++;
				if (!strcmp(field, "director_name")) {
					sF = 2;
				}
				break;
			case 3:
				stoi = atoi(p);
				records[ctotal].review = stoi;
				//printf("the #reviewers is %d.\n", records[ctotal].review);
				count++;
				if (!strcmp(field, "num_critic_for_reviews")) {
					sF = 3;
				}
				break;
			case 4:
				stoi = atoi(p);
				records[ctotal].duration = stoi;
				//printf("the duration is %d.\n", records[ctotal].duration);
				count++;
				if (!strcmp(field, "duration")) {
					sF = 4;
				}
				break;
			case 5:
				stoi = atoi(p);
				records[ctotal].dFbLikes = stoi;
				//printf("the #of FB Likes is %d.\n", records[ctotal].dFbLikes);
				count++;
				if (!strcmp(field, "director_facebook_likes")) {
					sF = 5;
				}
				break;
			case 6:
				stoi = atoi(p);
				records[ctotal].a3FbLikes = stoi;
				//printf("the Actor 3 FB likes is %d.\n",
				//	records[ctotal].a3FbLikes);
				count++;
				if (!strcmp(field, "actor_3_facebook_likes")) {
					sF = 6;
				}
				break;
			case 7:
				strcpy(records[ctotal].a2Name, p);
				//printf("the Actor 2 Name is %s.\n", records[ctotal].a2Name);
				count++;
				if (!strcmp(field, "actor_2_name")) {
					sF = 7;
				}
				break;
			case 8:
				stoi = atoi(p);
				records[ctotal].a1FbLikes = stoi;
				//printf("the Actor 1 FB Likes is %d.\n",
				//records[ctotal].a1FbLikes);
				count++;
				if (!strcmp(field, "actor_1_facebook_likes")) {
					sF = 8;
				}
				break;
			case 9:
				stoi = atoi(p);
				records[ctotal].gross = stoi;
				//printf("the gross $ is %d.\n", records[ctotal].gross);
				count++;
				if (!strcmp(field, "gross")) {
					sF = 9;
				}
				break;
			case 10:
				strcpy(records[ctotal].genres, p);
				//printf("the Genre is %s.\n", records[ctotal].genres);
				count++;
				if (!strcmp(field, "genres")) {
					sF = 10;
				}
				break;
			case 11:
				strcpy(records[ctotal].a1Name, p);
				//printf("the Actor 1 Name is %s.\n", records[ctotal].a1Name);
				count++;
				if (!strcmp(field, "actor_1_name")) {
					sF = 11;
				}
				break;
			case 12:
				//p = checkIf(p);
				strcpy(records[ctotal].mTitle, p);
				//printf("the Movie title is %s.\n", records[ctotal].mTitle);
				count++;
				if (!strcmp(field, "movie_title")) {
					sF = 12;
				}
				break;
			case 13:
				stoi = atoi(p);
				records[ctotal].votes = stoi;
				//printf("the #Votes is %d.\n", records[ctotal].votes);
				count++;
				if (!strcmp(field, "num_voted_users")) {
					sF = 13;
				}
				break;
			case 14:
				stoi = atoi(p);
				records[ctotal].castFbLikes = stoi;
				//printf("the Cast FB Likes is %d.\n",
				//records[ctotal].castFbLikes);
				count++;
				if (!strcmp(field, "cast_total_facebook_likes")) {
					sF = 14;
				}
				break;
			case 15:
				strcpy(records[ctotal].a3Name, p);
				//printf("the Actor 3 Name is %s.\n", records[ctotal].a3Name);
				count++;
				if (!strcmp(field, "actor_3_name")) {
					sF = 15;
				}
				break;
			case 16:
				stoi = atoi(p);
				records[ctotal].facenum = stoi;
				//printf("the poster Face count is %d.\n",
				//	records[ctotal].facenum);
				count++;
				if (!strcmp(field, "facenumber_in_poster")) {
					sF = 16;
				}
				break;
			case 17:
				strcpy(records[ctotal].plot, p);
				//printf("the Plot is %s.\n", records[ctotal].plot);
				count++;
				if (!strcmp(field, "plot_keywords")) {
					sF = 17;
				}
				break;
			case 18:
				strcpy(records[ctotal].movielink, p);
				//printf("the Movie URL is %s.\n", records[ctotal].movielink);
				count++;
				if (!strcmp(field, "movie_imdb_link")) {
					sF = 18;
				}
				break;
			case 19:
				stoi = atoi(p);
				records[ctotal].userReview = stoi;
				//printf("the User Review is %d.\n", records[ctotal].userReview);
				count++;
				if (!strcmp(field, "num_user_for_reviews")) {
					sF = 19;
				}
				break;
			case 20:
				strcpy(records[ctotal].language, p);
				//printf("the language is %s.\n", records[ctotal].language);
				count++;
				if (!strcmp(field, "language")) {
					sF = 20;
				}
				break;
			case 21:
				strcpy(records[ctotal].country, p);
				//printf("the country is %s.\n", records[ctotal].country);
				count++;
				if (!strcmp(field, "country")) {
					sF = 21;
				}
				break;
			case 22:
				strcpy(records[ctotal].cRating, p);
				//printf("the Content Rating is %s.\n", records[ctotal].cRating);
				count++;
				if (!strcmp(field, "content_rating")) {
					sF = 22;
				}
				break;
			case 23:
				stoi = atoi(p);
				records[ctotal].budget = stoi;
				//printf("the Budget is %d.\n", records[ctotal].budget);
				count++;
				if (!strcmp(field, "budget")) {
					sF = 23;
				}
				break;
			case 24:
				stoi = atoi(p);
				records[ctotal].tYear = stoi;
				//printf("the Release Year is %d.\n", records[ctotal].tYear);
				count++;
				if (!strcmp(field, "title_year")) {
					sF = 24;
				}
				break;
			case 25:
				stoi = atoi(p);
				records[ctotal].a2FbLikes = stoi;
				//printf("the Actor 2 FB Likes is %d.\n",
				//	records[ctotal].a2FbLikes);
				count++;
				if (!strcmp(field, "actor_2_facebook_likes")) {
					sF = 25;
				}
				break;
			case 26:
				stof = atof(p);
				records[ctotal].imdbScore = stof;
				//printf("imdb  is %s\n", p);
				//printf("the IMBD score is %g.\n", records[ctotal].imdbScore);
				count++;
				if (!strcmp(field, "imdb_score")) {
					sF = 26;
				}
				break;
			case 27:
				stof = atof(p);
				records[ctotal].aRatio = stof;
				//printf("aspect ratio is %s\n", p);
				//printf("the Aspect Ratio is %g.\n", records[ctotal].aRatio);
				count++;
				if (!strcmp(field, "aspect_ratio")) {
					sF = 27;
				}
				break;
			case 28:
				stoi = atoi(p);
				records[ctotal].movieFbLikes = stoi;
				//printf("the Facebook Moive Likes data is %d.\n",
				//records[ctotal].movieFbLikes);
				count++;
				if (!strcmp(field, "movie_facebook_likes")) {
					sF = 28;
				}
				break;

			default:
				printf("the added data is NULL.\n");
			}

			//break;

			if (count == 12) {
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
	}
	//printf("Ctotal = %d\n", ctotal);
	free(line);

	//MergeSort(records, pray, sF, field, 1, ctotal - 1);
	split(records, 1, ctotal - 1, compareArr, compareArr_size);
	//changePos(&records,2,1);
	//Print2(records,ctotal);
	FILE *wf;
	wf = fopen(newFile, "w+");

	Print(wf, records, ctotal);

	return 0;
}
