#include<pthread.h>
#include<time.h>
#include<string.h>
#include<stdio.h>
int p1=0;
int p2=0;
int n=0;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mVar=PTHREAD_MUTEX_INITIALIZER;
void *referee(void *arg);
void *player1(void *arg);
void *player2(void *arg);
int vcheck();
int dcheck();
void mprint();
int test=0;
int game_finish=0;
int a[6][7];
//int i,j; 
int main()
{
  int i,j;
  printf("Player 1-RED");
  printf("Player 2-YELLOW");
  for(i=0;i<6;i++)
    {
      for(j=0;j<7;j++)
	{
	  a[i][j] = 0;
	}
    }
  printf("Player 1-RED\n");
  printf("Player 2-YELLOW\n");

  mprint();
  pthread_t rtid,p1tid,p2tid; 
  pthread_create(&p2tid,NULL,player2,NULL);
  pthread_create(&p1tid,NULL,player1,NULL);
  pthread_create(&rtid,NULL,referee,NULL);
  pthread_join(p2tid,NULL);
  pthread_join(p1tid,NULL);
  pthread_join(rtid,NULL); 
  return 0;
}

void *referee(void *arg)
{
  int tie = 1;
  int i,j;
   for(;;)
    {
      pthread_mutex_lock(&mVar);
      while(test==1)
	{
	  pthread_cond_wait(&cond,&mVar);
	}
      test=1;
      vcheck();
      dcheck();
      if(p1==1)
      {
      	  game_finish = 1;
	  printf("\n PLAYER 1-RED  WON \n");
	  pthread_cond_signal(&cond);
	  pthread_mutex_unlock(&mVar);
	  return 0;
	}
      if(p2==1)
	{
	  game_finish = 1;
	  printf("\n PLAYER 2-YELLOW WON\n");
	  pthread_cond_signal(&cond);
          pthread_mutex_unlock(&mVar);
          return 0;
	}
      if ( tcheck() == 0)
	{
	printf("\n There is tie\n");
	game_finish = 1;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mVar);
     	return 0;
	}
      pthread_cond_signal(&cond);
      pthread_mutex_unlock(&mVar);
    }
}

int tcheck()
{
  int i,j;
  for(i=0;i<6;i++)
    {
	  for(j=0;j<7;j++)
	    {
	      if(a[i][j]==0)
		{
		  return(1);
		}
	    }
	}
  return(0);
}

int vcheck()
{
  int i,j;
  for(i=0;i<6;i++)
    {
      for(j=0;j<7;j++)
	{
	  if(a[i][j]==a[i+1][j] && a[i][j]==a[i+2][j] && a[i][j]==a[i+3][j] && a[i][j]==1 && (i+1)<6 && (i+2)<6 && (i+3)<6)
	    {
	      p1=1;
	      return 0;
	    }
	  
	  if(a[i][j]==a[i+1][j] && a[i][j]==a[i+2][j] && a[i][j]==a[i+3][j] && a[i][j]==2 && (i+1)<6 && (i+2)<6 && (i+3)<6)
	    {
	      p2=1;
	      return 0;
	    }
	}
    }
    return(1);
}
int dcheck()
{
  int i,j;
  for(i=0;i<6;i++)
    {
      for(j=0;j<7;j++)
	{
	  if(a[i][j]==a[i+1][j+1] && a[i][j]==a[i+2][j+2] && a[i][j]==a[i+3][j+3] && a[i][j]==1 && (i+1)<6 && (i+2)<6 && (i+3)<6 && (j+1)<7 && (j+2)<7 && (j+3)<7)
	    {
	      p1=1;
	      return 0;
	    }
          if(a[i][j]==a[i+1][j-1] && a[i][j]==a[i+2][j-2] && a[i][j]==a[i+3][j-3] && a[i][j]==1 && (i+1)<6 && (i+2)<6 && (i+3)<6 && (j-1)>-1 && (j-2)>-1 && (j-3)>-1)
	   {
	     p1=1;
	     return 0;
	   }
	  if(a[i][j]==a[i+1][j+1] && a[i][j]==a[i+2][j+2] && a[i][j]==a[i+3][j+3] && a[i][j]==2 && (i+1)<6 && (i+2)<6 && (i+3)<6 && (j+1)<7 && (j+2)<7 && (j+3)<7)
	  {
	    p2=2;
	    return 0;
	  }
	if(a[i][j]==a[i+1][j-1] && a[i][j]==a[i+2][j-2] && a[i][j]==a[i+3][j-3] && a[i][j]==2 && (i+1)<6 && (i+2)<6 && (i+3)<6 && (j-1)>-1 && (j-2)>-1 && (j-3)>-1)
	  {
	    p2=2;
	    return 0;
	  }
       }
    }
    return(1);
}

void *player1(void *arg)
{
  int row_cal,i,j;
  for(;;)
    {
      pthread_mutex_lock(&mVar);
      while(test==0)
	  pthread_cond_wait(&cond,&mVar);
      if(game_finish==1)
	{
	  pthread_cond_signal(&cond);
	  pthread_mutex_unlock(&mVar);
	  return 0;
	}
      row_cal=-1;
      srand(time(NULL));      
      while(row_cal==-1)
	{
	  sleep(1);
	  j=rand()%7;
	  printf("Player1-RED is playing\n");
	  printf("Coloumn selected:%d\n\n",j+1);
	  for(i=5;i>-1;i--)
	    {
	      if(a[i][j]==0) 
		{
		  row_cal=i;
		  break;
		}
	    }
	}
      a[row_cal][j]=1;
      mprint();
      test=0;
      pthread_cond_signal(&cond);
      pthread_mutex_unlock(&mVar);
      
    }
  return 0;
}
void *player2(void *arg)
{
  int row_cal,i,j;
  for(;;)
    {
      pthread_mutex_lock(&mVar);
      while(test==0)
        {
          pthread_cond_wait(&cond,&mVar);
	}
      if(game_finish==1)
	{
	  pthread_cond_signal(&cond);
	  pthread_mutex_unlock(&mVar);
	  return 0;
	}
      row_cal=-1;
       srand(time(NULL));
       while(row_cal==-1)
        {
	  sleep(1);
          j=rand()%7;
	  printf("Player2-YELLOW is playing\n");
	  printf("Coloumn Selected:%d\n\n",j+1);
          for(i=5;i>-1;i--)
            {
              if(a[i][j]==0)
                {
                  row_cal=i;
                  break;
                }
            }
        }
      a[row_cal][j]=2;
      mprint();
      test=0;
      pthread_cond_signal(&cond);
      pthread_mutex_unlock(&mVar);
    }
  return 0;
}

void mprint()
{
  int i,j;
  printf("\n");
  for(i=0;i<6;i++)
    {
      for(j=0;j<7;j++)
	{
	  printf("%d  ",a[i][j]);
	}
      printf("\n");
    }
}
