#include <stdio.h>
#include <string.h>

#include "../src/tape.h"

#define ASSERT(c, msg) if (!(c)) {printf("Test failed: %s\n", msg); return -1;}
#define ASSERT_JMP(c, msg, jmp) if (!(c)) {printf("Test failed: %s\n", msg); goto jmp;}

uint8_t sym2num[] = {
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    1   ,2   ,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0   ,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
char *num2sym = "_01";

/* Various tests.
 * test if allocation and initialization works as expected
 * test write_string
 * test if automatic size calculation works
 */
int tape_test_alloc_grow_delete() {
    struct tape *t = tape_create_empty();
    char buf[32];

    ASSERT(t != NULL, "allocation error")
    ASSERT_JMP(t->l.size == 0 && t->r.size == 0, "tape not initialized correctly", FAIL1)

    tape_write_string(t, "0100101_01___1", sym2num);
    tape_write(t, 0, 13);
    tape_write(t, 2, -5);

    tape_sprint(t, buf, num2sym);
    ASSERT_JMP(strcmp(buf, "1____0100101_01") == 0, "unexpected tape content", FAIL1)

    tape_destroy(t);
    return 0;
FAIL1:
    tape_destroy(t);
    return -1;
}

/* Test array growth.
 * array must have at least range (last index + 1) rounded up to next power of two
    - if last index is 63, array must at least have range 64
    - if last index is 64, array must at least have range 128
   array is only resized if the size is lower or equal to 1/4 of the range
    - if last range was 256 and new size is 64, array should shrink to range 128
 */
int tape_test_resize() {
    struct tape *t = tape_create_empty();
    
    tape_write(t, 1, 63);
    tape_write(t, 1, 64);
    tape_write(t, 1, 128);
    ASSERT_JMP(t->r.range == 256, "range error: on index 128, range should be 256", FAIL2);
    ASSERT_JMP(t->r.size == 129, "size error: after write to 128, size should be 129", FAIL2);
    tape_write(t, 0, 128);
    ASSERT_JMP(t->r.range == 256, "range error: on last index 64 after removing 128, range should still be 256", FAIL2);
    tape_write(t, 0, 64);
    ASSERT_JMP(t->r.range == 128, "range error: on last index 63, range should shrink to 128", FAIL2);
    
    tape_destroy(t);
    return 0;
FAIL2:
    tape_destroy(t);
    return -1;
}

/* Test array growth.
 * array should not grow if a 0 is written, as it is useless information
 * array should have minimum size of DYNARR_SIZE_MIN (see src/tape.h)
 */
int tape_test_noresize() {
    struct tape *t = tape_create_empty();
    
    tape_write(t, 1, 10);
    tape_write(t, 0, 10);
    ASSERT_JMP(t->r.range == DYNARR_SIZE_MIN, "range error: on empty tape, range should be DYNARR_SIZE_MIN", FAIL3);
    tape_write(t, 0, 10);
    ASSERT_JMP(t->r.range == DYNARR_SIZE_MIN, "range error: range should not change on empty write", FAIL3);
    
    tape_destroy(t);
    return 0;
FAIL3:
    tape_destroy(t);
    return -1;
}

int main() {
    if (tape_test_alloc_grow_delete() || tape_test_resize() || tape_test_noresize())
        printf("Tests failed.\n");
    else
        printf("Tests successful.\n");

    return 0;
}
