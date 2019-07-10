#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

typedef struct SHOT
{
	long thread_id;
	int score;
} shot;

typedef struct Queue
{
    int front, rear, size;
    unsigned capacity;
    //int* array;
    shot** array;
}Queue;

long game_total[20];
int length_of_the_game=0; 
Queue *queue;

pthread_mutex_t mtx;
pthread_cond_t cond;
pthread_cond_t condWaiting;
int readyflag=0;

// function to create a queue of given capacity. 
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0; 
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (shot**) malloc(queue->capacity * sizeof(shot));
    return queue;
}
 
// Queue is full when size becomes equal to the capacity 
int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }
 
// Function to add an item to the queue.  
// It changes rear and size
void enqueue(struct Queue* queue, shot *item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
  
}
 
// Function to remove an item from queue. 
// It changes front and size
shot * dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        perror("Empty queue");
    shot *item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
 
// Function to get front of queue
/**shot *front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}*
 
// Function to get rear of queue
shot * rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}**/

void* threadfunction(void* players_id){
	pthread_mutex_lock(&mtx);
	while(!readyflag){
		pthread_cond_signal(&condWaiting);
		pthread_cond_wait(&cond,&mtx);
	}//may be use another lock  here
	for(int i=0; i < length_of_the_game;++i){	
//	while(!readyflag){
//		pthread_cond_signal(&condWaiting);
//		pthread_cond_wait(&cond,&mtx);
//	}//may be use another lock  here
	int points=rand()%2+2;
	int scored=rand()%2+0;
	if(scored == 0)
		points = 0;
	
		shot * game=(shot *)malloc(sizeof(shot));
		game->thread_id=(long)players_id;
		game->score=points;
		enqueue(queue, game);
		game_total[(long)players_id] += points;
		pthread_mutex_unlock(&mtx);//might need to set this outside the loop 
	}
} 

void print_results(int players)
{
	int max=game_total[0];
	int winners[players];
	int winners_ndx = 0;
	int player;
	for(int i = 0; i < length_of_the_game * players; ++i)
	{
		 shot *game = dequeue(queue); 
		printf("Player %d -> Score %d\n", game->thread_id + 1, game->score);
	}

	for(int i = 0; i < players; ++i)
	{
		if(max < game_total[i]){
			max=game_total[i];
			player=i;
		}
		printf("Player %d -> Total Score %d\n", i + 1, game_total[i]);
	}
	if(max == 0)
		return;
	for(int i = 0; i < players; ++i)
	{
		if(game_total[i] == max)
			winners[winners_ndx++] = i;
	}
	for(int i = 0; i < winners_ndx; ++i)
	{
		printf("Champion with %d points: Player %d\n", max, winners[i] + 1);
	}
}

int main (int argc, char * argv[]){
	if(argc < 3){
		perror("Less number of argument");
		exit(1);//unsuccessful termination
	}
	int number_of_players=atoi(argv[1]);//number of threads you want to create
	 length_of_the_game=atoi(argv[2]);//the length of the game, wait time
	srand(time(NULL));
	 queue=createQueue(number_of_players*length_of_the_game);
	pthread_t thread[number_of_players];
	pthread_mutex_init(&mtx,NULL);
	for(long i=0; i<number_of_players; ++i){
		pthread_create(&thread[i],NULL,threadfunction,(void*)i);
	}
	pthread_mutex_lock(&mtx);
//	for(int i=0; i < length_of_the_game;++i){	
	readyflag=1;
	pthread_cond_broadcast(&cond);
//	sleep(1);	
//	}
	pthread_mutex_unlock(&mtx);
	for(int i=0;i < number_of_players; i++){
		pthread_join(thread[i],NULL);
	}
	pthread_mutex_destroy(&mtx);
	print_results(number_of_players);
	pthread_exit(NULL);
	
	return 0;
}
