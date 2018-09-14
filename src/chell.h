#ifndef CHELL_H
#define CHELL_H

#define CHELL_PROMPT_ARR_LENGTH 100
#define CHELL_INITIAL_LINE_CAPACITY 16

#include "stdio.h"

typedef struct
{
    char prompt[CHELL_PROMPT_ARR_LENGTH];
    char *currentLine;
    size_t lineCapacity;
} chell_state_t;

chell_state_t *new_chell();

void delete_chell(chell_state_t **state);

void chell_set_prompt(chell_state_t *state, const char *prompt);

void readline(chell_state_t *state);

void executeline(chell_state_t *state);

#endif