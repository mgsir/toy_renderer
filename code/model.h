
#include <vector>

#include "geometry.h"
#include "tgaimage.h"

class Model {
    private:
        std::vector<Vec3f> verts_;
        std::vector<Vec3f> uv_;
        std::vector<Vec3f> norms_;
        std::vector<std::vector<Vec3i>> faces_;
        TGAImage diffusemap_;
        TGAImage normalmap_;
        TGAImage specularmap_;
        void load_texture(std::string filename, const char *suffix, TGAImage &img);

    public:
        Model(const char *filepath);
        ~Model();
        int nfaces();
        int nverts();
        
};