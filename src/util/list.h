#ifndef UTIL_LIST_H
#define UTIL_LIST_H

struct list;

int ls_init(struct list *ls);
void ls_free(struct list *ls);
struct list *ls_new();
void ls_destroy(struct list *ls);
void ls_add(struct list *ls, void *elem);
void ls_len(struct list *ls);
void *ls_get(struct list *ls, int index);
int ls_get2(struct list *ls, int index, void **elem);

#endif
