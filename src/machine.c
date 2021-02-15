/**
 * Turing machine structure.
 * @author LuWae
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "machine.h"

/**
 * Create an empty turing machine.
 * The machine is filled with error states that can be selectively overwritten.
 * Statenames and state transitions have to be set somewhere else.
 * @param nstates number of states of the turing machine, excluding error state
 * @param chars the chars the machine will recognize. BLANK must be included.
 * @return new turing machine, NULL on error
 */
struct tmachine *tmachine_new(int nstates, char *chars) {
    struct tmachine *tm = malloc(sizeof(*tm));
    
    if (strchr(chars, BLANK) == NULL) {
        ERROR("the turing machine must recognize blanks");
        free(tm);
        return NULL;
    }
    tm->nsyms = strlen(chars);
    tm->nstates = nstates;
    tm->translator = translator_new(chars);
    
    size_t statenames_size = nstates * sizeof(char *); 
    tm->statenames = malloc(statenames_size);
    memset(tm->statenames, 0, statenames_size);
    
    size_t statetab_size = tm->nsyms * nstates * sizeof(struct command);
    tm->statetab = malloc(statetab_size);
    memset(tm->statetab, TRANSLATOR_MISS, statetab_size); //fill with error states
    
    tm->mtape = tape_new();
    
    return tm;
}

/**
 * Free turing machine.
 * @param tm self
 */
void tmachine_free(struct tmachine *tm) {
    translator_free(tm->translator);
    for (int i = 0; i < tm->nstates; i++)
        free(*(statenames + i));
    free(statenames);
    free(statetab);
    tape_free(tm->mtape);
}

/**
 * Find a state number by name.
 * This function will only be called at compile time.
 * @param tm self
 * @param statename the name of the requested state
 */
uint16_t tmachine_find_state(struct tmachine *tm, char *statename) {
    for (int i = 0; i < tm->nstates; i++)
        if (strcmp(*(tm->statenames + i), statename) == 0)
            return i;
    return STATE_END;
}

/**
 * Set a command in the turing machine.
 * @param tm self
 * @param statename the state containing the command
 * @param insym the symbol that leads to the command
 * @param newsym the symbol that should be written
 * @param move the movement action of the turing machine
 * @param newstate the state to switch to after the command is executed
 */
void tmachine_set_command(struct tmachine *tm, char *statename, char insym, char newsym, char move, uint16_t newstate) {
    struct command *c = tm_command(tm, tmachine_find_state(tm, statename), insym);
    c->sym = newsym;
    c->move = move;
    c->state = newstate;
}

void tmachine_print(struct tmachine *tm) {
    printf("tmachine @%p:\n", tm);
    printf("  nsyms=%d, nstates=%d\n", tm->nsyms, tm->nstates);
    printf("  states:\n");
    
    for (int nst = 0; nst < tm->nstates; nst++) {
        printf("    state %s:\n", *(tm->statenames + nst));
        char currentsym;
        struct command *currentcommand;
        for (int i = 0; (currentsym = *(tm->n2c + i)); i++) {
            currentcommand = tm_command(tm, nst, currentsym);
            printf("      %c: %c %c %s", currentsym, currentcommand->sym == )
        }
    }
}

/* test */
#ifdef DEBUG
int main() {
    struct tmachine *tm = tmachine_new(2, "_01");
    
}
#endif
