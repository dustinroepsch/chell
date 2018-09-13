#include <stdio.h>

#include "src/chell.h"

int main() {
    chell_state_t* chell = new_chell(NULL);
    printf("%s", chell->prompt);
    delete_chell(&chell);
    return 0;
}