#include "chell.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"

chell_state_t *new_chell()
{
    chell_state_t *state = malloc(sizeof(chell_state_t));

    chell_set_prompt(state, NULL);

    return state;
}

void delete_chell(chell_state_t **state)
{
    free(*state);
}

void chell_set_prompt(chell_state_t *state, const char *prompt)
{
    if (prompt == NULL || strlen(prompt) + 1 /*null byte*/ >= CHELL_PROMPT_ARR_LENGTH)
    {
        strcpy(state->prompt, "308sh> ");
    }
    else
    {
        strcpy(state->prompt, prompt);
    }
}