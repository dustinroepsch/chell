#include "chell.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libgen.h>
#include <ctype.h>

chell_state_t *new_chell()
{
    chell_state_t *state = malloc(sizeof(chell_state_t));

    chell_set_prompt(state, NULL);
    state->lineCapacity = CHELL_INITIAL_LINE_CAPACITY;
    state->currentLine = malloc(CHELL_INITIAL_LINE_CAPACITY);
    strcpy(state->currentLine, "");
    state->shouldExit = 0;
    state->numChildren = 0;
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

void remove_trailing_whitespace(char *str)
{
    const int length = strlen(str);
    int i = length - 1;
    while (i >= 0 && isspace(str[i]))
    {
        str[i] = 0;
        i--;
    }
}

void readline(chell_state_t *state)
{
    getline(&(state->currentLine), &(state->lineCapacity), stdin);
    remove_trailing_whitespace(state->currentLine);
}

int attempt_built_in_command(const char *program, char **const args, chell_state_t *state)
{
    if (strcmp(program, "exit") == 0)
    {
        state->shouldExit = 1;
        return 1;
    }
    if (strcmp(program, "pid") == 0)
    {
        printf("%d\n", getpid());
        return 1;
    }
    if (strcmp(program, "ppid") == 0)
    {
        printf("%d\n", getppid());
        return 1;
    }
    if (strcmp(program, "cd") == 0)
    {
        if (args[1] == NULL)
        {
            chdir(getenv("HOME"));
        }
        else
        {
            if (chdir(args[1]) == -1)
            {
                printf("Cannot find directory %s\n", args[1]);
            }
        }
        return 1;
    }
    if (strcmp(program, "pwd") == 0)
    {
        char *dir = getcwd(NULL, 0);
        printf("%s\n", dir);
        free(dir);
        return 1;
    }
    if (strcmp(program, "set") == 0)
    {
        if (args[1] == NULL || args[2] == NULL)
        {
            printf("Set takes two arguements\n");
            return 1;
        }
        setenv(args[1], args[2], 1);
        return 1;
    }

    if (strcmp(program, "get") == 0)
    {
        if (args[1] == NULL)
        {
            printf("get takes one arguement\n");
            return 1;
        }
        char *val = getenv(args[1]);
        if (val == NULL)
        {
            printf("Unkown environment variable %s\n", args[1]);
            return 1;
        }
        printf("%s\n", val);
        return 1;
    }

    return 0;
}

void print_exit_info(pid_t pid, int status)
{
    if (WIFEXITED(status))
    {
        printf("Process %d terminated normally\n", pid);
    }
    else if (WIFSIGNALED(status))
    {
        printf("Process %d terminated due to uncaught signal.\n", pid);
    }
    else if (WIFSTOPPED(status))
    {
        printf("Process %d stopped.\n", pid);
    }
    else if (WIFCONTINUED(status))
    {
        printf("Process %d continued.\n", pid);
    }
    else if (WEXITSTATUS(status))
    {
        printf("Process %d terminated with exit status \n", WEXITSTATUS(status));
    }
}

int should_run_in_background(char *line)
{
    int len = strlen(line);
    if (len == 0)
    {
        return 0;
    }
    if (line[len - 1] == '&')
    {
        line[len - 1] = 0;
        remove_trailing_whitespace(line);
        return 1;
    }
    return 0;
}

void executeline(chell_state_t *state)
{
    char *currentline = strdup(state->currentLine);
    int run_in_background = should_run_in_background(currentline);

    char *program = strtok(currentline, CHELL_ARG_DELIM);
    char *filename = strdup(program);

    char *args[CHELL_MAX_PROGRAM_ARGS];
    args[0] = basename(filename);

    int numArgs = 1;

    while (numArgs < CHELL_MAX_PROGRAM_ARGS - 1 /*to save room for the NULL pointer*/
           && ((args[numArgs] = strtok(NULL, CHELL_ARG_DELIM)) != NULL))
    {
        numArgs++;
    }

    args[numArgs] = NULL; /* Required for execvp to know how many args there are*/

    if (attempt_built_in_command(program, args, state))
    {
        goto cleanup; /*can't just return because we need to free some pointers */
    }

    pid_t processId = fork();

    if (processId == 0)
    {
        printf("Process: %d\n", getpid());
        if (-1 == execvp(program, args))
        {
            printf("Unknown executable %s\n", program);
            /* exit program with cleanup */
            state->shouldExit = 1;
            goto cleanup;
        }
    }
    else
    {
        if (!run_in_background)
        {
            int status;
            pid_t pid = waitpid(processId, &status, 0);
            print_exit_info(pid, status);
        }
        else
        {
            state->children[state->numChildren] = processId;
            state->numChildren++;
            /*sleep so that the child has a chance to print its process info before the next chell line is printed*/
            usleep(100);
        }
    }

cleanup:
    free(filename);
    free(currentline);
}

void check_if_background_child_exited(chell_state_t *state)
{
    if (state->numChildren > 0)
    {
        int status;
        pid_t pid = waitpid(-1, &status, WNOHANG);
        if (pid > 0)
        {
            print_exit_info(pid, status);
            state->numChildren--;
        }
    }
}