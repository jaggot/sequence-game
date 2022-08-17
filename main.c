#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SFML/Graphics.h>

int count(const char* toCount, const char** toFindIn, int lengthOfToFindIn){
	int returned = 0;
	for(int i = 0; i < lengthOfToFindIn; i++){
		if(toFindIn[i] == NULL){
			break;
		}
		if(strcmp(toCount, toFindIn[i]) == 0){
			returned++;
		}
	}
	return returned;
}

// you are passing a pointer to a pointer, to a function, as a pointer. Therefore three asterisks are required.
void shuffle(const char** deck, char*** newDeck, int deckSize){
	if(newDeck[0] != NULL){
		for(int i = 0; i < deckSize; i++){
			if(newDeck[0][i] != NULL){
				free(newDeck[0][i]);
			}
		}
		free(newDeck[0]);
	}
	newDeck[0] = malloc(sizeof(char*) * deckSize);
	for(int i = 0; i < deckSize; i++){
		newDeck[0][i] = NULL;
	}
	char* test;
	int len = 0;
	for(int i = 0; i < deckSize; i++){
		do{
			test = deck[rand()%deckSize];
		}while(count(test, newDeck[0], deckSize) >= 2);
		len = strlen(test);
		newDeck[0][i] = malloc(sizeof(char) * (len+1) );
		strcpy(newDeck[0][i], test);
		newDeck[0][i][len] = 0;
	}
}

char* append(const char* a, const char* b){
	int len = sizeof(char) * (strlen(a) + strlen(b) + 1);
	char* returned = malloc( len );
	strcpy(returned, a);
	strcpy(returned+strlen(a), b);
	returned[len-1] = 0;
	return returned;
}

int main(){
	//srand(time(0));
	#define maxCardTypes 13
	const char* allCardTypes[maxCardTypes]={
		"A",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"10",
		"J",
		"Q",
		"K"
	};
	int maxSuites = 4;
	const char* allSuites[4]={
		"C", "D", "H", "S"
	};

	int cardsGenerated = 0;
	const int cardsToGenerate = 2*13*4;

	int currentCardType = 0;
	int currentSuite = 0;

	// stack allocated
	const char* allCards[cardsToGenerate];
	for(currentCardType = 0; currentCardType < maxCardTypes; ){
		for(currentSuite = 0; currentSuite < maxSuites; ){
			for(int j = 0; j < 2; j++){
				allCards[cardsGenerated] = append(allCardTypes[currentCardType], allSuites[currentSuite]);
				//printf("%s ", allCards[cardsGenerated]);
				cardsGenerated++;
			}
			currentSuite++;
		}
		//printf("\n");
		currentCardType++;
		//cardsGenerated++;
	}

	char** wholeDeck = NULL;
	// now wholeDeck will be heap allocated. Remember to delete/free.

	shuffle(allCards, &wholeDeck, cardsToGenerate);

	for(int i = 0; i < cardsToGenerate; i++){
		if(strcmp(wholeDeck[i], "AH") == 0){
			printf("\033[0;31m");
		}
		printf("%s ", wholeDeck[i]);
		if(strcmp(wholeDeck[i], "AH") == 0){
			printf("\033[m");
		}
		if( (i+1) % 4 == 0){
			printf("\033[0;31m");
			printf("--%d\n", i);
			printf("\033[m");
		}
	}


//	------------THE GAME----------------------------

	int cardIndex = 0;
	char* playerCards[3][4];
	for(int i = 0; i < 3; i++){
		printf("Player %d: ", i);
		for(int j = 0; j < 4; j++){
			playerCards[i][j] = wholeDeck[cardIndex++];
			printf("%s, ", playerCards[i][j]);
		}
		printf("\n");
	}
	
	//diamond, club jacks are wild
	//heart, spades jacks remove


//	------------END-GAME----------------------------

	
	//-------CLEAN UP---------------------------------

	//if(allCards != NULL){
		for(int i = 0; i < cardsToGenerate; i++){
			if(allCards[i] != NULL){
				free(allCards[i]);
			}
		}
	/*
		free(allCards);
	}
	*/
	if(wholeDeck != NULL){
		for(int i = 0; i < cardsToGenerate; i++){
			if(wholeDeck[i] != NULL){
				free(wholeDeck[i]);
			}
		}
		free(wholeDeck);
	}
	sfRenderWindow* window = sfRenderWindow_create((sfVideoMode){ .width=800, .height=500 }, "Lol", sfClose, sfContextDefault );
	sfRenderWindow_setPosition(window, (sfVector2i){ 100, 50 });
	while(sfRenderWindow_isOpen(window)){
		sfEvent e;
		while(sfRenderWindow_pollEvent(window, &e)){
			if(e.type == sfEvtClosed){
				sfRenderWindow_close(window);
			}
		}
		sfRenderWindow_clear(window, sfBlack);
		sfRenderWindow_display(window);
	}
	return 0;
}
