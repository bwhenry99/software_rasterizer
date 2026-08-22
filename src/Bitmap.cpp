#include "Bitmap.hpp"

#include <fstream>
#include <iostream>


Bitmap::Bitmap(uint16_t aLength, uint16_t aWidth)
   : mLength(aLength)
	, mWidth(aWidth)
{
   mBitmapPtr = new color[aLength * aWidth];
}

Bitmap::~Bitmap()
{
   if (mBitmapPtr != nullptr)
   {
      delete[] mBitmapPtr;
   }
}

void Bitmap::WriteToBMP(std::string aFilepath)
{
   std::ofstream file;
   try
   {
      file.open(aFilepath);
   }
   catch(std::exception err)
   {
      std::cout << "Bad filepath.";
   }

   //TODO
}