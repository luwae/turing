#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef uint8_t u8;
typedef int8_t i8;
typedef uint32_t u32;

#define L (-1)
#define R (1)

struct state;

struct action {
    u8 start;
    u8 len;
    
    u8 sym;
    i8 move;
    u32 next;
};

struct state {
    union {
        u8 nactions;
        u32 namep;
    } info;
    u32 parent;
    struct action actions[];
};

char _move_right[sizeof(struct state) + 2 * sizeof(struct action)];
char _move_left[sizeof(struct state) + 3 * sizeof(struct action)];
struct state *move_right = (struct state *) _move_right;
struct state *move_left = (struct state *) _move_left;

struct action *find_action(struct state *s, char c) {
    for (int i = 0; i < s->nactions; i++) {
        struct action *a = s->actions + i;
        if (a->start == 0) {
            return a;
        }
        if (c >= a->start && (c - a->start) < a->len) {
            return a;
        }
    }
    return NULL;
}

int main() {
    move_right->nactions = 2;
    move_right->name = "move_right";
    move_right->parent = NULL;
    move_right->actions[0].start = '_';
    move_right->actions[0].len = 1;
    move_right->actions[0].sym = 0;
    move_right->actions[0].move = L;
    move_right->actions[0].next = move_left;
    move_right->actions[1].start = 0;
    move_right->actions[1].len = 0;
    move_right->actions[1].sym = 0;
    move_right->actions[1].move = R;
    move_right->actions[1].next = move_right;
    
    move_left->nactions = 3;
    move_left->name = "move_left";
    move_left->parent = NULL;
    move_left->actions[0].start = '0';
    move_left->actions[0].len = 1;
    move_left->actions[0].sym = '1';
    move_left->actions[0].move = 0;
    move_left->actions[0].next = NULL;
    move_left->actions[1].start = '1';
    move_left->actions[1].len = 1;
    move_left->actions[1].sym = '0';
    move_left->actions[1].move = L;
    move_left->actions[1].next = move_left;
    move_left->actions[2].start = '_';
    move_left->actions[2].len = 1;
    move_left->actions[2].sym = '1';
    move_left->actions[2].move = 0;
    move_left->actions[2].next = NULL;
    
    char tape[65];
    for (int i = 0; i < 64; i++) {
        tape[i] = '_';
    }
    tape[65] = '\0';
    char *p = tape + 32;
    memcpy(p, "10011", 5);
    
    struct state *s = move_right;
    
    printf("%s\n", tape);
    while (s) {
        struct action *a = find_action(s, *p);
        if (a == NULL) {
            break;
        }
        printf("%c %c %s\n", a->sym, a->move == R ? 'R' : a->move == L ? 'L' : '0', a->next ? a->next->name : "END");
        if (a->sym)
            *p = a->sym;
        p += a->move;
        s = a->next;
    }
    printf("%s\n", tape);
}

/*
 * 0x7fTMB
 */
