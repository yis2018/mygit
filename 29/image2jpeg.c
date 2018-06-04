#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "jpeglib.h"
#include "image2jpeg.h"


struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;

// 提取未压缩图像数据
// 
int image_to_imgbuff(I2JINF *imageinfo, JSAMPLE * image_buffer)
{
	FILE *bmp_file = NULL;
	
    BMPFILEHEADER fileHeader;
    BMPINF infoHeader;
	int row_counter = 0, line_counter = 0;
	JSAMPLE temp[3] = {0};
	JSAMPLE *row_pointer;
	
	// 打开 bmp 文件
	if ((bmp_file = fopen(imageinfo->image_pathname, "rb")) == NULL) 
	{
		fprintf(stderr, "can't open %s\n", imageinfo->image_pathname);
		return -1;
	}
	
	fseek(bmp_file, 0, 0);								// 将文件指针移到起始地址
	fread(&fileHeader, sizeof(fileHeader), 1, bmp_file);	// 读取原图像bmp信息
	fread(&infoHeader, sizeof(infoHeader), 1, bmp_file);
	// 对于 24/32bit 深度的 bmp 不需要调色板
	
	if (infoHeader.bBitCount != 24)
	{
		fprintf(stderr, "the bmp is not a 24 bit\n");
		fclose(bmp_file);
		return -1;
	}
	imageinfo->width = infoHeader.bWidth;
	imageinfo->height = infoHeader.bHeight;
	
	// image_buffer = malloc(infoHeader.bHeight * infoHeader.bWidth * 3);
	// if (image_buffer == NULL)
	// {
		// fprintf(stderr, "no more memory\n");
		// fclose(bmp_file);
		// return -1;
	// }
	printf("fileHeader->bType = %x\n", fileHeader.bType);
	printf("w = %d, h = %d\n", imageinfo->width, imageinfo->height);
	printf("w = %d, h = %d\n", imageinfo->width, imageinfo->height);
	printf("w = %d, h = %d\n", imageinfo->width, imageinfo->height);
	printf("&image_buffer = %p\n", image_buffer);
	
	// 读取图像数据		bmp 数据从最后一行开始，从左到右存放，BGR 排序
	for (row_counter=infoHeader.bHeight-1; row_counter>=0; row_counter--)
	{
		row_pointer = image_buffer + row_counter * infoHeader.bWidth * 3;
		for (line_counter=0; line_counter<infoHeader.bWidth; line_counter++)
		{
			fread(temp, (infoHeader.bBitCount / 8), 1, bmp_file);
			*(row_pointer++) = temp[2];
			*(row_pointer++) = temp[1];
			*(row_pointer++) = temp[0];
		}
	}
	
	printf("&image_buffer = %p\n", image_buffer);
	
	fclose(bmp_file);
	return 0;
}

int imgbuff_to_img(J2IINF *imageinfo, const JSAMPLE * image_buffer)
{
	FILE *bmp_file = NULL;
	
    BMPFILEHEADER fileHeader = {0};
    BMPINF infoHeader ={0};
	int row_counter = 0, line_counter = 0;
	//JSAMPLE temp[3] = {0};
	JSAMPLE *row_pointer;
	
	// 打开 bmp 文件
	if ((bmp_file = fopen(imageinfo->image_pathname, "wb")) == NULL) 
	{
		fprintf(stderr, "can't open %s\n", imageinfo->image_pathname);
		return -1;
	}
	
	fileHeader.bType = ('M' << 8) | 'B';
	fileHeader.bSize = sizeof(BMPFILEHEADER) + sizeof(BMPINF) + imageinfo->width * imageinfo->height * 3;
	fileHeader.bOffset = 0;
	
	infoHeader.bInfoSize = sizeof(BMPINF);
	infoHeader.bWidth = imageinfo->width;
	infoHeader.bHeight = imageinfo->height;
	infoHeader.bPlanes = 1;
	infoHeader.bBitCount = 24;
	infoHeader.bCompression = 0;
	infoHeader.bmpImageSize = imageinfo->width * imageinfo->height * 3;
	infoHeader.bXPelsPerMeter = 0;
	
	
	fseek(bmp_file, 0, 0);								// 将文件指针移到起始地址
	fwrite(&fileHeader, sizeof(fileHeader), 1, bmp_file);	// 读取原图像bmp信息
	fwrite(&infoHeader, sizeof(infoHeader), 1, bmp_file);
	// 对于 24/32bit 深度的 bmp 不需要调色板
	
	// if (infoHeader.bBitCount != 24)
	// {
		// fprintf(stderr, "the bmp is not a 24 bit\n");
		// fclose(bmp_file);
		// return -1;
	// }
	// imageinfo->width = infoHeader.bWidth;
	// imageinfo->height = infoHeader.bHeight;
	
	// image_buffer = malloc(infoHeader.bHeight * infoHeader.bWidth * 3);
	// if (image_buffer == NULL)
	// {
		// fprintf(stderr, "no more memory\n");
		// fclose(bmp_file);
		// return -1;
	// }
	
	printf("w = %d, h = %d\n", imageinfo->width, imageinfo->height);
	printf("&image_buffer = %p\n", image_buffer);
	
	// 读取图像数据		bmp 数据从最后一行开始，从左到右存放，BGR 排序
	for (row_counter=infoHeader.bHeight-1; row_counter>=0; row_counter--)
	{
		row_pointer = image_buffer + row_counter * infoHeader.bWidth * 3;
		for (line_counter=0; line_counter<infoHeader.bWidth; line_counter++)
		{
			fwrite(row_pointer, 3, 1, bmp_file);
			row_pointer += 3;
			//*(row_pointer++) = temp[2];
			//*(row_pointer++) = temp[1];
			//*(row_pointer++) = temp[0];
		}
	}
	
	printf("&image_buffer = %p\n", image_buffer);
	
	fclose(bmp_file);
	return 0;
}

int bmp24_to_jpeg(I2JINF *imageinfo, JSAMPLE * image_buffer)
{
	struct jpeg_compress_struct cinfo; 		// 压缩参数结构体
    struct jpeg_error_mgr jerr; 		// 错误服务函数
	
	FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);		// 创建一个压缩算法实例
	if ((outfile = fopen(imageinfo->jpeg_pathname, "wb")) == NULL) 
	{
		fprintf(stderr, "can't open %s\n", imageinfo->jpeg_pathname);
		return -1;
	}
	jpeg_stdio_dest(&cinfo, outfile);		// 设置stdio流输出文件
	
	// 设置参数
	cinfo.image_width = imageinfo->width; 		/* image width and height, in pixels */
	cinfo.image_height = imageinfo->height;
	cinfo.input_components = 3;				/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 		/* colorspace of input image */
	
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, imageinfo->quality, TRUE /* limit to baseline-JPEG values */);
	
	jpeg_start_compress(&cinfo, TRUE);
	
	// 每行图像数据量
	row_stride = cinfo.image_width * 3;	/* JSAMPLEs per row in image_buffer */

	while (cinfo.next_scanline < cinfo.image_height) {
		// 指向某一行
		row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
		//printf("%p\n", (row_pointer[0]));
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	
	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
	
	return 0;
	
}

void my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

int read_JPEG_file (J2IINF *imageinfo, JSAMPLE * image_buffer)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */
  int line_counter;

  if ((infile = fopen(imageinfo->jpeg_pathname, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", imageinfo->jpeg_pathname);
    return 0;
  }

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {

    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
  }
  jpeg_create_decompress(&cinfo);

  jpeg_stdio_src(&cinfo, infile);

  (void) jpeg_read_header(&cinfo, TRUE);

  (void) jpeg_start_decompress(&cinfo);

  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  while (cinfo.output_scanline < cinfo.output_height) {

    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    //put_scanline_someplace(image_buffer[0], row_stride);
	for (line_counter=0; line_counter<cinfo.output_width*3; line_counter+=3)
	{
		*image_buffer++ = buffer[0][line_counter + 2];
		*image_buffer++ = buffer[0][line_counter + 1];
		*image_buffer++ = buffer[0][line_counter + 0];
	}
  }

  (void) jpeg_finish_decompress(&cinfo);

  jpeg_destroy_decompress(&cinfo);

  fclose(infile);

  return 1;
}

