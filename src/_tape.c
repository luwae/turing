#include <stdio.h>
#include <stdlib.h>

struct tape {
    struct dynarr l;
    struct dynarr r;
};


struct tape *tape_create() {
    struct tape *t = malloc(sizeof(*t));
    
    dynarr_init(&(t->l));
    dynarr_init(&(t->r));

    return t;
}

void tape_destroy(struct tape *t) {
    dynarr_free(&(t->l));
    dynarr_free(&(t->r));
}

void tape_write(struct tape *t, char data, int index) {
}
