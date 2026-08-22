#include <iostream>

#include "raylib.h"

void DrawLine(int aX, int aY, int bX, int bY, Color* aFrameBuffer, Color aColor);

struct triangle
{
   int a[2];
   int b[2];
   int c[2];
};

const int viewHeight = 720;
const int viewWidth = 1080;

int main()
{
   std::string title = "Software Rendered BAYBEE!!!";

   SetTargetFPS(60);
   InitWindow(viewWidth, viewHeight, title.c_str());
   Color*  frameBuffer = new Color[viewHeight * viewWidth];
   Image screenImage{ frameBuffer, viewWidth, viewHeight, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
   Texture2D screen = LoadTextureFromImage(screenImage);
   uint8_t globalAlpha = 255;

   // create one hardcoded triangle
   triangle try1{
      {14, 43},
      {24, 567},
      {1010, 700}
   };

   while (!WindowShouldClose())
   {
      globalAlpha--;
      for (unsigned int i = 0; i < viewWidth; i++)
      {
         for (unsigned int j = 0; j < viewHeight; j++)
         {
            // start by rasterizing the border of the triangle
            DrawLine(try1.a[0], try1.a[1], try1.b[0], try1.b[1], frameBuffer, RED);
            DrawLine(try1.b[0], try1.b[1], try1.c[0], try1.c[1], frameBuffer, GREEN);
            DrawLine(try1.c[0], try1.c[1], try1.a[0], try1.a[1], frameBuffer, BLUE);

            //unsigned int index = i + j * viewWidth;
            //frameBuffer[index].r = static_cast<uint8_t>(255 * i / viewWidth);
            //frameBuffer[index].g = static_cast<uint8_t>(255 * j / viewHeight);
            //frameBuffer[index].b = globalAlpha;
            //frameBuffer[index].a = 255;
            
         }
      }
      UpdateTexture(screen, frameBuffer);
      BeginDrawing();
      {
         DrawTexture(screen, 0, 0, RAYWHITE);
      }
      EndDrawing();


   }
   UnloadTexture(screen);
   delete frameBuffer;
   CloseWindow();

   return 0;
}

void DrawLine(int aX, int aY, int bX, int bY, Color* aFrameBuffer, Color aColor)
{
   bool steep = false;
   if (abs(bY - aY) > abs(bX - aX))
   {
      std::swap(aX, aY);
      std::swap(bX, bY);
      steep = true;
   }
   if (aX > bX)
   {
      std::swap(aX, bX);
      std::swap(aY, bY);
   }

   for (int x = aX; x <= bX; x++)
   {
      int y = aY + (float(x - aX) / float(bX - aX)) * (bY - aY);
      if (steep)
      {
         aFrameBuffer[y + x * viewWidth] = aColor;
      }
      else
      {
         aFrameBuffer[x + y * viewWidth] = aColor;
      }
   }
}