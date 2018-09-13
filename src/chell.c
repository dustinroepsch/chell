#include "chell.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"

chell_state_t *new_chell(const char *prompt)
{
    chell_state_t *state = malloc(sizeof(chell_state_t));

    if (prompt == NULL || strlen(prompt) + 1 /*null byte*/ >= CHELL_PROMPT_ARR_LENGTH)
    {
        strcpy(state->prompt, "308sh> ");
    }
    else
    {
        strcpy(state->prompt, prompt);
    }
    return state;
}

void delete_chell(chell_state_t **state) {
    free(*state);
}