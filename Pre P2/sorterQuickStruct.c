#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "Sorter.h"
#include <stdlib.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#define NUM 100     // its big so we can have enough space
#define Max_Thread_Size 50 
struct arraySort{
	struct mData* array;
	int start;
	int end;
	int comp_ptr;
};

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
void swap(struct mData* a, struct mData* b)
{
	struct mData temp = *a;
	*a = *b;
	*b = temp;
}
void insertionSort(struct mData* array,int size,int comp_ptr)
{
	int i,key,j;
	for (int i = 1; i <size ;i++)
	{
		j=i-1;
		while(j >= 0 && compare(array[j],array[i],comp_ptr) > 0)
		{
			array[j+1] = array[j];
			j = j-1;
		}
		array[j+1] = array[i];
	}
}

int randPartition(struct mData* array,int left, int right,int comp_ptr)
{

	int pivotPoint = ( rand() % (right-left+1)) + left;
	swap(&array[right],&array[pivotPoint]);

	int index = left;
	int i = left;
	for(;i<right;i++)
	{
		if(compare(array[i],array[right],comp_ptr) < 0)//array[i] < array[right])
		{
			swap(&array[i],&array[index]);
			index++;
		}
	}

		swap(&array[index],&array[right]);

		return index;
}

void quickSort(struct mData *array,int start, int end,int comp_ptr)
{
	if(start < end)
	{
		int mid = randPartition(array,start,end,comp_ptr);
		quickSort(array,start,mid-1,comp_ptr);
		quickSort(array,mid+1,end,comp_ptr);
	}
}
void *quickSort2(void *iS)
{	
	struct arraySort *inputStruct = (struct arraySort *)iS;
	int start = inputStruct -> start;
	int end = inputStruct -> end;
	struct mData* array = inputStruct -> array;
	int comp_ptr = inputStruct -> comp_ptr;
	if(start < end)
	{
		int mid = randPartition(array,start,end,comp_ptr);
		quickSort(array,start,mid-1,comp_ptr);
		quickSort(array,mid+1,end,comp_ptr);
	}
}
void quickSortThreaded(struct mData* array,int start,int end,int ptr)
{
	pthread_t tid;
	int numOfThreads = ((end+1)/Max_Thread_Size) + 1; 
	int tc;
	int tempStart = start;
	int tempEnd = start + Max_Thread_Size-1;
	if(tempEnd >end)
		tempEnd = end;
	printf("%d\n",numOfThreads );
	struct arraySort a;
	a.array = array;
	a.start = tempStart;
	a.end = tempEnd;
	a.comp_ptr = ptr;
	pthread_t *threads = malloc(sizeof(pthread_t) * (numOfThreads));
	for (int i = 0; i < numOfThreads; ++i)
	{
		printf("%d : %d : %d\n",tempStart,tempEnd,i);
		tc = pthread_create(&threads[i],NULL,quickSort2,(void*)&a);
			if (tc) {
				printf("ERROR; return code from pthread_create() is %d\n", tc);
				exit(-1);
			}
		tempStart = tempEnd+1;
		if(tempStart>end)
			tempStart = end;
		tempEnd = tempStart+Max_Thread_Size-1;
		if(tempEnd  >end)
			tempEnd = end;
		a.start = tempStart;
		a.end = tempEnd;
		
		
	}
	while ((numOfThreads - 1) >= 0) {
		numOfThreads--;
		pthread_join(threads[numOfThreads], NULL);
		//printf("Return value %ld\n", (long)ans);
	}
	insertionSort(array,end+1,ptr);

}
int main(int argc, char const *argv[])
{
		return 0;
}