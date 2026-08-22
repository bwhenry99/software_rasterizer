#include <iostream>

#include "raylib.h"

int main()
{
   const int viewHeight = 512;
   const int viewWidth = 512;
   std::string title = "Software Rendered BAYBEE!!!";

   InitWindow(viewWidth, viewHeight, title.c_str());

   while (!WindowShouldClose())
   {
      BeginDrawing();
      {
         ClearBackground(RAYWHITE);
         DrawText("Hello World", 128, 126, 32, BLACK);
         
      }
      EndDrawing();


   }
}