#include <string>
#include <vector>

namespace swrast
{

using float3 = float[3];

struct face {
   size_t vertices[3];
   size_t normal;
};

class Model
{
public:
   Model() = default;
   Model(std::string aFile);
   ~Model();

   void LoadFromFile(std::string aFile);
//private:
   std::vector<float*> mVertices;
   std::vector<float*> mNormals;
   std::vector<face>   mFaces;
};
}