#include "chell.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/wait.h>


chell_state_t *new_chell()
{
    chell_state_t *state = malloc(sizeof(chell_state_t));

    chell_set_prompt(state, NULL);
    state->lineCapacity = CHELL_INITIAL_LINE_CAPACITY;
    state->currentLine = malloc(CHELL_INITIAL_LINE_CAPACITY);
    strcpy(state->currentLine, "");
    return state;
}

void delete_chell(chell_state_t **state)
{
    free((*state)->currentLine);
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

void readline(chell_state_t *state)
{
    getline(&(state->currentLine), &(state->lineCapacity), stdin);
}

void executeline(chell_state_t *state) 
{
    char* currentline = strdup(state->currentLine);
    char* program = strtok(currentline, " ");

    char* args[1];
    args[0] = NULL;

    pid_t processId = fork();


    if (processId == 0) {
        execvp(program, args);
    } else {
        wait(NULL);
    }

    free(currentline);
}