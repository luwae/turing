#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <tape.h>

#define STATETAB_SIZE(nsyms, nstates) \
    sizeof(struct command) * nstates * nsyms

struct command {
    uint8_t sym;
    int8_t move;
    uint16_t next;
};

struct tmachine {
    int nsyms;
    int nstates;
    
    char **statenames;
    
    uint8_t *sym2num;
    char *num2sym;
    
    struct command *statetab;
    struct tape *mtape;
};

//--------------------
int test_nsyms = 3;
int test_nstates = 2;
char *_test_statenames = "s\0a";
char *test_statenames[2] = {&_test_statenames, &_test_statenames + 2};

uint8_t test_sym2num[] = {
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    1   ,2   ,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0   ,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
char *test_num2sym = "_01";
uint8_t _test_statetab[] = {
0x00, 0x00, 0x01, 0x00,
0x01, 0x02, 0x00, 0x00,
0x02, 0x02, 0x00, 0x00,
0x02, 0x01, 0xFF, 0xFF,
0x02, 0x01, 0xFF, 0xFF,
0x01, 0x00, 0x01, 0x00
};
struct command *test_statetab = (struct command *) _test_statetab;
char *test_start_string = "10011";
//--------------------

struct tmachine *tmachine_create(int nsyms, int nstates) {
    struct tmachine *tm = malloc(sizeof(*tm));

    tm->nsyms = nsyms;
    tm->nstates = nstates;
    tm->mtape = tape_create_empty();
        
    return tm;
}

void tmachine_destroy(struct tmachine *tm) {
    free(tm->statenames);
    free(tm->statetab);
    tape_destroy(tm->mtape);

    free(tm);
}

int main() {
    struct tmachine *tm = tmachine_create(test_nsyms, test_nstates);
    tm->statenames = test_statenames;
    tm->sym2num = test_sym2num;
    tm->num2sym = test_num2sym;
    tm->statetab = test_statetab;
    tape_write_string(tm->mtape, test_start_string, test_sym2num);

    tmachine_destroy(tm);
    return 0;
}
