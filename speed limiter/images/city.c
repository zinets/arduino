
/*******************************************************************************
* image
* filename: unsaved
* name: city
*
* preset name: Monochrome
* data block size: 8 bit(s), uint8_t
* RLE compression enabled: no
* conversion type: Monochrome, Diffuse Dither 128
* bits per pixel: 1
*
* preprocess:
*  main scan direction: top_to_bottom
*  line scan direction: forward
*  inverse: no
*******************************************************************************/

/*
 typedef struct {
     const uint8_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;
*/
#include <stdint.h>



static const uint8_t image_data_city[32] = {
    0x00, 0x00, 
    0x7f, 0x00, 
    0x41, 0x00, 
    0x41, 0xf0, 
    0x59, 0x10, 
    0x41, 0x10, 
    0x41, 0xd0, 
    0x59, 0x3e, 
    0x41, 0x22, 
    0x41, 0xaa, 
    0x59, 0x22, 
    0x41, 0x2a, 
    0x41, 0xa2, 
    0x59, 0x2a, 
    0x41, 0x22, 
    0x41, 0x22
};
const tImage city = { image_data_city, 16, 16,
    8 };

