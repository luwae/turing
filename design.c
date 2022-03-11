struct action {
    char sym;
    int move;
    void *state;
}

struct state {
    char *name;
    struct state *parent;
    struct action actions[];
}

0x7fTMB
