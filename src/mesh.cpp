#include "mesh.hpp"
#include <iostream>
#include <exception>


Mesh::Mesh::Mesh(const std::string &mesh_file) : base_index(1), zone_index(1), mesh_first_index(1)
{
    if (cg_open(mesh_file.c_str(), CG_MODE_READ, &file_index))
        throw std::runtime_error("Could not open mesh file");

    cg_zone_read(file_index, base_index, zone_index, zone_name.data(), mesh_size[0].data());

    mesh_last_index[0] = mesh_size[0][0];
    mesh_last_index[1] = mesh_size[0][1];
    mesh_last_index[2] = mesh_size[0][2];

    cg_coord_read(file_index, base_index, zone_index, "COORD_X", CGNS_ENUMV(RealDouble), &mesh_first_index, &mesh_last_index[0], coordinate_x.data());
    cg_coord_read(file_index, base_index, zone_index, "COORD_Y", CGNS_ENUMV(RealDouble), &mesh_first_index, &mesh_last_index[1], coordinate_y.data());
    cg_coord_read(file_index, base_index, zone_index, "COORD_Z", CGNS_ENUMV(RealDouble), &mesh_first_index, &mesh_last_index[2], coordinate_z.data());

    cg_nbocos(file_index, base_index, zone_index, &number_of_boundaries);

    for (int boundary_index = 1; boundary_index <= number_of_boundaries; boundary_index++)
    {
        Boundary boundary;
        cg_boco_info(file_index, 
            base_index, 
            zone_index, 
            boundary_index, 
            boundary.name.data(), 
            &boundary.type, 
            &boundary.pointset_type,
            &boundary.number_of_points,
            boundary.normal_index.data(),
            boundary.normal_vector.data(),
            &boundary.normal_data_type,
            boundary.dataset.data()
        );
        
    }

    cg_close(file_index);
}

Mesh::Mesh::~Mesh()
{

}