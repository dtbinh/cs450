/* TGA image file creation */

#include <stdio.h>              /* standard I/O */
#include <stdint.h>             /* C99 integer types */
#include <inttypes.h>           /* C99 integer format strings */
#include <string.h>
#include "tga.h"                /* TGA interface header */

bool
write_file (char * name, uint8_t * pic, uint32_t height, uint32_t width)
{
  //Calculate the hex values of the width/height and flip them (little endian)
  uint8_t width_up = (uint8_t)(width>>8);
  uint8_t width_lo = (uint8_t)(width);
  uint8_t height_up = (uint8_t)(height>>8);
  uint8_t height_lo = (uint8_t)(height);

  uint8_t tga_header[] = { 0x00, 0x00,  0x02,  0x00,  0x00,  0x00,  0x00,  
                          0x00, 0x00,  0x00,  0x00,  0x00, width_lo, width_up,
                          height_lo, height_up, 0x18,  0x00};

  //Initialize the file pointer
  FILE *fp;
	fp = fopen(name,"wb"); //Open the file
  fwrite(tga_header, sizeof(uint8_t), 18, fp); //Write the header for the tga file
  fwrite(pic,sizeof(uint8_t),height * width * 3,fp); //Write the pixels to the file
  fclose(fp); //Close the file
  printf ("Writing %" PRIu32 " bytes\n", width * height * 3 + 18);
  fp = NULL; //Closing the dangling pointer
  return true;
}
