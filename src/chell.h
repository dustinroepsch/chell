#ifndef CHELL_H
#define CHELL_H

#define CHELL_PROMPT_ARR_LENGTH 100 /* Max number of characters the prompt can be, for the -p flag. */

#define CHELL_INITIAL_LINE_CAPACITY 16 /* The starting capacity for calls to getline, but the currentLine array will grow if needed */

#define CHELL_MAX_PROGRAM_ARGS 100 /* Maximum number of arguments a call to execvp can pass. There's no way someone will type a command with more than 100 arguements, and this is much less messy then dynamically regrowing the array*/

#define CHELL_ARG_DELIM " \n" /* The delimeters used to split the line typed by the user*/

#define CHELL_MAX_CHILDREN 10000 /* The max number of child processes chell can have*/

#include <stdio.h>
#include <sys/types.h>

typedef struct
{
    char prompt[CHELL_PROMPT_ARR_LENGTH];
    char *currentLine;
    size_t lineCapacity;
    int shouldExit;
    pid_t children[CHELL_MAX_CHILDREN];
    int numChildren;
} chell_state_t;

chell_state_t *new_chell();

void delete_chell(chell_state_t **state);

void chell_set_prompt(chell_state_t *state, const char *prompt);

void readline(chell_state_t *state);

void executeline(chell_state_t *state);

void check_if_background_child_exited(chell_state_t *state);
#endif