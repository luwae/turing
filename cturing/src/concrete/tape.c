#include <stdio.h>
#include "tape.h"

int Tape_init(Tape *t, const char *s) {
    uchar_vec_init(&t->left);
    uchar_vec_init(&t->right);
    unsigned char *ptr;
    while (*s) {
        if ((ptr = uchar_vec_pushresv(&t->right)))
            *ptr = *s++;
        else {
            Tape_destroy(t);
            return 0;
        }
    }
    return 1;
}

void Tape_destroy(Tape *t) {
    uchar_vec_destroy(&t->left, NULL);
    uchar_vec_destroy(&t->right, NULL);
}

size_t Tape_size(Tape *t) {
    return uchar_vec_size(&t->left) + uchar_vec_size(&t->right);
}

size_t Tape_lsize(Tape *t) {
    return uchar_vec_size(&t->left);
}

size_t Tape_rsize(Tape *t) {
    return uchar_vec_size(&t->right);
}

unsigned char *Tape_at(Tape *t, int index) {
    if (index < 0)
        return uchar_vec_at(&t->left, (size_t) ~index);
    else
        return uchar_vec_at(&t->right, (size_t) index);
}

unsigned char *Tape_atresv(Tape *t, int index) {
    unsigned char pad = '_';
    if (index < 0)
        return uchar_vec_atresv(&t->left, (size_t) ~index, &pad);
    else
        return uchar_vec_atresv(&t->right, (size_t) index, &pad);
}

void Tape_print(Tape *t) {
    size_t i = uchar_vec_size(&t->left);
    while (i--)
        printf("%c ", *uchar_vec_at(&t->left, i));
    for (size_t i = 0; i < uchar_vec_size(&t->right); i++)
        printf("%c ", *uchar_vec_at(&t->right, i));
    printf("\n");
}
