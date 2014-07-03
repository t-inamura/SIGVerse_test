struct BITMAPFILEHEADER {
  short bfType;
  int bfSize;
  short bfReserved1;
  short bfReserved2;
  int bfOffBits;
};
 
struct BITMAPINFOHEADER {
  int biSize;
  long biWidth;
  long biHeight;
  long biPlanes;
  long biBitCount;
  long biCompression;
  long biSizeImage;
};