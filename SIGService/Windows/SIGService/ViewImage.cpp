#include <windows.h>
#define _USE_MATH_DEFINES

#include <math.h>
#include "ViewImage.h"
#include <stdio.h>

namespace sigverse
{
	int ViewImage::getWidthBytes(int width, int bytesPerOnePixel)
	{
		int widthByteSize = width * bytesPerOnePixel;

		// width must be aligned 32bit boundary
		if ((widthByteSize & 0x03) > 0) {
			widthByteSize = ((widthByteSize >> 2) + 1) << 2;
		}

		return widthByteSize;
	}

	int ViewImage::calcBufferSize(const ViewImageInfo &info)
	{
		return getWidthBytes(info.getWidth(), info.getBytesPerOnePixel())*info.getHeight();
	}

	void ViewImage::setBitImageAsWindowsBMP(unsigned char *bitImage)
	{
		if (!bitImage) return;
  
		int width, height;
		//int widthByteSize, imageByteSize;
		int widthByteSize;
		int x, y;
  
		width = getWidth();
		height = getHeight();

		double fov = getFOVy();
		double fl = (height/2)/tan(fov/2*M_PI/180);

		// width must be aligned 32bit boundary
		widthByteSize = getWidthBytes(width, m_info.getBytesPerOnePixel());
  
		// clear buffer
		memset(m_buf, 0, m_buflen);
  
		int nbyte = m_info.getBytesPerOnePixel();
  
		// Note: be cautious to difference of y-axis order and pixel format.
		// (src) RGBARGBARGBA....
		// (dst) BGRBGRBGR....
		for (y=0; y<height; y++) {
			for (x=0; x<width; x++) {
				unsigned char *pFrom = bitImage + ((height-(y+1))*width + x)*4;
				for(int i = 0;i<nbyte;i++){
					//modified by okamoto@tome (2011/9/16)
					m_buf[y*widthByteSize + x*nbyte + nbyte-(i+1)] = pFrom[i];
				}
			}
		}
	}
	bool ViewImage::saveAsWindowsBMP(const char *fname)
	{

		assert(m_info.getDataType() == IMAGE_DATA_WINDOWS_BMP);
	
		FILE *fp;
		errno_t error;
		if(error = fopen_s(&fp, fname, "wb") != 0){
			return false; 
		}

		assert(sizeof(BITMAPFILEHEADER) == 14);
		assert(sizeof(BITMAPINFOHEADER) == 40);
	
		BITMAPINFOHEADER bi = {0};
	
		char *tmp;
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = getWidth();
		bi.biHeight = -getHeight();
		bi.biPlanes = 1;
		switch(m_info.getColorBitType()) {
	case COLORBIT_24:
		{
			bi.biBitCount = 24; break;
		}
	case DEPTHBIT_8:
		{
			tmp = new char[m_buflen*3];
			bi.biBitCount = 24;

			int height = getHeight();
			int width = getWidth();

			for(int i = 0; i < height; i++){
				for(int j = 0; j < width; j++){
					int tmp_i = i*width+j;

					char distance = m_buf[i*width+j];
		
					tmp[tmp_i*3]   = distance;
					tmp[tmp_i*3+1] = distance;
					tmp[tmp_i*3+2] = distance;

				}
			}
			break;
		}
	default:
		assert(0);
		}
		int size = getBufferLength();

		bi.biCompression = 0;
		bi.biSizeImage = size;
	
		BITMAPFILEHEADER bf = {0};

		bf.bfType = *(unsigned short*)"BM";
		bf.bfReserved1 = 0;
		bf.bfReserved2 = 0;
		bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		bf.bfSize = bf.bfOffBits + bi.biSizeImage;
	
		fwrite((char*)&bf, sizeof(bf), 1, fp);
		fwrite((char*)&bi, sizeof(bi), 1, fp);
		if(m_info.getColorBitType() == DEPTHBIT_8) 
		{
			fwrite(tmp, size*3, 1, fp);
			delete [] tmp;
		}
		else
		{
			fwrite(getBuffer(), size, 1, fp);
		}
		fclose(fp);

		return true;
	}

}


