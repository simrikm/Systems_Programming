#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef enum State
{
	Deck,
	Play,
	Used_pile,
}State;

typedef struct Player
{
	char hand[22];
	char suit[22];
	int  money;
	int bet;
	int hand_value;
	int won;
}Player;

typedef struct Card
{
	char suit;
	char  deck_value;//to keep track of the letters print as a char in the end
	int value;
	State state;
}Card;

int ante=1;
Card deck[52];
Player player, dealer;
int total_bet_money=0;
int track_draw_card=0;

void create_a_deck()
{
	char suits[]={'H','C','D','S'};
	int index=0;
	for(int i=1;i <=13;++i)
	{
		for(int j=0; j<=3; ++j,index++)
		{
			switch(i){
				case 1:
					deck[index].deck_value='A';
					deck[index].value=11;
					break;
				case 11:	
					deck[index].deck_value='J';
					deck[index].value=10;
					break;
				case 12:
					deck[index].deck_value='Q';
					deck[index].value=10;
					break;
				case 13:
					deck[index].deck_value='K';
					deck[index].value=10;
					break;
				default:
					deck[index].deck_value=i+'0';
					deck[index].value=i;
					break;
			}		
			deck[index].suit=suits[j];
			deck[index].state=1;
		}
	}

}

void initialize()
{
	player.money=20;
	dealer.bet=ante;
	player.won=0;
	dealer.won=0;
}

void draw_a_card(){	
	int a=rand()%51;
	int b=rand()%51;
	if(deck[a].state==2||deck[a].state==1)
		a=rand()%51;
	if(deck[b].state==2||deck[b].state==1)
		b=rand()%51;		
	player.hand[track_draw_card]=deck[a].deck_value;
	dealer.hand[track_draw_card]=deck[b].deck_value;
	player.suit[track_draw_card]=deck[a].suit;
	dealer.suit[track_draw_card++]=deck[b].suit;
	deck[a].state=1;
	deck[b].state=1;
	player.hand_value+=deck[a].value;
	dealer.hand_value+=deck[b].value;

}

void print_dealers_card(){
	printf("Dealer's Card: ");	
	for (int i=0;i <track_draw_card;++i)
		printf("%c%c ",dealer.hand[i],dealer.suit[i]);	
	printf("HAND_WORTH: %d\n",dealer.hand_value);

}

void printcard(){
	printf("Players Card: ");
	for (int i=0;i <track_draw_card;++i)
		printf("%c%c ",player.hand[i],player.suit[i]);	
	printf("HAND_WORTH: %d\n",player.hand_value);
	
}

void reset(){
	total_bet_money=0;
	track_draw_card=0;
	player.hand_value=0;
	dealer.hand_value=0;
}

void game_start(){
	reset();
	srand(time(NULL));
	for(int i=0; i<2 ;++i)
		draw_a_card();	
		printcard();
}


int place_a_bet(){
	int bet_money;
	printf("How much would you like to bet?");
	scanf(" %d",&bet_money);
	return bet_money;
}


char betting(){	
	char answer;
	printf("Enter y to place a bet else enter n to place minimum bet: ");
	scanf(" %c",&answer);
	player.bet=answer=='y'?place_a_bet():ante;
	if(player.bet>player.money){
		printf("not enough money to bet,the game will be ended\n");
		return 'q';
	}
	player.money-=player.bet;
	total_bet_money=player.bet+dealer.bet;
	return 'p';
}



int  check_for_blackjack(){
	if(dealer.hand_value == 21){
			printf("Dealer blackjack\n");
			printcard();
			print_dealers_card();
			dealer.won++;
			return 1;		
		}
	

		if(player.hand_value ==21){
			printf("BLACKJACK\n");
			player.won++;
			printcard();
			print_dealers_card();
			player.money+=total_bet_money;
			return 1;
		}
	if(player.hand_value > 21){
		printf("you loose\n");
		dealer.won++;
		return 1;
	}
	
	if(dealer.hand_value > 21){
		printf("You won\n");
		player.won++;
		player.money+=total_bet_money;
		return 1;
	}
	return 0;

}


char continue_game(){
	char ans;
	printf("Press p to play again q to quit? ");	
	scanf(" %c",&ans);
	return ans;

}

void check_between_player_and_dealer(){
	if(dealer.hand_value < player.hand_value){
		printf("You Won!\n"); 
		player.money+=total_bet_money;
		player.won++;
	}else{
		printf("You Loose!\n");
		dealer.won++;
}
}

int ask_for_action(){
	int action;
	printf("Please choose the action:\n HIT: 0\n STAY: 1\n");
	scanf(" %d",&action);
	if(action==0){
		draw_a_card();
		printcard();
		if(check_for_blackjack()==1)
			action=1;	
	}else{
		check_for_blackjack();
		check_between_player_and_dealer();
		if(check_for_blackjack()==1)
			action=1;
	}
	return action;
}

void print_game_stat(){
	if(player.won > dealer.won)
		printf("You won the game!!! Your Money:  %d\n",player.money);
	else
		printf("You loose the game!!! Your Money: %d\n",player.money);
}


int main(){
	char answer;
	create_a_deck();
	initialize();
	do{	
	game_start();
	answer=betting();	
	while(answer!='q'){
	/**	if(check_for_blackjack()==1){
			break;	
		}else{
			if(ask_for_action()==1){
			printcard();
			print_dealers_card();
			break;		
			}
		}**/
		if(ask_for_action()==1)
			printcard();
			print_dealers_card();
			break;		
	}
		answer=continue_game();
	}
	while(answer=='p');
	print_game_stat();
	return 0;
}
