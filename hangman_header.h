
#ifndef HANGMAN_HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#define HANGMAN_HEADER_H_
#define LIVES 7 				//theoretically changeable, but stick figure graphic would not work anymore
#define MAXCHARACTERS 64		//max. characters to read in from user input/for words
#define MAXFILENAMELENGTH 101	//max. length of file name
#define MAXSAVEDWORDS 100		//max. amount of saved already used words

/*!
 * \brief Checks, if a provided file was opened successfully.
 * \param *pFile Word input file
 * \details If file pointer is NULL, program exits with unsuccessful termination.
 */
void checkFile(FILE *pFile);
/*!
 * \brief Scans and checks user guess and returns it to main.
 * \param *pLog Log file
 * \param *wordToGuess Secret Hangman word
 * \details Asks the user to enter a letter and capitalizes input.
 * Writes current time into log file.
 *
 * 4 options:
 * 1. If user manages to guess the whole word, they instantly win the round and the game restarts.
 * 2. If user inputs more than 1 character and fail to guess the secret word it's "Game over".
 * 3. If user input is 1 alphabetic character, the entered character is returned.
 * 4. If user input is 1 non-alphabetic character a message appears and the function restarts.
 * \return guess[0] = 0 with option 1. and 2. - game restarts in main()
 * \return guess[0] if user enters an alphabetic character
 * \return checkUserGuess() if user enters non-alphabetic character
 */
char readUserGuess(FILE *pLog, char *wordToGuess);
/*!
 * \brief Checks with subfunction isInWord(), if entered letter is part of the guessing bank of incorrect letters.
 * \param position Counter for incorrectly entered letters
 * \param guess User guess
 * \param *guessingBank Saves incorrectly guessed letters
 * \return 1 If entered letter is in guessing bank/already guessed
 * \return 0 If entered letter is not in guessing bank/already guessed
 */
int isAlreadyGuessed(int position, char guess, char *guessingBank);
/*!
 * \brief Checks, if entered letter is in secret word and replaces blank with correctly entered letter.
 * \param wordLength Length of secret word
 * \param guess User guess
 * \param *wordToGuess Secret Hangman word
 * \param *blanks Saves blanks and correctly guessed letters
 * \return 1 If entered letter is in secret word
 * \return 0 If entered letter is not in secret word
 */
int checkUserGuess(int wordLength, char guess, char *wordToGuess, char *blanks);
/*!
 * \brief Checks, if the player has won the game
 * \param *pLog Log file
 * \param *wordToGuess Secret Hangman word
 * \param *blanks Saves blanks and correctly guessed letters
 * \details If the secret word and all entered letters (that represent the secret word) match, the user wins.
 * Prints a winning message to stdout and the current time and secret word into the log file.
 * \return 1 If user won the game
 * \return 0 If user has not yet won the game
 */
int checkIfWon(FILE *pLog, char *wordToGuess, char *blanks);
/*!
 * \brief Prints the stick figure and the game board.
 * \param position Counter for incorrectly entered letters
 * \param *pLog Log file
 * \param *board Hangman stick figure
 * \param *blanks Saves blanks and correctly guessed letters
 * \param *guessingBank Saves incorrectly guessed letters
 * \details Prints stick figure to stdout and game board to stdout and log file.
 * Calls function printGuessingBank() to print incorrect letters to stdout
 */
void printBoard(int position, FILE *pLog, char *board, char *blanks, char *guessingBank);
/*!
 * \brief Picks a random word from a provided list of words
 * \param *wordLength Length of the chosen word
 * \param *pWordFile File with list of words
 * \param *totalWords Total words read in from file
 * \param **wordToGuess Secret Hangman word
 * \param **blanks Saves blanks and correctly guessed letters
 * \param **guessingBank Saves incorrectly guessed letters
 * \details Randomly chooses a word by selecting a random number between 1 and totalWords.
 * Reads the amount of characters of chosen secret word in and allocates memory for the secret word,
 * blanks and the guessing bank. Scans the word from the file and capitalizes it.
 * Lastly initializes blanks and guessing bank arrays.
 */
void pickWord(int *wordLength, FILE *pWordFile, int totalWords, char **wordToGuess, char **blanks, char **guessingBank);
/*!
 * \brief Asks the user, if they want to play again.
 * \details Reads in user input after the question if they want to play again.
 *
 * 3 options:
 * 1. Y to play again - Resets game and restarts game loop.
 * 2. N to quit - Exits the game.
 * 3. Invalid input - Message appears to enter valid input.
 * \return 1 if Y
 * \return 0 if N
 * \return playAgain() if user entered invalid input
 */
int playAgain();
/*!
 * \brief Checks, if entered letter is part of the guessing bank of incorrect letters.
 * Subfunction of isAlreadyGuessed()
 * \param position Counter for incorrectly entered letters
 * \param guess User guess
 * \param *guessingBank Saves incorrectly guessed letters
 * \return 1 If entered letter is in guessing bank/already guessed
 * \return 0 If entered letter is not in guessing bank/already guessed
 */
int isInWord(int position, char guess, char *guessingBank);
/*!
 * \brief Prints incorrectly guessed letters ("guessing bank") to stdout
 * \param *guessingBank Saves incorrectly guessed letters
 */
void printGuessingBank(char *guessingBank);
/*!
 * \brief Checks, if a selected word has been chosen already and resets the process
 * \param *wordToGuess Secret Hangman word
 * \return 1 If a word has been selected already
 * \return 0 If a word has not yet been selected
 */
int wordAlreadyUsed(int totalWords, char *wordToGuess);

#endif /* HANGMAN_HEADER_H_ */
