
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
    0x00, 0x03, 
    0x00, 0x04, 
    0x00, 0x04, 
    0x00, 0x06, 
    0x00, 0x01, 
    0x00, 0x00, 
    0x00, 0x00, 
    0x80, 0x00, 
    0x40, 0xc0, 
    0x31, 0x23, 
    0x0a, 0x1c, 
    0x04, 0x08, 
    0x18, 0x08, 
    0x20, 0x04, 
    0x40, 0x02, 
    0x80, 0x01
};
const tImage city = { image_data_city, 16, 16,
    8 };

