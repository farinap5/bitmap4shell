/*
    made by Farinap5 https://github.com/farinap5
*/

#ifndef UTILS_H
#define UTILS_H

#include "structs.h"

void size_conv(unsigned int size) {
  int timesD = 0;
  
  while (size > 1024) {
    timesD++;
    size = (int)(size / 1024);
  }
  printf("%d", size);
  if (timesD == 0) {
    printf("B\n");
  } else if (timesD == 1) {
    printf("Kb\n");
  } else if (timesD == 2) {
    printf("Mb\n");
  } else if (timesD == 3) {
    printf("Gb\n");
  } else if (timesD == 4) {
    printf("Tb\b");
  } else {
    printf("??\n");
  }
}

void show_basic_info(bitmap_header *head, dib_header *dbhead) {
     printf("Magic Number: %.2s\nSize: %d\nOffset: %d\n",
     head->magic, head->size, head->offset);
     printf("%dwpx x %dhpx of %dbits\nOff Size: %d\n", 
     dbhead->w_pixels, dbhead->h_pixels, dbhead->bits_pixel,
     (dbhead->w_pixels * dbhead->h_pixels) * (dbhead->bits_pixel/(uint32_t)8));
     
     uint32_t size = ((dbhead->w_pixels * dbhead->h_pixels) * (dbhead->bits_pixel/(uint32_t)8));
     uint32_t real_size = (size - (size % 8));
     printf("Image may hold %db -> ",real_size/8);
     size_conv(real_size/8);
}


uint8_t *int_to_byte(uint32_t n) {
     uint8_t *bytes = (uint8_t *)malloc(4);
     //unsigned long n = 175;

     bytes[0] = (n >> 24) & 0xFF;
     bytes[1] = (n >> 16) & 0xFF;
     bytes[2] = (n >> 8) & 0xFF;
     bytes[3] = n & 0xFF;
     return bytes;
}

uint32_t byte_to_int(uint8_t *bytes) {
     uint32_t x = (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];
     return x;
}

void help() {
     printf("-h Help menu\n"
     "-b Bitmap image as source\n"
     "-r Result file (result bitmab after process)\n"
     "-f File from where to get or save volume\n"
     "-d Describe bitmap image\n" 
     "-e Extract data (from -b to -f)\n"  
     "-s Hide data (from -f having -b as bitmap source, create modified bitmap -r)\n");
}

#endif //UTILS_H