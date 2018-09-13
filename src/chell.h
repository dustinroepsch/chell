#ifndef CHELL_H
#define CHELL_H

#define CHELL_PROMPT_ARR_LENGTH 100

typedef struct
{
    char prompt[CHELL_PROMPT_ARR_LENGTH];

} chell_state_t;

chell_state_t *new_chell();

void delete_chell(chell_state_t **state);

void chell_set_prompt(chell_state_t *state, const char *prompt);

#endif