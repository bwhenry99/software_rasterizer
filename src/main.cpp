#include <iostream>

#include "raylib.h"

int main()
{
   const int viewHeight = 720;
   const int viewWidth = 1080;
   std::string title = "Software Rendered BAYBEE!!!";

   SetTargetFPS(60);
   InitWindow(viewWidth, viewHeight, title.c_str());
   Color*  frameBuffer = new Color[viewHeight * viewWidth];
   Image screenImage{ frameBuffer, viewWidth, viewHeight, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
   Texture2D screen = LoadTextureFromImage(screenImage);
   uint8_t globalAlpha = 255;
   while (!WindowShouldClose())
   {
      globalAlpha--;
      for (unsigned int i = 0; i < viewWidth; i++)
      {
         for (unsigned int j = 0; j < viewHeight; j++)
         {
            
            unsigned int index = i + j * viewWidth;
            frameBuffer[index].r = static_cast<uint8_t>(255 * i / viewWidth);
            frameBuffer[index].g = static_cast<uint8_t>(255 * j / viewHeight);
            frameBuffer[index].b = globalAlpha;
            frameBuffer[index].a = globalAlpha;
            
         }
      }
      UpdateTexture(screen, frameBuffer);
      BeginDrawing();
      {
         DrawTexture(screen, 0, 0, RAYWHITE);
         DrawText("Hello World", 128, 126, 32, BLACK);
         
      }
      EndDrawing();


   }
   UnloadTexture(screen);
   delete frameBuffer;
   CloseWindow();

   return 0;
}