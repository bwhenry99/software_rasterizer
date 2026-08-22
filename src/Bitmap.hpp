#include <cstdint>
#include <string>

struct color
{
   uint8_t red;
   uint8_t green;
   uint8_t blue;
};

class Bitmap
{
public:
	Bitmap(uint16_t aLength, uint16_t aWidth);
	~Bitmap();

   void WriteToBMP(std::string aFilepath);

	uint16_t mLength;
	uint16_t mWidth;
   color* mBitmapPtr = nullptr;
};