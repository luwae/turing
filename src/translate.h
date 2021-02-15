#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <stdint.h>

#define TRANSLATOR_MISS 0xff

uint8_t *translator_new(char *chars);
void translator_free(uint8_t *t);

#endif
