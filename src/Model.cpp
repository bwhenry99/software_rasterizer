#include "Model.hpp"

#include <fstream>
#include <filesystem>
#include <sstream>
#include <string_view>

float ClampAngle0_360(float aAngle)
{
   float angle = std::fmod(aAngle, 360.0f);
   if (angle < 0.0f)
   {
      angle += 360.0;
   }
   return angle;
}

constexpr float cPI = 3.14159f;

swrast::Model::Model(std::string aFileName)
{
   LoadFromFile(aFileName);
}

swrast::Model::~Model()
{
   for (size_t i = 0; i < mStaticVertices.size(); i++)
   {
      delete mStaticVertices[i];
   }

   for (size_t i = 0; i < mVertices.size(); i++)
   {
      delete mVertices[i];
   }

   for (size_t i = 0; i < mNormals.size(); i++)
   {
      delete mNormals[i];
   }
}

void swrast::Model::LoadFromFile(std::string aFileName)
{
   std::filesystem::path filepath(aFileName);

   // open file
   std::ifstream filestream(filepath);
   std::string line;

   while (std::getline(filestream, line))
   {
      std::stringstream stream(line);
      std::string prefix;
      stream >> prefix;
      if (prefix == "#")
      {
         // skip comments
         continue;
      }

      if (prefix == "v")
      {
         // parse out vertex point
         float x, y, z;
         stream >> x >> y >> z;
         mStaticVertices.push_back(new float3{x,y,z});
         mVertices.push_back(new float3{ x,y,z });
         continue;
      }

      if (prefix == "vn")
      {
         //need to save normals
         // parse out vertex point
         float x, y, z;
         stream >> x >> y >> z;
         mNormals.push_back(new float3{x,y,z});
         continue;
      }

      if (prefix == "vt")
      {
         // I'll probably want to handle texture corrdinates at some point
         continue;
      }

      if (prefix == "f")
      {
         std::string vert;
         
         std::vector<size_t> vertexIndicies;
         while ((stream >> std::ws).peek() != EOF)
         {
            stream >> vert;
            std::string ind = (vert.substr(0, vert.find_first_of("/")));
            vertexIndicies.push_back(std::stoi(ind));
            //TODO: get normal and texture info
         }
         if (vertexIndicies.size() < 3)
         {
            //maybe throw exception here
            continue; //just skip for now
         }
         
         mFaces.push_back({ {vertexIndicies[0] - 1, vertexIndicies[1] - 1, vertexIndicies[2] - 1}, 0 });

         if (vertexIndicies.size() > 3)
         {
            for (size_t i = 3; i < vertexIndicies.size(); i++)
            {
               // this is kind of a nested nightmare...
               mFaces.push_back({ {vertexIndicies[0] - 1, vertexIndicies[i - 1] - 1, vertexIndicies[i] - 1}, 0 });
            }
         }
      }
   }
}

void swrast::Model::SetYawDeg(float aYaw)
{
   mRotation[1] = ClampAngle0_360(aYaw) / (2 * cPI);
}

void swrast::Model::UpdateOrientation()
{
   // TODO I'm only implements yaw right now
   for (size_t i = 0; i < mStaticVertices.size(); i++)
   {
      mVertices[i][0] =  mStaticVertices[i][0] * cos(mRotation[1]) + mStaticVertices[i][2] * sin(mRotation[1]);
      mVertices[i][2] = -mStaticVertices[i][0] * sin(mRotation[1]) + mStaticVertices[i][2] * cos(mRotation[1]);
   }
}