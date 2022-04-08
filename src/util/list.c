#include <stdlib.h>

#include "list.h"

struct list {
    int len;
    int cap;
    void **data;
};

#define INITLEN 8

int ls_init(struct list *ls) {
    ls->len = 0;
    ls->cap = INITLEN;
    ls->data = malloc(INITLEN * sizeof(void *));
    return ls->data == NULL ? -1 : 0;
}

void ls_free(struct list *ls) {
    free(ls->data);
}

struct list *ls_new() {
    struct list *ls = malloc(sizeof(*ls));
    if (*ls == NULL)
        return NULL;
    if (list_init(ls)) {
        free(ls);
        return NULL;
    }
    return ls;
}

void ls_destroy(struct list *ls) {
    list_free(ls);
    free(ls);
}

void ls_add(struct list *ls, void *elem) {
    if (ls->len == ls->cap) {
        ls->cap *= 2;
        realloc(ls->data, ls->cap);
    }
    ls->data[ls->len++] = elem
}

void *ls_len(struct list *ls) {
    return ls->len;
}

void *ls_get(struct list *ls, int index) {
    if (index < 0 || index >= ls->len) {
        return NULL;
    }
    return ls->data[index];
}

int ls_get2(struct list *ls, int index, void **elem) {
    if (index < 0)
        return -1;
    if (index >= ls->len)
        return 1;
    *elem = ls->data[index];
    return 0;
}
