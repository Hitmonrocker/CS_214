#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "Sorter.h"
int compare(data leftArr, data rightArr, int comp_ptr)
{

	switch(comp_ptr)
	{
		case 0:
			return strcmp(leftArr.color,rightArr.color);
		break;
		case 1:
			return strcmp(leftArr.dirName,rightArr.dirName);
		break;
		case 2:
			return strcmp(leftArr.critCount,rightArr.critCount);
		break;
		case 3:
			return (leftArr.durMin) - (rightArr.durMin);
		break;
		case 4:
			return strcmp(leftArr.dirFB,rightArr.dirFB);
		break;
		case 5:
			return  (leftArr.act3FB) - (rightArr.act3FB);
		break;
		case 6:
			return strcmp(leftArr.act2Name,rightArr.act2Name);
		break;
		case 7:
			return  (leftArr.act1FB) - (rightArr.act1FB);
		break;
		case 8:
			return  (leftArr.gross) - (rightArr.gross);
		break;
		case 9:
			return strcmp(leftArr.genre,rightArr.genre);
		break;
		case 10:
			return strcmp(leftArr.act1Name,rightArr.act1Name);
		break;
		case 11:
			return strcmp(leftArr.title,rightArr.title);
		break;
		case 12:
			return  (leftArr.numVoted) -  (rightArr.numVoted);
		break;
		case 13:
			return  (leftArr.totalFB) -  (rightArr.totalFB);
		break;
		case 14:
			return strcmp(leftArr.act3Name,rightArr.act3Name);
		break;
		case 15:
			return  (leftArr.faceNum) -  (rightArr.faceNum);
		break;
		case 16:
			return strcmp(leftArr.keyWord,rightArr.keyWord);
		break;
		case 17:
			return strcmp(leftArr.link,rightArr.link);
		break;
		case 18:
			return  (leftArr.numReview) -  (rightArr.numReview);
		break;
		case 19:
			return strcmp(leftArr.lang,rightArr.lang);
		break;
		case 20:
			return strcmp(leftArr.country,rightArr.country);
		break;
		case 21:
			return strcmp(leftArr.rated,rightArr.rated);
		break;
		case 22:
			return  (leftArr.budget) - (rightArr.budget);
		break;
		case 23:
			return  (leftArr.year) - (rightArr.year);
		break;
		case 24:
			return  (leftArr.act2FB) -  (rightArr.act2FB);
		break;
		case 25:
			return leftArr.score - rightArr.score;
		break;
		case 26:
			return leftArr.ratio - rightArr.ratio;
		break;
		case 27:
			 return leftArr.movieFB - rightArr.movieFB ;
		break;
		default:
		return  (leftArr.critCount) - (rightArr.critCount);
		break;

	}

}
void mergeData(data *array,int left , int middle , int right, int* comp_ptr,int comp_ptr_size) // Merges the two arrays together returns a combined array
{
	int size1,size2;
	int counter = 0;
	size1 = middle-left+1;
	size2 = right-middle;

	data *first = (data*) malloc(sizeof(data)*size1);
	data *second = (data*) malloc(sizeof(data)*size2);
	int i;
	for (i = 0; i < size1; ++i)
	{
		first[i] = array[left+i];
	}
	int j;
	for (j = 0; j < size2; ++j)
	{
		second[j] = array[middle+1+j];
	}

int iL = 0;
int iR = 0;
int iM = left;

while(iL < size1 && iR < size2)
{
	int comp = compare(first[iL],second[iR],comp_ptr[counter]);
	if(comp < 0)
	{
		array[iM] = first[iL];
		iL++;
	}
	 else if (comp > 0)
	{
		array[iM] = second[iR];
		iR++;
	}
	else
	{
		while((counter + 1) < comp_ptr_size)
		{
			int comp = compare(first[iL],second[iR],comp_ptr[counter]);
			if(comp < 0)
			{
			array[iM] = first[iL];
			iL++;
			}
			 else if (comp > 0)
			{
			array[iM] = second[iR];
			iR++;
			}
			else
				{
				}
		counter++;
		}
		else
		{
			array[iM] = first[iL];
			iL++;
		}

	}

iM++;
}
while(iL < size1)
{
	array[iM] = first[iL];
	iL++;
	iM++;
}
while(iR < size2)
{
	array[iM] = second[iR];
	iR++;
	iM++;
}
free(first);
free(second);
first = NULL;
second = NULL;

}

void split(data *array, int left, int right,int* comp_ptr,int comp_ptr_size)
{

	if (left < right)
	{

		int middle=left+(right-left)/2;
		split(array,left,middle,comp_ptr,comp_ptr_size);
		split(array,middle+1,right,comp_ptr,comp_ptr_size);
		mergeData(array,left,middle,right,comp_ptr,comp_ptr_size);
	}

}
/*
So here is what I changed I made strcmp the basic string comparison
I changed all the compares to addition or subtraction this way it 
returns the values <1,1, or >1 this way we can tell if something 
is greater than, equal to, or less than the other. If the are equal we move to the
next thing in comp_ptr i.e maybe we want to find movie_title and then director name
we keep doing this for as long as things are equal or until we have no more comparison parameters
to which we just treat equal case as less than. The only thing you need to send into split is the pointer and the 
size of the array for comparison ptr. Let me know if you have any questions. 




 */
