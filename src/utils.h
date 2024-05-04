/*
    made by Farinap5 https://github.com/farinap5
*/

#ifndef UTILS_H
#define UTILS_H

#include "structs.h"

void size_conv(unsigned int size);
void show_basic_info(bitmap_header *head, dib_header *dbhead);
uint8_t *int_to_byte(uint32_t n);
uint32_t byte_to_int(uint8_t *bytes);
void help();

#endif //UTILS_H