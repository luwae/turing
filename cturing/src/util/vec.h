#include <stdlib.h>
#include <string.h>

struct P(vec) {
    size_t size;
    size_t cap;
    T *data;
};

void P(vec_init)(struct P(vec) *v);
void P(vec_destroy)(struct P(vec) *v, void (*elem_destroy)(T *));
size_t P(vec_size)(struct P(vec) *v);
T *P(vec_pushresv)(struct P(vec) *v);
int P(vec_push)(struct P(vec) *v, T *data);
T *P(vec_at)(struct P(vec) *v, size_t index);
T *P(vec_atresv)(struct P(vec) *v, size_t index, T *pad);

#if defined(VEC_IMPL)
void P(vec_init)(struct P(vec) *v) {
    v->size = v->cap = 0;
    v->data = NULL;
}

void P(vec_destroy)(struct P(vec) *v, void (*elem_destroy)(T *)) {
    if (v->data) {
        if (elem_destroy)
            for (size_t i = 0; i < v->size; i++)
                elem_destroy(&v->data[i]);
        free(v->data);
    }
}

struct P(vec) *P(vec_new)() {
    struct P(vec) *v = malloc(sizeof(*v));
    if (v)
        P(vec_init)(v);
    return v;
}

void P(vec_free)(struct P(vec) *v, void (*elem_free)(T *)) {
    P(vec_destroy)(v, elem_free);
    free(v);
}

size_t P(vec_size)(struct P(vec) *v) {
    return v->size;
}

T *P(vec_pushresv)(struct P(vec) *v) {
    if (v->size == v->cap) {
        size_t newcap = v->cap ? v->cap * 2 : 8;
        T *newdata = malloc(newcap * sizeof(T));
        if (!newdata)
            return NULL;
        if (v->data) {
            memcpy(newdata, v->data, v->cap * sizeof(T));
            free(v->data);
        }
        v->cap = newcap;
        v->data = newdata;
    }
    return &v->data[v->size++];
}

int P(vec_push)(struct P(vec) *v, T *data) {
    T *ptr = P(vec_pushresv)(v);
    if (!ptr)
        return 0;
    *ptr = *data;
    return 1;
}

T *P(vec_at)(struct P(vec) *v, size_t index) {
    return (index < v->size) ? &v->data[index] : NULL;
}

T *P(vec_atresv)(struct P(vec) *v, size_t index, T *pad) {
    if (index < v->size)
        return &v->data[index];
    T *ptr;
    while (index >= v->size)
        if ((ptr = P(vec_pushresv)(v)))
            *ptr = *pad;
        else
            return NULL;
    return &v->data[index];
}
#endif
