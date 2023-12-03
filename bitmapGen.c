#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "src/structs.h"

int main(){
    int row, column;
    uint32_t width = 1080;
    uint32_t height = 720;
    int size = width * height * 4;
    
    bitmap_header head;
    dib_header dibhead;

    strcpy(head.magic, "BM");
    head.size = (54+size);
    head.sec1 = 0x0;
    head.sec2 = 0x0;
    head.offset = 0x36;

    dibhead.dib_head_size = 40;
    dibhead.h_pixels = height;
    dibhead.w_pixels = width;
    dibhead.n_colors_p = 1;
    dibhead.bits_pixel = (uint16_t)32;
    dibhead.raw_bitmap = size;
    dibhead.compression = 0x0;
    dibhead.a = 0x0;
    dibhead.b = 0x0;
    dibhead.Important_color = 0x0;
    dibhead.colors_pallet = 0x0;

    uint8_t *pixels = (uint8_t *)malloc(size);
    for(row = height - 1; row >= 0; row--) {
        for(column = 0; column < width; column++) {
            int p = (row * width + column) * 4;
            pixels[p + 0] = 128; //blue
            pixels[p + 1] = 25;  //green
            pixels[p + 2] = 25;  //red
        }
    }

    FILE *file = fopen("32bitLarge.bmp", "wb");
    fwrite(&head, 1, 14, file);
    fwrite(&dibhead, 1, 40, file);
    fwrite(pixels, 1, size, file);
    free(pixels);
    fclose(file);
    return 0;
}
