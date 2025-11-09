#ifndef MESH_H
#define MESH_H


#include <cgnslib.h>
#include <string>
#include <vector>

namespace Mesh
{
    typedef struct Boundary
    {
        std::string name;
        CG_BCType_t type;
        CG_PointSetType_t pointset_type;
        cgsize_t number_of_points;
        int normal_index;
        cgsize_t normal_list_size;
        CG_DataType_t normal_data_type;
        std::vector<int> dataset;
    } Boundary;


    class Mesh
    {
    private:
        int file_index, base_index, zone_index;
        std::string zone_name;

        std::vector<std::vector<std::vector<double>>> coordinate_x, coordinate_y, coordinate_z;
        std::vector<Boundary> boundaries;

        cgsize_t mesh_size[3][3];
        cgsize_t mesh_first_index;
        cgsize_t mesh_last_index[3];
        int number_of_boundaries;

    public:
        Mesh(const std::string &mesh_file);
        ~Mesh();
    };
}


#endif