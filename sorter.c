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
void file_sort(char* field, FILE *fp, const char * output_dir, char * filename);
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
void Merge(struct mData records[], void** pray, int sF, char* field, int left,
		int mid, int right, int size) {
//	printf("**********************************************************************************\n");
	int dataField = ((int *) pray)[0];
//	printf("The dataField is %d\n", dataField);
	int i = left;
	int j = mid;
	int m = mid + 1, n = right;
	char* one;
	char* two;
	int numOne;
	int numTwo;
	float flowOne;
	float flowTwo;

	if (dataField == 0) {
		//printf("The dataField 'strings' is %d\n", dataField);
		while (i <= j && m <= n) {
				//printf("The cpyLeft side is %s\n", Left[i]);
				//printf("The cpyRight side is %s\n", Right[i]);
				one = (char *) pray[i];
				two = (char *) pray[m];
		//		printf("one is %s\n", one);
		//		printf("two is %s\n", two);

				if ((strcmp(one, two)) <= 0) {
		//			printf("The cmp is %d\n", strcmp(pray[i], pray[j]));
					i++;
					//changePos(&records, m++, i++);

				} else {
		//			printf("switching %s and %s\n",one,two);
					changePos(&records, m, i++);
		//			printf("switched\n");

		//			int x;
		//		for (x = 1; x < size + 1; x++) {
		//				 printf("Movie title : %s\n",records[x].mTitle);
		//			}
				}


				}

			while (i < j) {
				//printf("im in t-2\n");
				i++;
				n++;
			}

			while (m < n) {
				//printf("im in t-1\n");
				Merge(records, pray, sF, field, m - 1, m, n, size);
				m++;
				j++;
			}

	}
	if (dataField == 1) {
		//printf("The dataField 'ints' is %d\n", dataField);
		while (i <= j && m <= n) {
				//printf("The cpyLeft side is %s\n", Left[i]);
				//printf("The cpyRight side is %s\n", Right[i]);
				numOne = *(int*) pray[i];
				numTwo = *(int*) pray[m];
		//		printf("one is %d\n", numOne);
		//		printf("two is %d\n", numTwo);

				if (numOne<numTwo) {
		//			printf("The cmp is %d\n", strcmp(pray[i], pray[j]));
					i++;
					//changePos(&records, m++, i++);
					//strcpy(records[k++].mTitle, Left[i++]);
				} else {
		//			printf("switching %s and %s\n",one,two);
					changePos(&records, m, i++);
					//int sum = ((m-1)+n)/2;
					//Merge(records, pray, sF, field, m-1, sum, n, size);
		//			printf("switched\n");

		//			int x;
		//			for (x = 1; x < size + 1; x++) {
		//				 printf("Movie title : %s\n",records[x].mTitle);
					}
					//changePos(&records, k++, m++);
					//strcpy(records[k++].mTitle, Right[j++]);
				}

			while (i < j) {
				//printf("im in t-2\n");
				i++;
				n++;
			}

			while (m < n) {
				//printf("im in t-1\n");
				Merge(records, pray, sF, field, m - 1, m, n, size);
				m++;
				j++;
			}
	}
	if (dataField == 2) {
		//printf("The dataField 'floats' is %d\n", dataField);
		while (i <= j && m <= n) {
						//printf("The cpyLeft side is %s\n", Left[i]);
						//printf("The cpyRight side is %s\n", Right[i]);
						flowOne = *(float*) pray[i];
						flowTwo = *(float*) pray[m];
				//		printf("one is %d\n", numOne);
				//		printf("two is %d\n", numTwo);

						if (flowOne<flowTwo) {
				//			printf("The cmp is %d\n", strcmp(pray[i], pray[j]));
							i++;
							//changePos(&records, m++, i++);
							//strcpy(records[k++].mTitle, Left[i++]);
						} else {
				//			printf("switching %s and %s\n",one,two);
							changePos(&records, m, i++);
							//int sum = ((m-1)+n)/2;
							//Merge(records, pray, sF, field, m-1, sum, n, size);
				//			printf("switched\n");

				//			int x;
				//			for (x = 1; x < size + 1; x++) {
				//				 printf("Movie title : %s\n",records[x].mTitle);
							}
							//changePos(&records, k++, m++);
							//strcpy(records[k++].mTitle, Right[j++]);
						}

					while (i < j) {
						//printf("im in t-2\n");
						i++;
						n++;
					}

					while (m < n) {
						//printf("im in t-1\n");
						Merge(records, pray, sF, field, m - 1, m, n, size);
						m++;
						j++;
					}
	}

	/*printf("address of 1 is %p\n", pray[1]);
	 printf("address of 2 is %p\n", pray[2]);
	 printf("address of 3 is %p\n", pray[3]);*/

	/*if ((strcmp(one, two)) < 0) {
	 printf("one is %s\n", one);
	 printf("two is %s\n", two);
	 printf("less than \n");
	 }*/



}

void MergeSort(struct mData records[], void** pray, int sF, char * field,
		int left, int right) {

	int size = right;
	int sum;
	/*printf("The field is %s\n", field);
	 printf("The left is %d\n", left);
	 printf("The right is %d\n", right);*/

	if (left < right) {
//		printf("----------------------------------------------------------------------------------\n");
//		printf("The 2 left is %d\n", left);
//		printf("The size/right is %d\n", size);
		sum = left + right;
		int mid = (sum / 2);
//		printf("The mid 1 is %d\n", mid);
		MergeSort(records, pray, sF, field, left, mid);
		//printf("The mid2 is %d\n", mid);
		MergeSort(records, pray, sF, field, mid + 1, right);
//		printf("before merge mid is %d\n", mid);
//		printf("before merge right is %d\n", right);
//		printf("before merge left is %d\n", left);
		Merge(records, pray, sF, field, left, mid, right, size);
	}

}

void Print(FILE *wf,struct mData records[], int size) {

	fprintf(wf,"color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,"
					"actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,"
					"actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,"
					"title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
	int i;
	for (i = 1; i < size; i++) {
		//printf(" Movie %s. \n",  records[i].mTitle);

		fprintf(
				wf,"%s,%s,%d,%d,%d,%d,%s,%d,%d,%s,%s,%s,%d,%d,%s,%d,%s,%s,%d,%s,%s,%s,%d,%d,%d,%g,%g,%d\n",
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



 void Print2(struct mData records[], int ctotal){
 int i ;
 for(i= 1;i<ctotal;i++){
	 printf("Movie title : %s\n",records[i].mTitle);
 }

 }//(uintptr_t)

//set void pointers to a field in struct
void *vPointers(struct mData arr[], int ctotal, int sF) {

	void** ray = malloc(sizeof(*arr) * ctotal);
	int u ;
	//printf("value of sF is %d\n",sF);
	for (u = 1; u < ctotal; u++) {
		switch (sF) {

		case 1:

			ray[u] = &arr[u].color;
			//printf("%d is %s\n", u, (char*) ray[u]);
			if (sF == 1) {
				ray[0] = (int*)0;
				//printf("%d is %d\n", u, *(int*)ray[0]);

			}
			break;
		case 2:
			ray[u] = &arr[u].dName;
			//printf("%d is %s\n", u, (char*) ray[u]);
			if (sF == 2) {
				ray[0] = (int*)0;
			}
			break;
		case 3:
			ray[u] = &arr[u].review;
			if (sF == 3) {
				ray[0] = (int*)1;
			}
			break;
		case 4:
			ray[u] = &arr[u].duration;
			//printf("%d is %d\n", u, *(int*)ray[u]);
			if (sF == 4) {
				ray[0] = (int*)1;
			}
			break;
		case 5:
			ray[u] = &arr[u].dFbLikes;
			if (sF == 5) {
				ray[0] = (int*)1;
			}
			break;
		case 6:
			ray[u] = &arr[u].a3FbLikes;
			if (sF == 6) {
				ray[0] = (int*)1;
			}
			break;
		case 7:
			ray[u] = &arr[u].a2Name;
			if (sF == 7) {
				ray[0] = (int*)0;
			}
			break;
		case 8:
			ray[u] = &arr[u].a1FbLikes;
			if (sF == 8) {
				ray[0] = (int*)1;
			}
			break;
		case 9:
			ray[u] = &arr[u].gross;
			if (sF == 9) {
				ray[0] = (int*)1;
			}
			break;
		case 10:
			ray[u] = &arr[u].genres;
			if (sF == 10) {
				ray[0] = (int*)0;
			}
			break;
		case 11:
			ray[u] = &arr[u].a1Name;
			if (sF == 11) {
				ray[0] = (int*)0;
			}
			break;
		case 12:
			ray[u] = &arr[u].mTitle;
			//printf("%d is %s\n", u, (char*) ray[u]);
			//printf("address of %s is %p\n",(char*) ray[u],ray[u]);
			if (sF == 12) {
				ray[0] = (int*)0;
				//printf("%d is %d\n", u, *(int*)ray[0]);

			}
			break;
		case 13:
			ray[u] = &arr[u].votes;
			if (sF == 13) {
				ray[0] = (int*)1;
			}
			break;
		case 14:
			ray[u] = &arr[u].castFbLikes;
			if (sF == 14) {
				ray[0] = (int*)1;
			}
			break;
		case 15:
			ray[u] = &arr[u].a3Name;
			if (sF == 15) {
				ray[0] = (int*)0;
			}
			break;
		case 16:
			ray[u] = &arr[u].facenum;
			if (sF == 16) {
				ray[0] = (int*)1;
			}
			break;
		case 17:
			ray[u] = &arr[u].plot;
			if (sF == 17) {
				ray[0] = (int*)0;
			}
			break;
		case 18:
			ray[u] = &arr[u].movielink;
			if (sF == 18) {
				ray[0] = (int*)0;
			}
			break;
		case 19:
			ray[u] = &arr[u].userReview;
			if (sF == 19) {
				ray[0] = (int*)1;
			}
			break;
		case 20:
			ray[u] = &arr[u].language;
			if (sF == 20) {
				ray[0] = (int*)0;
			}
			break;
		case 21:
			ray[u] = &arr[u].country;
			if (sF == 21) {
				ray[0] = (int*)0;
			}
			break;
		case 22:
			ray[u] = &arr[u].cRating;
			if (sF == 22) {
				ray[0] = (int*)0;
			}
			break;
		case 23:
			ray[u] = &arr[u].budget;
			if (sF == 23) {
				ray[0] = (int*)1;
			}
			break;
		case 24:
			ray[u] = &arr[u].tYear;
			if (sF == 24) {
				ray[0] = (int*)1;
			}
			break;
		case 25:
			ray[u] = &arr[u].a2FbLikes;
			if (sF == 25) {
				ray[0] = (int*)1;
			}
			break;
		case 26:
			ray[u] = &arr[u].imdbScore;
			if (sF == 26) {
				ray[0] = (int*)2;
			}
			break;
		case 27:
			ray[u] = &arr[u].aRatio;
			if (sF == 27) {
				ray[0] = (int*)2;
			}
			break;
		case 28:
			ray[u] = &arr[u].movieFbLikes;
			if (sF == 28) {
				ray[0] = (int*)1;
			}
			break;

		default:
			printf("error \n");
		}
	}
	//print2(ray);
	return ray;

}

//just a print funtion
int convert(int sF) {
	printf("******************************\n");
	printf("The field converted is %d\n", sF);
	printf("******************************\n");
	return 0;
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

void file_sort(char* field, FILE *fp,const char * output_dir, char * filename) {

	printf("File being analyzed is %s\n", filename);

	char * newFile = (char*)malloc(sizeof(strlen(filename))+50);
	strcpy(newFile,strip(filename));
	strcat(newFile,"-sorted");
	strcat(newFile,"-");
	strcat(newFile,field);
	strcat(newFile,".csv");
	printf("newFile is %s\n", newFile);


	//printf("Field being analyzed is %s\n", field);//print the field being analyzed
	// end checking

	char word;											// just a variable
	struct mData* records = malloc(NUM * sizeof *records);// struct that hold all the tokens from lines
	int count;// this will act as an index counter for each field of the struct
	char *line = NULL;								// pointer for each line
	size_t len = 0;
	ssize_t read;
	int ctotal = 1;						// count the total number of tokens
	int sF = 0;									// field being sorted.

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
	void** pray = malloc(sizeof(void*) * 2);
	pray = vPointers(records, ctotal, sF);
//	printf("on bottom is %d\n",  *(int*)pray);
	/*printf("address of 1 is %p\n",pray[1]);
	 printf("address of 2 is %p\n",pray[2]);
	 printf("address of 3 is %p\n",pray[3]);*/
//	printf("%s\n", *(char*)pray[1]);
//	printf("\n");
//	printf("\n");
//	printf("\n");
	MergeSort(records, pray, sF, field, 1, ctotal-1);
	//changePos(&records,2,1);
	//Print2(records,ctotal);
	FILE *wf;
	wf = fopen(newFile, "w+");

	Print(wf,records,ctotal);



	return 0;
}
