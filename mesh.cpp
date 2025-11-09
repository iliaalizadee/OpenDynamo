#include "mesh.hpp"
#include <iostream>


Mesh::Mesh::Mesh(const std::string &mesh_file)
{
    if (cg_open(mesh_file.c_str(), CG_MODE_READ, &file_index))
        std::cerr << cg_get_error() << std::endl;

    cg_zone_read(file_index, base_index, zone_index, "ZONE", mesh_size[0]);

    irmin = 1;
    irmax[0] = mesh_size[0][0];
    irmax[1] = mesh_size[0][1];
    irmax[2] = mesh_size[0][2];

    cg_coord_read(file_index, base_index, zone_index, "COORD_X", CGNS_ENUMV(RealDouble), &irmin, &irmax[0], coord_x.data());
    cg_coord_read(file_index, base_index, zone_index, "COORD_Y", CGNS_ENUMV(RealDouble), &irmin, &irmax[1], coord_y.data());
    cg_coord_read(file_index, base_index, zone_index, "COORD_Z", CGNS_ENUMV(RealDouble), &irmin, &irmax[2], coord_z.data());

    cg_close(file_index);
}

Mesh::Mesh::~Mesh()
{

}