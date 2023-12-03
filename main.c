#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/utils.h"
#include "src/structs.h"
#include "src/steg.h"

int main(int argc, char *argv[]) {
     bitmap_header  head;
     dib_header     dbhead;
     FILE           *file;
     FILE           *source;
     FILE           *out_f;
     uint8_t        magic[2] = {0xde, 0xad}; // The signature must be 2 bytes.
     char *fname     = "32bitLarge.bmp";
     char *out_f_n   = "out.bmp";
     char *datafile  = "shellcode.bin";
     int function = 0 ;

     for (int i = 0; i < argc; i++) {
          if (strcmp("-h",argv[i]) == 0) {
               help();
               return 0;
          }
          if (strcmp("-b",argv[i]) == 0) { // bitmap source
               fname = argv[i+1];
          }
          if (strcmp("-r",argv[i]) == 0) { // image result
               out_f_n = argv[i+1];
          }
          if (strcmp("-f",argv[i]) == 0) { // data file
               datafile = argv[i+1];
          }
          if (strcmp("-d",argv[i]) == 0) { // describe
               function = 0;
          }
          if (strcmp("-e",argv[i]) == 0) { // extract
               function = 1;
          }
          if (strcmp("-s",argv[i]) == 0) { // hide
               function = 2;
          }
     }


     file = fopen(fname,"rb");
     if (file == NULL) {
          printf("Error: Could not open %s.\n", fname);
          return 0;
     }

     fread(&head,    sizeof(bitmap_header), 1, file);
     fread(&dbhead,  sizeof(dib_header), 1, file);

     if (head.magic[0] != 0x42 || head.magic[1] != 0x4d) {
          printf("Error: %s Not a bmp file.\n", fname);
          fclose(file);
          return 0;
     }
     // may be optimized. it dont need to be done for all cases, like 'describe'.
     uint32_t size = ((dbhead.w_pixels * dbhead.h_pixels) * (dbhead.bits_pixel/(uint32_t)8));
     uint8_t *data = (uint8_t *)malloc(size);
     fread(data,  size, 1, file);
     fclose(file);
     if (function == 0) { // Describe
          show_basic_info(&head,&dbhead);
          test_for_saved(data, size, 1);
          return 0;
     } else if (function == 1) {
          show_basic_info(&head,&dbhead);
          if (test_for_saved(data, size, 1) == 1) {
               uint8_t *volume = read_from_byte_array(data, size);
               out_f = fopen(datafile,"wb");
               uint32_t volume_size = get_hidden_data_size(data, size);
               fwrite(volume, volume_size, 1, out_f);
               printf("%db written to %s\n", volume_size, datafile);
               free(volume);
               return 0;
          }
     } else if (function == 2) {
          show_basic_info(&head,&dbhead);
          if (test_for_saved(data, size, 0) == 1) {
               printf("There are data saved. You override lose it!\n continue? [yn]: ");
               char op;
               scanf("%c", &op);
               if (op == 'n' || op == 'N') {
                    return 0;
               }
               if (op != 'y' && op != 'Y') {
                    printf("Could not get it!\n");
                    return 1;
               }
          }

          source = fopen(datafile,"rb");
          if (file == NULL) {
               printf("Error: Could not open %s.\n", datafile);
               return 0;
          }
          fseek(source, 0, SEEK_END);
          uint32_t source_data_size = ftell(source);

          fseek(source, 0, SEEK_SET);
          uint8_t *volume = (uint8_t *)malloc(source_data_size);
          fread(volume, source_data_size, 1, source);

          if (write_to_byte_array(data, size, volume ,source_data_size, magic) == 1) {
               out_f = fopen(out_f_n,"wb");
               fwrite(&head, sizeof(bitmap_header), 1, out_f);
               fwrite(&dbhead, sizeof(dib_header), 1, out_f);
               fwrite(data, size, 1, out_f);
               fclose(out_f);
               printf("%s modified\n", out_f_n);
          }
     }
     
     return 0;
}