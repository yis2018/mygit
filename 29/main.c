#include <stdio.h>
#include <stdlib.h>
#include "jpeglib.h"
#include "image2jpeg.h"

int main(void)
{
	I2JINF my_info = {
		"./a.bmp",
		"./output.jpg",
		20,
	};
	J2IINF my_info2 = {
		"./b.bmp",
		"./output.jpg",
		20,
	};
	JSAMPLE * image_buffer = NULL;
	JSAMPLE * image_buffer2 = NULL;
		
	image_buffer = malloc(1024 * 1024 * 3);
	if (image_buffer == NULL)
	{
		fprintf(stderr, "no more memory\n");
		return -1;
	}	
	image_buffer2 = malloc(1024 * 1024 * 3);
	if (image_buffer2 == NULL)
	{
		fprintf(stderr, "no more memory\n");
		return -1;
	}
	
	printf("&image_buffer = %p\n", image_buffer);
	image_to_imgbuff(&my_info, image_buffer);
	printf("&image_buffer = %p\n", image_buffer);
	bmp24_to_jpeg(&my_info, image_buffer);
	
	my_info2.width = my_info.width;
	my_info2.height = my_info.height;
	
	read_JPEG_file(&my_info2, image_buffer2);
	imgbuff_to_img(&my_info2, image_buffer2);
	
	free(image_buffer);
	free(image_buffer2);
	return 0;
}





