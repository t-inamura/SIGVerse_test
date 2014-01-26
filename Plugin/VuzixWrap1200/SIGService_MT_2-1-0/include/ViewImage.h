#ifndef ViewImage_h
#define ViewImage_h

#include <stdlib.h>
#include <assert.h>
#include <math.h>

namespace sigverse
{

	//typedef unsigned short ImageDataType;

	enum ImageDataType{
		IMAGE_DATA_TYPE_ANY = 0,
		IMAGE_DATA_WINDOWS_BMP,
	};


	//typedef unsigned short ColorBitType;

	enum ColorBitType{
		COLORBIT_ANY = 0,
		COLORBIT_24,
		DEPTHBIT_8,
	};

	enum ImageDataSize {
		IMAGE_320X240 = 0,
		IMAGE_320X1,
	};

	class ViewImageInfo
	{
	private:
		ImageDataType	m_dataType;
		ColorBitType	m_cbType;
		int		m_width;
		int		m_height;

	public:
		ViewImageInfo(ImageDataType dataType, ColorBitType cbType, ImageDataSize sz) : m_dataType(dataType), m_cbType(cbType)
		{
		
			switch(sz) {
	case IMAGE_320X240:
		m_width = 320; m_height = 240;
		break;
	case IMAGE_320X1:
		m_width = 320; m_height = 1;
		break;
	default:
		assert(0);
		break;
			}
		}
		ViewImageInfo(ImageDataType dataType, ColorBitType cbType, int w, int h)
			: m_dataType(dataType), m_cbType(cbType), m_width(w), m_height(h)
		{
		}
		ViewImageInfo(const ViewImageInfo &o)
			: m_dataType(o.m_dataType),
			m_cbType(o.m_cbType),
			m_width(o.m_width), m_height(o.m_height)
		{
		}
		ImageDataType getDataType() const { return m_dataType; }
		ColorBitType  getColorBitType() const { return m_cbType; }
		int	      getWidth() const { return m_width; }
		int	      getHeight() const { return m_height; }

		int	getBytesPerOnePixel() const {
			int b;
			switch(m_cbType) {
	case COLORBIT_24:
		b = 3; break;
	case DEPTHBIT_8:
		b = 1; break;
	default:
		assert(0);	// error
		b = 0; break;
			}

			return b;
		}
	};


	class ViewImage
	{
	private:
		ViewImageInfo	m_info;
		char *		m_buf;
		int		m_buflen;
	
		double m_fov;
		double m_ar;   //aspect ratio
	public:
		ViewImage(const ViewImageInfo &info)
			: m_info(info), m_buf(0), m_buflen(0)
		{
			m_buflen = calcBufferSize(info);
			m_buf = new char [m_buflen];
		}

		~ViewImage() {
			if (m_buf) {
				delete [] m_buf; m_buf = 0;
			}
		}

	private:
		// sekikawa(2007/10/12)
		int getWidthBytes(int width, int bytesPerOnePixel);

		int	calcBufferSize(const ViewImageInfo &info);

	public:
		const ViewImageInfo & getInfo() { return m_info; }

		int  getWidth() const { return m_info.getWidth(); }
		int  getHeight() const { return m_info.getHeight(); }
		char * getBuffer() const { return m_buf; }
		void setBuffer(char *buf) { m_buf = buf; }
		int   getBufferLength() const { return m_buflen; }
		void setFOVy(double fov){ m_fov = fov;} 
		void setAspectRatio(double ar){ m_ar = ar;}
		double getFOVy(){ return m_fov;} 
		double getFOVx(){ return 2*atan(tan(m_fov*0.5)*m_ar);} 
		double getAspectRatio(){ return m_ar;}

		//virtual void setDimension(int n){} 

        // sekikawa(2007/10/12)
        // convert RGBA format to BGR and turn y-axis upside down
        void setBitImageAsWindowsBMP(unsigned char *bitImage);

        bool    saveAsWindowsBMP(const char *fname);

	};
} // namespace sigverse
#endif // ViewImage_h
 

