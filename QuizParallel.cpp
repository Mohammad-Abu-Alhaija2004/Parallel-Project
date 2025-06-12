#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
pthread_mutex_t lock;
pthread_cond_t ready;
pthread_cond_t red_turn,blue_tern;
int current_card;
int red_count;
    int blue_count;
}cardcontrol;

cardcontrol car{

    .lock=PTHREAD_MUTEX_INITIALIZER,
    .ready=PTHREAD_COND_INITIALIZER,
    .red_turn=PTHREAD_COND_INITIALIZER,
    .blue_tern=PTHREAD_COND_INITIALIZER,
    current_card=0,
    red_count=0,
    blue_count=0

};


    char cards[5];
    

    srand(time(NULL));
    for(int i=0;i<5;i++){cards[i]= (rand()%2) ? 'R' : 'B';}

void* red_team (void* arg){

    while(current_card<5){

        pthread_mutex_lock(&car.lock);
        while (cards[current_card]!='R')
        {
            pthread_cond_wait(&car.red_tern,&car.lock);
        }
        
pthread_mutex_unlock(&car.lock);


    }

pthread_cond_signal(&car.blue_turn);
return NULL;
}

void* blue_team (void* arg){

    while(current_card<5){ 
        pthread_mutex_lock(&car.lock);
         while (cards[current_card]!='R')
        {
            pthread_cond_wait(&car.blue_tern,&car.lock);
        }

        pthread_mutex_unlock(&car.lock);
    }

pthread_cond_signal(&car.red_turn);
return NULL;
}



int main(){

    
 
    pthread_t threadR,threadB;

    pthread_create(&threadR,NULL,&red_team,NULL);
    pthread_create(&threadB,NULL,&red_team,NULL);

    for(int i=0;i<5;i++){

        pthread_mutex_lock(&car.lock);


        car.current_card=i;

        pthread_cond_broadcast(&car.red_turn);



    }

   

        pthread_join(threadR,NULL);
        pthread_join(threadB,NULL);

    

return 0;
}//main