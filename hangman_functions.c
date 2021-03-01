
#include "hangman_header.h"

int wordAlreadyUsed(int totalWords, char *wordToGuess) {
	static char savedWords[MAXSAVEDWORDS][MAXCHARACTERS]; //save var over multiple function calls
	static int index = 0;

	for(int i = 0; i < MAXSAVEDWORDS; i++) {
		if(strcmp(savedWords[i], wordToGuess) == 0) {
			return 1;
		}
	}

	strcpy(savedWords[index], wordToGuess);
	index++;

	return 0;
}

void printGuessingBank(char *guessingBank) {
	printf("Letters not in word: ");
		for(int i = 0; i < LIVES; i++) {
			printf("%c ", guessingBank[i]);
		}

	printf("\n");
}

void checkFile(FILE *pFile) {
	if (pFile == NULL) {
		printf ("Error! File could not be opened!\n");
		exit(1);
	}
}

void printBoard(int position, FILE *pLog, char *board, char *blanks, char *guessingBank) {
	int logLives = LIVES - position;

	printf("\n%s\n", board);
	printf("\n| %s |\n", blanks);
	printGuessingBank(guessingBank);
	fprintf(pLog, "\n| %s |\n", blanks);
	fprintf(pLog, "\nLives: %d", logLives);
}

char readUserGuess(FILE *pLog, char *wordToGuess) {
	time_t currentWinTime;
	char guess[MAXCHARACTERS];
	printf("Enter a letter:\n> ");
	scanf("%s", guess);

	for(int i = 0; i < (signed int)strlen(guess); i++) { //cast strlen to signed to avoid std:c99 warning (unsigned/signed int comp.)
		guess[i] = toupper(guess[i]);
	}

	fprintf(pLog, "\nUser input: %s\n", guess);
	time(&currentWinTime);
	fprintf(pLog, "Time: %s\n", ctime(&currentWinTime));

	if(strncmp(guess, wordToGuess, MAXCHARACTERS) == 0) {
		printf("\nCongratulations, you win!\nThe word was %s.\n\n", wordToGuess);
		fprintf(pLog, "User managed to guess the word %s.\n\n", wordToGuess);
		fprintf(pLog, "\nGame has ended.\n");
		guess[0] = 0;
		return guess[0];
	} else if(strlen(guess) > 1) {
		printf("Game over!\nThe word was %s.\n\n", wordToGuess);
		fprintf(pLog, "\nGame over!\nThe word was %s.\n\n", wordToGuess);
		fprintf(pLog, "\nGame has ended.\n");
		guess[0] = 0;
		return guess[0];
	}

	if(isalpha(guess[0])) {
		return guess[0];
	}

	printf("Please only enter letters of the alphabet!\n");
	return readUserGuess(pLog, wordToGuess);
}

int isAlreadyGuessed(int position, char guess, char *guessingBank) {
	if(isInWord(position, guess, guessingBank)) {
		printf("This letter was already guessed.\n");
		return 1;
	} return 0;
}

int checkUserGuess(int wordLength, char guess, char *wordToGuess, char *blanks) {
	int correctUserGuess = 0;
	for(int i = 0; i < wordLength; i++) {
		if(wordToGuess[i] == guess) {
			blanks[i] = guess;
			correctUserGuess = 1;
		}
	}

	return correctUserGuess;
}

int playAgain() {
	char userInput;
	printf("Do you want to play again? (Y/N)");
	scanf(" %c", &userInput); //& needed for reading single char; whitespace needed to tell scanf to ignore whitesp.
	printf("%c", userInput);

	if(userInput == 'y' || userInput =='Y') {
		return 1;
	}

	if(userInput == 'n' || userInput =='N') {
		return 0;
	}

	printf("\nInvalid input. Please enter (Y/N).\n");
	return playAgain();
}

int checkIfWon(FILE *pLog, char *wordToGuess, char *blanks) {
	time_t currentWinTime;

	if(strcmp(wordToGuess, blanks) == 0) {
		printf("\nCongratulations, you win!\nThe word was %s.\n\n", wordToGuess);
		fprintf(pLog, "User managed to guess the word %s.\n\n", wordToGuess);
		fprintf(pLog, "\nGame has ended.\n");

		time(&currentWinTime);
		fprintf(pLog, "Time: %s\n", ctime(&currentWinTime));

		return 1;
	}
	return 0;
}

int isInWord(int position, char guess, char *guessingBank) {
	for(int i = 0; i < position; ++i) {
		if((guessingBank)[i] == guess) {
			return 1;
		}
	}

	return 0;
}

void pickWord(int *wordLength, FILE *pWordFile, int totalWords, char **wordToGuess, char **blanks, char **guessingBank) {
	int lines = totalWords;
	char c = 0;
	int seekPosition = 0;
	int characters = 0;
	int randomLine = 0;

	fseek(pWordFile, 0, SEEK_SET); //seek to beginning of file, because getc() changed position in file

	randomLine = (rand() % lines) + 1;
	lines = 0;
	c = 0;
	while(lines < randomLine && (c = getc(pWordFile)) != EOF) {
		if(lines + 1 == randomLine) {
			characters++;
		} else {
			seekPosition++;
		}
		if(c == '\n') {
			lines++;
		}
	}
	characters--;
	*wordLength = characters;

	if(*wordToGuess == NULL && *blanks == NULL && *guessingBank == NULL) {
		*wordToGuess = (char*) malloc(*wordLength + 1);
		*blanks = (char*) malloc(*wordLength);
		*guessingBank = (char*) malloc(LIVES + 1); //6 in guessingBank at max for 6 wrong guesses + NULL terminator

		if(*wordToGuess == NULL || *blanks == NULL || *guessingBank == NULL) { //check for errors (pointer returns NULL)
			fprintf(stderr, "Out of memory!\n");
			exit(1);
		}
	} else {
		*wordToGuess = realloc(*wordToGuess, *wordLength + 1);
		*blanks = realloc(*blanks, *wordLength);

		if(*wordToGuess == NULL || *blanks == NULL) {
			fprintf(stderr, "Out of memory!\n");
			exit(1);
		}
	}

	(*wordToGuess)[characters] = '\0';
	(*blanks)[characters] = '\0';
	(*guessingBank)[LIVES] = '\0';

	fseek(pWordFile, seekPosition, SEEK_SET); 		//set position in file at secret position
	fread(*wordToGuess, 1, characters, pWordFile); 	//read word into secret, 1 byte at a time, with length of characters, from handle

	for(int i = 0; i < *wordLength; i++) {
		(*wordToGuess)[i] = toupper((*wordToGuess)[i]);
	}

	for(int i = 0; i < *wordLength; i++) {
		(*blanks)[i] = '-';
	}

	for(int i = 0; i < LIVES; i++) {
		(*guessingBank)[i] = ' ';
	}
}
