#ifndef MESH_H
#define MESH_H
#include <cgnslib.h>
#include <string>
#include <vector>

namespace Mesh
{
    class Mesh
    {
    private:
        int file_index, base_index, zone_index;
        std::vector<std::vector<std::vector<double>>> coord_x, coord_y, coord_z;
        cgsize_t mesh_size[3][3];
        cgsize_t irmin;
        cgsize_t irmax[3];

    public:
        Mesh(const std::string &mesh_file);
        ~Mesh();
    }; 
}
#endif