/*
    made by Farinap5 https://github.com/farinap5
*/

#ifndef STEG_H
#define STEG_H
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
     uint8_t *magic);

/*
     @param data is the array of bytes/pixels. Is unchanged data.
     @param data_size is thesize of data
     @returns if could be read (1) or not (0).
*/
uint8_t *read_from_byte_array(uint8_t *data, uint32_t data_size);

/*
     @param data is the array of bytes/pixels. Is unchanged data.
     @param data_size is thesize of data
     @param v print all
     @returns if could be read (1) or not (0).
*/
int test_for_saved(uint8_t *data, uint32_t data_size, int v);

uint32_t get_hidden_data_size(uint8_t *data, uint32_t data_size);

#endif //STEG_H