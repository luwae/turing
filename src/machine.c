#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STATETAB_SIZE(nsym, nstate) \
    sizeof(struct command) * nstate * nsym

struct command {
    uint8_t sym;
    int8_t move;
    uint16_t next;
};

struct tmachine {
    int nsyms;
    int nstates;
    
    uint8_t *sym2num;
    char *num2sym;
    char **statenames;
    
    struct command *statetab;
};

int main() {
    unsigned char data[] = {
        0x00, 0xFF, 0x01, 0x00, //state 0(s), symbol 0(_)
        0x01, 0x01, 0x00, 0x00, //state 0(s), symbol 1(0)
        0x02, 0x01, 0x00, 0x00, //state 0(s), symbol 2(1)
        0x02, 0x00, 0xFF, 0xFF, //state 1(a), symbol 0(_)
        0x02, 0x00, 0xFF, 0xFF, //state 1(a), symbol 1(0)
        0x01, 0xFF, 0x01, 0x00  //state 1(a), symbol 2(1)
        };
    
    uint8_t sym2num[128] = { //make this smaller?
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        };
        
    char num2sym[3] = {
        '_', '0', '1'
        };
    char *statenames[2] = {"s", "a"};
    
    struct tmachine tm;
    tm.nsyms = 3;
    tm.nstates = 2;
    tm.sym2num = sym2num;
    tm.num2sym = num2sym;
    tm.statenames = statenames
    tm.statetab = (struct command *) data;
    return 0;
}
