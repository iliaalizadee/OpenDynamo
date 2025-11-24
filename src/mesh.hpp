#ifndef MESH_H
#define MESH_H


#include <cgnslib.h>
#include <string>
#include <vector>
#include <array>


namespace Mesh
{
    typedef struct
    {
        std::string name;
        CG_BCType_t type;
        CG_PointSetType_t pointset_type;
        cgsize_t number_of_points;
        std::array<int, 3> normal_index;
        std::array<float, 3> normal_vector;
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
        std::array<std::array<cgsize_t, 3>, 3> mesh_size;
        cgsize_t mesh_first_index;
        std::array<cgsize_t, 3> mesh_last_index;
        int number_of_boundaries;


    public:
        Mesh(const std::string &mesh_file);
        ~Mesh();
    };
}


#endif