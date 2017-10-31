#include "Sorter.c"
#include "inputSplitter.c"
#include <string.h>
char *strtok_new(char * string, char const * delimiter);


int main(int argc, char const *argv[])
{

char*cat[] = {"color","director_num","num_critic_for_reviews","duration","director_facebook_likes","actor_3_facebook_likes","actor_2_name","actor_1_facebook_likes"\
    ,"gross","genres","actor_1_name","movie_title","num_voted_users","cast_total_facebook_likes","actor_3_name","facenumber_in_poster","plot_keywords","movie_imdb_link"\
    "num_user_for_reviews","language","country","content_rating","budget","title_year","actor_2_facebook_likes","imdb_score","aspect_ratio","movie_facebook_likes"}; 


int comp_ptr_size = getInputSize (argv[2]);

int *compareArr = (int*)malloc(sizeof(int)*comp_ptr_size); 
getInput(compareArr,argv[2],cat,27,comp_ptr_size);


  data *total = (data*)malloc(sizeof(data)*4);//
 FILE *fp = fopen(argv[1],"r");
  
//
  strcpy(total[1].color,"color");
  strcpy(total[1].dirName,"Shithead");
  total[1].critCount = 24;
  total[1].durMin = 120;
  total[1].dirFB = 10000;
  total[1].act3FB = 20013;
  strcpy(total[1].act2Name,"Jimmy Nuetron");
  total[1].act1FB = 410;
  total[1].gross = 4;
  strcpy(total[1].genre,"neato|cool");
  strcpy(total[1].act1Name,"vince");
  strcpy(total[1].title,"the story of sys prog");
  total[1].numVoted =12;
  total[1].totalFB =345789;
  strcpy(total[1].act3Name,"John");
  total[1].faceNum = 2;
  strcpy(total[1].keyWord,"theis|is|shit");
  strcpy(total[1].link,"www.ass.com");
  total[1].numReview = 125600; 
  strcpy(total[1].lang,"nigerian");
  strcpy(total[1].country,"bolivia");
  strcpy(total[1].rated,"R");
  total[1].budget = 894000;
  total[1].year = 2018;
  total[1].act2FB = 12315;
  total[1].score = 3.6;
  total[1].ratio = 16.9;
  total[1].movieFB = 123981390;
//
//
  strcpy(total[0].color,"color");
  strcpy(total[0].dirName,"Tuff");
  total[0].critCount = 24;
  total[0].durMin = 120;
  total[0].dirFB = 10000;
  total[0].act3FB = 20013;
  strcpy(total[0].act2Name,"Jol Nuetron");
  total[0].act1FB = 410;
  total[0].gross = 123;
  strcpy(total[0].genre,"scary");
  strcpy(total[0].act1Name,"Leo");
  strcpy(total[0].title,"life of pee");
  total[0].numVoted =12;
  total[0].totalFB =345789;
  strcpy(total[0].act3Name,"veton");
  total[0].faceNum = 2;
  strcpy(total[0].keyWord,"col|stuff|goes|down");
  strcpy(total[0].link,"www.reg.com");
  total[0].numReview = 125600; 
  strcpy(total[0].lang,"english");
  strcpy(total[0].country,"america");
  strcpy(total[0].rated,"PG-13");
  total[0].budget = 894001;
  total[0].year = 2018;
  total[0].act2FB = 12315;
  total[0].score = 3.6;
  total[0].ratio = 16.9;
  total[0].movieFB = 123981390;
//
//
  strcpy(total[3].color,"color");
  strcpy(total[3].dirName,"Shithead");
  total[3].critCount = 4;
  total[3].durMin = 123;
  total[3].dirFB = 13333;
  total[3].act3FB = 23313;
  strcpy(total[3].act2Name,"Honson");
  total[3].act1FB = 413;
  total[3].gross = 123;
  strcpy(total[3].genre,"hawwar");
  strcpy(total[3].act1Name,"Victor");
  strcpy(total[3].title,"Lovely Jenny");
  total[3].numVoted =12;
  total[3].totalFB =345789;
  strcpy(total[3].act3Name,"Jordan");
  total[3].faceNum = 2;
  strcpy(total[3].keyWord,"Rape|Grope|stuff");
  strcpy(total[3].link,"www.xvideos.com");
  total[3].numReview = 125633; 
  strcpy(total[3].lang,"spanish");
  strcpy(total[3].country,"Busch Campus");
  strcpy(total[3].rated,"XXX");
  total[3].budget = 894333;
  total[3].year = 2318;
  total[3].act2FB = 12315;
  total[3].score = 3.6;
  total[3].ratio = 16.9;
  total[3].movieFB = 123981393;
//
//
  strcpy(total[2].color,"color");
  strcpy(total[2].dirName,"John Austin");
  total[2].critCount = 22;
  total[2].durMin = 122;
  total[2].dirFB = 12222;
  total[2].act3FB = 22213;
  strcpy(total[2].act2Name,"hello");
  total[2].act1FB = 412;
  total[2].gross = 4;
  strcpy(total[2].genre,"Stick|Poke|people");
  strcpy(total[2].act1Name,"Leo");
  strcpy(total[2].title,"The Stupid Sleepy Kids");
  total[2].numVoted =12;
  total[2].totalFB =345789;
  strcpy(total[2].act3Name,"Shy gurl");
  total[2].faceNum = 2;
  strcpy(total[2].keyWord,"cool|stuff");
  strcpy(total[2].link,"www.stop.org");
  total[2].numReview = 125622; 
  strcpy(total[2].lang,"italian");
  strcpy(total[2].country,"italy");
  strcpy(total[2].rated,"PG");
  total[2].budget = 8944555;
  total[2].year = 2218;
  total[2].act2FB = 12315;
  total[2].score = 3.6;
  total[2].ratio = 16.9;
  total[2].movieFB = 123981392;
//
//
//
//
printf("%d\n",*compareArr );
split(total,0,3,compareArr,comp_ptr_size);
int i;
for (i = 0; i <4 ; i++)
  {
    printf("%s : %s : %d : %d\n",total[i].title,total[i].link,total[i].movieFB,total[i].year);
  }
 fclose(fp);



return 0;
}
