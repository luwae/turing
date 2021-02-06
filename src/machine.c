/**
 * Implementation of the turing machine structure.
 * @author Lucas Waeldele
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <tape.h>
#include <arena.h>

/**
 * Size of all states in the machine combined, in bytes.
 */
#define STATETAB_SIZE(nsyms, nstates) \
    sizeof(struct command) * nstates * nsyms

/**
 * A single command in a single state.
 * Specifies what symbol shall be written, whether to move, and next state.
 */
struct command {
    char sym;
    char move;
    uint16_t next;
};

/**
 * The turing machine structure.
 * Contains size information, state names, state information, and a tape.
 */
struct tmachine {
    int nsyms;
    int nstates;
    
    uint8_t *translator;
    char **statenames;
    
    struct command *statetab;
    struct tape *mtape;
};

/**
 * Create a new turing machine in the memory arena.
 * @param nsyms the number of symbols this turing machine recognizes.
 * @param nstates the number of states this turing machine has.
 * @return pointer to the new turing machine
 */
struct tmachine *tmachine_create(int nsyms, int nstates) {
    struct tmachine *tm = aalloc(sizeof(*tm));

    tm->nsyms = nsyms;
    tm->nstates = nstates;
    tm->mtape = tape_create_empty();
        
    return tm;
}

/* test */
#ifdef DEBUG

/**
 * Create an example turing machine, which adds one to a given binary number.
 */
int main() {
    ainit();
    struct tmachine *tm = tmachine_create(3, 2);
    tm->translator = translator_create("_01");
    char *statenames[] = {"smove", "sadd"};
    tm->statenames = statenames;

    afree();
}
#endif
