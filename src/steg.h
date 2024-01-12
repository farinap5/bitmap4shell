/*
    made by Farinap5 https://github.com/farinap5
*/

#ifndef STEG_H
#define STEG_H

#include "structs.h"
#include "utils.h"


/*
     @param data is the array of bytes/pixels. Is unchanged data.
     @param data_size is thesize of data
     @param volume is the data to encode to data
     @param volume_size is the size of the volume
     @param magic is the magic number (signature)
     @returns if could be written (1) or not (0).
*/
int write_to_byte_array(
     uint8_t *data, uint32_t data_size, 
     uint8_t *volume, uint32_t volume_size,
     uint8_t *magic) {

    uint32_t m_d_s = (data_size - (data_size % 8)); // data sized multiple of 8
    if (volume_size > (m_d_s/8)) {
        printf("Error: Volume size larger then expected!\n %d > %d",volume_size,(m_d_s/8));
        return 0;
    }
     
     unsigned int data_index = 0;

     // Write magic number to the data array
     for (int i = 0; i < 2; i++) { // For each byte from magic (signature)
          for (int j = 7; j != -1; j--) { // For each bit from this byte
               data[data_index] &= 0xFE; // Clear the last bit
               data[data_index] |= ((magic[i] >> j) & 0x01); // swap
               data_index++;
          }
     }

     uint8_t *v_size_byte = int_to_byte(volume_size);
     // write volume size to header of encoded data
     for (int i = 0; i < 4; i++) { // For each byte from volume_size (size of data to write)
          for (int j = 7; j != -1; j--) { // For each bit from this byte
               data[data_index] &= 0xFE; // Clear the last bit
               data[data_index] |= ((v_size_byte[i] >> j) & 0x01); // swap
               data_index++;
          }
     }
     free(v_size_byte);

     // write volume data
     for (int i = 0; i < volume_size; i++) { // For each byte from volume_size (size of data to write)
          for (int j = 7; j != -1; j--) { // For each bit from this byte
               data[data_index] &= 0xFE; // Clear the last bit
               data[data_index] |= ((volume[i] >> j) & 0x01); // swap
               data_index++;
          }
          //printf("%c written\n",volume[i]);
     }
     
     printf("%d bits written and bytes modified\n", data_index);
     return 1;
}

/*
     @param data is the array of bytes/pixels. Is unchanged data.
     @param data_size is thesize of data
     @returns if could be read (1) or not (0).
*/
uint8_t *read_from_byte_array(uint8_t *data, uint32_t data_size) {
    uint8_t magic[2];
    uint8_t byte_volume_size[4];
    //unsigned char magic = 0;
    
    unsigned int data_index = 0;

    // Extract magic
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            magic[i] = (magic[i] << 1) | (data[data_index] & 0x01);
            data_index++;
        }
    }
    /*for (int i = 0; i < 8; i++) {
        magic = (magic << 1) | (data[i] & 0x01);
    }*/
    //printf("Magic: 0x%X%X\n", magic[0],magic[1]);
    if (magic[0] != 0xde || magic[1] != 0xad ) {
        printf("Error: magic do not match.");
        return 0;
    }

    // Extract volume size
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            byte_volume_size[i] = (byte_volume_size[i] << 1) | (data[data_index] & 0x01);
            data_index++;
        }
    }
    uint32_t volume_size = byte_to_int(byte_volume_size);
    //printf("%d\n",volume_size);
    // After having the size of data saved in image, I may alocate and start extracting.

    uint8_t *volume = (uint8_t *)malloc(volume_size);
    // Extract volume
    for (int i = 0; i < volume_size; i++) {
        for (int j = 0; j < 8; j++) {
            volume[i] = (volume[i] << 1) | (data[data_index] & 0x01);
            data_index++;
        }
    }
    //printf("%s\n", volume);
    return volume;
}

/*
     @param data is the array of bytes/pixels. Is unchanged data.
     @param data_size is thesize of data
     @param v print all
     @returns if could be read (1) or not (0).
*/
int test_for_saved(uint8_t *data, uint32_t data_size, int v) {
    uint8_t magic[2];
    uint8_t byte_volume_size[4];
    //unsigned char magic = 0;
    
    unsigned int data_index = 0;

    // Extract magic
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            magic[i] = (magic[i] << 1) | (data[data_index] & 0x01);
            data_index++;
        }
    }

    
    if (magic[0] != 0xde || magic[1] != 0xad ) {
        if (v > 0)
            printf("Nothing Hidden!\n");
        return 0;
    }
    if (v > 0) {
        printf("Data hidden ");
        printf("-> Magic: 0x%X%X\n", magic[0],magic[1]);
    }

    // Extract volume size
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            byte_volume_size[i] = (byte_volume_size[i] << 1) | (data[data_index] & 0x01);
            data_index++;
        }
    }
    if (v > 0) {
        uint32_t volume_size = byte_to_int(byte_volume_size);
        printf("Hidden data length %db\n",volume_size);
    }

    return 1;
}


uint32_t get_hidden_data_size(uint8_t *data, uint32_t data_size) {
    unsigned int data_index = 16;
    uint8_t byte_volume_size[4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            byte_volume_size[i] = (byte_volume_size[i] << 1) | (data[data_index] & 0x01);
            data_index++;
        }
    }

    return byte_to_int(byte_volume_size);
}

#endif //STEG_H