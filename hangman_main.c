/*!
 * \author Patrick Rotter
 * \version 1.0
 * \date 16.01.2021
 * \mainpage Project HANGMAN - Official documentation
 * \section description Project description
 * This project represents the classic game "Hangman".
 * The player is provided a secret word and tries to guess it by suggesting letters within a certain number of guesses.
 */

#include "hangman_header.h"

int main(int argc, char *argv[]) {
	srand(time(NULL));

	char *guessingBank = NULL;	//array to save already guessed wrong letters
	char *wordToGuess = NULL;	//secret word
	char *blanks = NULL;		//array to save blank spaces and correctly guessed letters
	int gameLoop = 1;			//variable to execute game loop
	int position = 0;			//counter for incorrectly entered letters, used as a position marker
	int wordLength = 0;			//length of secret word
	int totalWords = 0;			//amount of words from input file
	int roundsPlayed = 0;		//counter for played rounds, used to determine if all words have been played
	time_t currentTime; 		//struct to save current time
	char wordFileLocation[MAXFILENAMELENGTH];
	char logFileLocation[MAXFILENAMELENGTH];
	char *board[LIVES] = {" ___\n|   |\n|\n|\n|", "\n ___\n|   |\n|   O\n|\n|", "\n ___\n|   |\n|   O\n|   |\n|",
				"\n ___\n|   |\n|   O\n|  /|\n|", "\n ___\n|   |\n|   O\n|  /|\\\n|",
				"\n ___\n|   |\n|   O\n|  /|\\\n|  /", "\n ___\n|   |\n|   O\n|  /|\\\n|  / \\"}; //string literals, pointer to char

	strcpy(wordFileLocation, argv[1]);
	strcpy(logFileLocation, argv[2]);

	FILE *wordInputFile;
	wordInputFile = fopen(wordFileLocation, "r");
	checkFile(wordInputFile);

	FILE *logFile;
	logFile = fopen(logFileLocation, "a");
	checkFile(logFile);

	char c = 0;
	while((c = getc(wordInputFile)) != EOF) {
		if(c == '\n') {
			totalWords++;		//increments lines with every new line, to get the amount of lines/words
		}
	}

	printf("\nWelcome to HANGMAN!\n\n> How to play:\nEnter a letter, until you have guessed the secret word.\n");
	printf("The whole word can be guessed aswell, but beware!\nIf your guess is incorrect, it's game over!\n");
	printf("You may enter up to 6 incorrect guesses before the gallow inevitably hangs you...\n\n");

	while(gameLoop) {
		position = 0;
		pickWord(&wordLength, wordInputFile, totalWords, &wordToGuess, &blanks, &guessingBank);

		if(roundsPlayed == totalWords) {
			printf("\n\nAll supplied words have been played before.\n"
				"Please start a new game, if you want to play again!\n");
			exit(0);
		}

		if(wordAlreadyUsed(totalWords, wordToGuess)) {
			continue;
		}

		fprintf(logFile, "\n_________________________________\n");
		fprintf(logFile, "\nGame has started.\n");

		time(&currentTime); //set current time
		fprintf(logFile, "Time: %s\n", ctime(&currentTime)); //ctime converts currentTime to string

		while(position < LIVES) {
			printBoard(position, logFile, board[position], blanks, guessingBank);
			char guess = readUserGuess(logFile, wordToGuess);
			if(guess == 0) { //if user guesses whole word (right or wrong, doesn't matter), break out of loop and ask to play again
				break;
			}

			if(isAlreadyGuessed(position, guess, guessingBank)) {
				continue;
			} else if (checkUserGuess(wordLength, guess, wordToGuess, blanks)) {
				if(checkIfWon(logFile, wordToGuess, blanks)) {
					break;
				}
			} else {
				guessingBank[position++] = guess;
			}
		}

		if(position >= LIVES) {
			printf("\nGame over!\nThe word was %s.\n\n", wordToGuess);
			fprintf(logFile, "\nGame over!\nThe word was %s.\n\n", wordToGuess);
			fprintf(logFile, "\nGame has ended.\n");

			time(&currentTime);
			fprintf(logFile, "Time: %s\n", ctime(&currentTime));
		}

		if(playAgain()) {
			roundsPlayed++;
			gameLoop = 1;
		} else {
			gameLoop = 0;
		}
	}

	printf("\n\nThank you for playing!\n");
	fclose(wordInputFile);
	fclose(logFile);

	free(wordToGuess);
	free(blanks);
	free(guessingBank);

	return 0;
}
