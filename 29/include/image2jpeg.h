#ifndef	__BMP_HEADER_H
#define __BMP_HEADER_H

#pragma pack(2)
 

typedef unsigned short checkboard;
/*定义Dcheckboard为e四个字节的类型*/
typedef unsigned long Dcheckboard;
 
/*位图文件头*/
typedef struct BMP_FILE_HEADER
{
    checkboard bType;             /*  文件标识符          */
    Dcheckboard bSize;            /*  文件的大小          */
    checkboard bReserved1;        /*  保留值,必须设置为0  */       
    checkboard bReserved2;        /*  保留值,必须设置为0  */
    Dcheckboard bOffset;          /*  文件头的最后到图像数据位开始的偏移量    */
} BMPFILEHEADER;

/*位图信息头*/
typedef struct BMP_INFO
{
    Dcheckboard bInfoSize;       /*  信息头的大小             */
    Dcheckboard bWidth;          /*  图像的宽度               */
    Dcheckboard bHeight;         /*  图像的高度               */
    checkboard bPlanes;          /*  图像的位面数             */
    checkboard bBitCount;        /*  每个像素的位数           */
    Dcheckboard bCompression;    /*  压缩类型                 */
    Dcheckboard bmpImageSize;    /*  图像的大小,以字节为单位  */
    Dcheckboard bXPelsPerMeter;  /*  水平分辨率               */
    Dcheckboard bYPelsPerMeter;  /*  垂直分辨率               */
    Dcheckboard bClrUsed;        /*  使用的色彩数             */
    Dcheckboard bClrImportant;   /*  重要的颜色数             */
} BMPINF;
 
/*彩色表*/
typedef struct RGB_QUAD 
{
    checkboard rgbBlue;         /*  蓝色强度  */
    checkboard rgbGreen;        /*  绿色强度  */
    checkboard rgbRed;          /*  红色强度  */
    checkboard rgbReversed;     /*  保留值    */
} RGBQUAD;


typedef struct IMG2JPEG_INF
{
	char * image_pathname;
	char * jpeg_pathname;
	int quality;
	unsigned int width;
	unsigned int height;
} I2JINF;

typedef struct JPEG2IMG_INF
{
	char * image_pathname;
	char * jpeg_pathname;
	int quality;
	unsigned int width;
	unsigned int height;
} J2IINF;


int image_to_imgbuff(I2JINF *imageinfo, JSAMPLE * image_buffer);
int imgbuff_to_img(J2IINF *imageinfo, const JSAMPLE * image_buffer);
int bmp24_to_jpeg(I2JINF *imageinfo, JSAMPLE * image_buffer);
int read_JPEG_file (J2IINF *imageinfo, JSAMPLE * image_buffer);

#endif

