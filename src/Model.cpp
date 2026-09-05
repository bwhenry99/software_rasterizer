#include "Model.hpp"

#include <fstream>
#include <filesystem>
#include <sstream>
#include <string_view>

swrast::Model::Model(std::string aFileName)
{
   LoadFromFile(aFileName);
}

swrast::Model::~Model()
{
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
         mVertices.push_back(new float3{x,y,z});
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