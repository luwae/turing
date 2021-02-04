#include <stdio.h>
#include <string.h>

#include "../src/dynarr.h"

#define ASSERT(c, msg) if (!(c)) {printf("Test failed: %s\n", msg); return -1;}
#define ASSERT_JMP(c, msg, jmp) if (!(c)) {printf("Test failed: %s\n", msg); goto jmp;}

//TODO: dynarr has not been checked for memory leaks
//TODO: this test only works for block size 256

#define LAST (BLOCK_SIZE - 1)

int test_init() {
    struct dynarr d;
    char buf[100];
    char *exp1 = "\0";
    char *exp2 = "_____1";
    char *exp3 = "_____1__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________1";
    dynarr_init(&d);
    ASSERT_JMP(*(d.blocks), "allocated null pointer block", FAIL1);

    ASSERT_JMP(dynarr_read(&d, LAST), "not filled with blanks", FAIL1);

    dynarr_sprint(&d, buf);
    ASSERT_JMP(strcmp(buf, exp1) == 0, "wrong buf after init", FAIL1);

    dynarr_write(&d, '1', 5);
    dynarr_sprint(&d, buf);
    ASSERT_JMP(strcmp(buf, exp2) == 0, "wrong buf after write 5", FAIL1);

    dynarr_write(&d, '1', BLOCK_SIZE);
    dynarr_sprint(&d, buf);
    ASSERT_JMP(d.nblocks == 2, "wrong nblocks after write", FAIL1);
    ASSERT_JMP(d.size == 256, "wrong size after write", FAIL1);
    ASSERT_JMP(strcmp(buf, exp3) == 0, "wrong buf after 256", FAIL1);

    dynarr_free(&d);
    return 0;
FAIL1:
    dynarr_free(&d);
    return -1;
}

int main() {
    return 0;
}
