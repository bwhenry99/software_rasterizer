#include <iostream>
#include <random>

#include "raylib.h"

#include "Model.hpp"

void DrawLine(int aX, int aY, int bX, int bY, Color* aFrameBuffer, Color aColor);

struct triangle
{
   int a[2];
   int b[2];
   int c[2];
};

bool IsInTriangle(int aX, int aY, triangle aTriangle);
int DotProduct(int x1, int y1, int x2, int y2);
triangle GenerateRandomTriangle(int xBound[2], int yBound[2], std::default_random_engine& aRandEngine);

const int viewHeight = 720;
const int viewWidth = 1080;

int main()
{
   std::default_random_engine randEngine;
   std::string title = "Software Rendered BAYBEE!!!";

   SetTargetFPS(10);
   InitWindow(viewWidth, viewHeight, title.c_str());
   Color*  frameBuffer = new Color[viewHeight * viewWidth];
   Image screenImage{ frameBuffer, viewWidth, viewHeight, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
   Texture2D screen = LoadTextureFromImage(screenImage);

   //// create one hardcoded triangle
   //triangle try1{
   //   {14, 43},
   //   {24, 567},
   //   {1010, 700}
   //};

   swrast::Model cube("../resources/Cube.obj");

   int xTriBounds[2] = {10, viewWidth - 10};
   int yTriBounds[2] = {10, viewHeight - 10};
   while (!WindowShouldClose())
   {
      triangle tri = GenerateRandomTriangle(xTriBounds, yTriBounds, randEngine);
      // start by rasterizing the border of the triangle
      //DrawLine(tri.a[0], tri.a[1], tri.b[0], tri.b[1], frameBuffer, RED);
      //DrawLine(tri.b[0], tri.b[1], tri.c[0], tri.c[1], frameBuffer, GREEN);
      //DrawLine(tri.c[0], tri.c[1], tri.a[0], tri.a[1], frameBuffer, BLUE);

      for (unsigned int i = 0; i < viewWidth; i++)
      {
         for (unsigned int j = 0; j < viewHeight; j++)
         {
            if (IsInTriangle(i, j, tri))
            {
               unsigned int index = i + j * viewWidth;
               frameBuffer[index].r = static_cast<uint8_t>(255 * i / viewWidth);
               frameBuffer[index].g = static_cast<uint8_t>(255 * j / viewHeight);
               frameBuffer[index].b = 0;
               frameBuffer[index].a = 255;
            }
            else
            {
               unsigned int index = i + j * viewWidth;
               frameBuffer[index].r = 0;
               frameBuffer[index].g = 0;
               frameBuffer[index].b = 0;
               frameBuffer[index].a = 255;
            }
         }
      }

      DrawLine(tri.a[0], tri.a[1], tri.b[0], tri.b[1], frameBuffer, WHITE);
      DrawLine(tri.b[0], tri.b[1], tri.c[0], tri.c[1], frameBuffer, WHITE);
      DrawLine(tri.c[0], tri.c[1], tri.a[0], tri.a[1], frameBuffer, WHITE);

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

bool IsInTriangle(int aX, int aY, triangle aTriangle)
{
   int xBounds[2] = { std::min({aTriangle.a[0], aTriangle.b[0], aTriangle.c[0]}),
                      std::max({aTriangle.a[0], aTriangle.b[0], aTriangle.c[0]}) };
   int yBounds[2] = { std::min({aTriangle.a[1], aTriangle.b[1], aTriangle.c[1]}),
                      std::max({aTriangle.a[1], aTriangle.b[1], aTriangle.c[1]}) };

   // if outside bounding box return false
   if (aX < xBounds[0] || aX > xBounds[1] || aY < yBounds[0] || aY > yBounds[1])
   {
      return false;
   }

   // get dot product of the vecttor from a triangle point to the coordinat in question
   // and each edge of the triangle rotated 90 degrees. x and y of triangle edge are flipped to 
   // perform rotation
   int dots[3] = {DotProduct(aX - aTriangle.a[0], aY - aTriangle.a[1], -(aTriangle.b[1] - aTriangle.a[1]), aTriangle.b[0] - aTriangle.a[0]),
                     DotProduct(aX - aTriangle.b[0], aY - aTriangle.b[1], -(aTriangle.c[1] - aTriangle.b[1]), aTriangle.c[0] - aTriangle.b[0]),
                     DotProduct(aX - aTriangle.c[0], aY - aTriangle.c[1], -(aTriangle.a[1] - aTriangle.c[1]), aTriangle.a[0] - aTriangle.c[0]) };
   
   // dot product tells us if point is right or left of edge based on sign
   // if point is either left of all edges or right of all edges it is inside the triangle
   if ((dots[0] <= 0 && dots[1] <= 0 && dots[2] <= 0) ||
       (dots[0] > 0 && dots[1] > 0 && dots[2] > 0))
   {
      return true;
   }
   return false;
}

int DotProduct(int x1, int y1, int x2, int y2)
{
   double dot = (x1 * x2) + (y1 * y2);
   return dot;
}

triangle GenerateRandomTriangle(int xBound[2], int yBound[2], std::default_random_engine& aRandEngine)
{
   std::uniform_int_distribution<int> xDist(xBound[0], xBound[1]);
   std::uniform_int_distribution<int> yDist(yBound[0], yBound[1]);

   triangle tri{
      {xDist(aRandEngine), yDist(aRandEngine)},
      {xDist(aRandEngine), yDist(aRandEngine)},
      {xDist(aRandEngine), yDist(aRandEngine)}
   };
   return tri;
}