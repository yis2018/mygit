#ifndef	__BMP_HEADER_H
#define __BMP_HEADER_H

#pragma pack(2)
 

typedef unsigned short checkboard;
/*����DcheckboardΪe�ĸ��ֽڵ�����*/
typedef unsigned long Dcheckboard;
 
/*λͼ�ļ�ͷ*/
typedef struct BMP_FILE_HEADER
{
    checkboard bType;             /*  �ļ���ʶ��          */
    Dcheckboard bSize;            /*  �ļ��Ĵ�С          */
    checkboard bReserved1;        /*  ����ֵ,��������Ϊ0  */       
    checkboard bReserved2;        /*  ����ֵ,��������Ϊ0  */
    Dcheckboard bOffset;          /*  �ļ�ͷ�����ͼ������λ��ʼ��ƫ����    */
} BMPFILEHEADER;

/*λͼ��Ϣͷ*/
typedef struct BMP_INFO
{
    Dcheckboard bInfoSize;       /*  ��Ϣͷ�Ĵ�С             */
    Dcheckboard bWidth;          /*  ͼ��Ŀ��               */
    Dcheckboard bHeight;         /*  ͼ��ĸ߶�               */
    checkboard bPlanes;          /*  ͼ���λ����             */
    checkboard bBitCount;        /*  ÿ�����ص�λ��           */
    Dcheckboard bCompression;    /*  ѹ������                 */
    Dcheckboard bmpImageSize;    /*  ͼ��Ĵ�С,���ֽ�Ϊ��λ  */
    Dcheckboard bXPelsPerMeter;  /*  ˮƽ�ֱ���               */
    Dcheckboard bYPelsPerMeter;  /*  ��ֱ�ֱ���               */
    Dcheckboard bClrUsed;        /*  ʹ�õ�ɫ����             */
    Dcheckboard bClrImportant;   /*  ��Ҫ����ɫ��             */
} BMPINF;
 
/*��ɫ��*/
typedef struct RGB_QUAD 
{
    checkboard rgbBlue;         /*  ��ɫǿ��  */
    checkboard rgbGreen;        /*  ��ɫǿ��  */
    checkboard rgbRed;          /*  ��ɫǿ��  */
    checkboard rgbReversed;     /*  ����ֵ    */
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

