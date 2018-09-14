#include <stdio.h>

#include "src/chell.h"
#include "unistd.h"
#include "getopt.h"
#include "stdlib.h"
#include "string.h"

void set_commandline_options(chell_state_t *state, int argc, char **argv);

int main(int argc, char **argv)
{
    chell_state_t *chell = new_chell();
    set_commandline_options(chell, argc, argv);

    while (strcmp(chell->currentLine, "exit\n") != 0)
    {
        printf("%s", chell->prompt);
        readline(chell);
        executeline(chell);
        // printf("%s", chell->currentLine);
    }

    delete_chell(&chell);
    return 0;
}

void set_commandline_options(chell_state_t *state, int argc, char **argv)
{
    int c;

    while ((c = getopt(argc, argv, "p:")) != -1)
    {
        switch (c)
        {
        case 'p':
            chell_set_prompt(state, optarg);
            break;
        default:
            abort();
        }
    }
}