#ifndef CHELL_H
#define CHELL_H

#define CHELL_PROMPT_ARR_LENGTH 100
#define CHELL_INITIAL_LINE_CAPACITY 16
#define CHELL_MAX_PROGRAM_ARGS 100
#define CHELL_ARG_DELIM " \n"
#define CHELL_MAX_CHILDREN 10000

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