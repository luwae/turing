#ifndef MACHINE_H
#define MACHINE_H

#include <stdint.h>

#include "translate.h"
#include "tape.h"

#define MOVE_L '<'
#define MOVE_R '>'
#define STATE_END 0xffff

/**
 * A single command in a single state.
 * Specifies what symbol shall be written, whether to move, and next state.
 * STATE_END in next specifies that the turing machine shall halt.
 * This is the default entry for all commands.
 */
struct command {
    char sym;
    char move;
    uint16_t next;
};

/**
 * The turing machine structure.
 * Contains size information, character translation, state names,
 * state information, and a tape.
 */
struct tmachine {
    int nsyms;
    int nstates;
    
    char *n2c;
    uint8_t *c2n;
    char **statenames;
    
    struct command *statetab;
    struct tape *mtape;
};

struct tmachine *tmachine_new(int nsyms, int nstates);
void tmachine_free(struct tmachine *tm);

uint16_t tmachine_find_state(struct tmachine *tm, char *statename);
void tmachine_set_command(struct tmachine *tm, char *statename, char insym, char newsym, char move, uint16_t newstate);
#define tm_command(tm, statenum, insym) \
    (tm->statetab + nsyms * statenum + *(tm->translator + insym))

#endif
