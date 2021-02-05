/**
 * Implementation of the turing machine structure.
 * @author Lucas Waeldele
 */

/**
 *
 * state s1:
 * struct command[nsyms]
 *
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <tape.h>

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
    
    char **statenames;
    
    struct command *statetab;
    struct tape *mtape;
};

/**
 * Create a new turing machine on the heap.
 * @param nsyms the number of symbols this turing machine recognizes.
 * @param nstates the number of states this turing machine has.
 * @return pointer to the new turing machine
 */
struct tmachine *tmachine_create(int nsyms, int nstates) {
    struct tmachine *tm = malloc(sizeof(*tm));

    tm->nsyms = nsyms;
    tm->nstates = nstates;
    tm->mtape = tape_create_empty();
        
    return tm;
}

/**
 * Free all allocated resources of a turing machine.
 * @param tm self
 */
void tmachine_destroy(struct tmachine *tm) {
    tape_destroy(tm->mtape);

    free(tm);
}

#ifdef DEBUG
int main() {
    
}
#endif
