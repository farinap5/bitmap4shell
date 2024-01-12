/*
    made by Farinap5 https://github.com/farinap5
*/

#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char      magic[2];
  uint32_t  size;
  uint16_t  sec1;
  uint16_t  sec2;
  uint32_t  offset;
} __attribute__ ((packed)) bitmap_header;

typedef struct {
    //uint16_t  padding;
    uint32_t  dib_head_size;
    uint32_t  h_pixels;
    uint32_t  w_pixels;
    uint16_t  n_colors_p;
    uint16_t  bits_pixel; // each pixel has x bits
    uint32_t  compression;
    uint32_t  raw_bitmap;
    uint32_t  a;
    uint32_t  b;
    uint32_t  colors_pallet;
    uint32_t  Important_color;
} __attribute__ ((packed)) dib_header;

#endif //STRUCTS_H