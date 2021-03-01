This project represents the classic game "Hangman".
The player is provided a secret word and tries to guess it by suggesting letters within a certain number of guesses.

> COMPILATION

Linked libraries:
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

> EXECUTION

To start the game supply argv[1] with the path to a word list with the following format:
word1
word2
word3

This means word, newline, word, newline and so on.
Pass argv[2] the path to a destination for the created log file while playing.

The program is pretty straight forward after it has been started.
A random word from the supplied word list will be chosen. Guess one letter at a time
or the whole word until the secret word is found or you loose all lives.

> COMPLETED TASKS

Task 1, 2, 3, 4

Additional features: Drawn Hangman stick figure printed out each round
