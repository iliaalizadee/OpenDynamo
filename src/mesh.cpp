#include "mesh.hpp"
#include <cgnslib.h>
#include <cstdint>
#include <cstring>
#include <format>
#include <stdexcept>
#include <strings.h>
#include <vector>


template<OpenDynamo::Floating T>
OpenDynamo::Mesh<T>::Mesh(const std::string &mesh_file) : base_index{1}
{
    
    if (cg_open(mesh_file.c_str(), CG_MODE_READ, &file_index) != CG_OK)
        throw std::runtime_error(std::format("Could not open mesh file {}", mesh_file));

    if (cg_nbases(file_index, &number_of_bases) != CG_OK)
    {
        cg_close(file_index);
        throw std::runtime_error("Invalid base count");
    }

    if (number_of_bases != 1)
    {
        cg_close(file_index);
        throw std::runtime_error("Only single-base meshes are supported");
    }

    if (cg_nzones(file_index, base_index, &number_of_zones) != CG_OK)
    {
        cg_close(file_index);
        throw std::runtime_error("Invalid zone count");
    }

    for (int32_t i = 0; i < number_of_zones; i++)
    {
        Zone<T> &zone = zones.emplace_back(i + 1);
        zone.name.resize(1024);

        if (cg_zone_read(file_index, base_index, i + 1, zone.name.data(), zone.size.data()) != CG_OK)
        {
            cg_close(file_index);
            throw std::runtime_error(std::format("Could not read zone {}", zone.index));
        }

        zone.name.resize(strlen(zone.name.data()));

        if (cg_ncoords(file_index, base_index, zone.index, &zone.number_of_coordinate_arrays) != CG_OK)
        {
            cg_close(file_index);
            throw std::runtime_error(std::format("Could not read the number of coordinate arrays for zone {}", zone.name));
        }

        if (zone.number_of_coordinate_arrays != 3)
        {
            cg_close(file_index);
            throw std::runtime_error(std::format("Zone {} is not 3D", zone.name));
        }

        for (int32_t j = 0; j < zone.number_of_coordinate_arrays; j++)
        {
            zone.coordinates.at(j).name.resize(1024);

            if (cg_coord_info(file_index, base_index, zone.index, j + 1, nullptr, zone.coordinates.at(j).name.data()) != CG_OK)
            {
                cg_close(file_index);
                throw std::runtime_error(std::format("Could not read the number of coordinate arrays for zone {}", zone.name));
            }
            
            zone.coordinates.at(j).name.resize(strlen(zone.coordinates.at(j).name.data()));

            const int64_t number_of_vertices{zone.size.at(0) * zone.size.at(1) * zone.size.at(2)};
            zone.coordinates.at(j).vertices.resize(number_of_vertices);

            if (cg_coord_read(file_index, 
                        base_index,
                        zone.index, 
                        zone.coordinates.at(j).name.c_str(), 
                        CG_RealDouble, 
                        std::array<int64_t, 3>{1,1,1}.data(), 
                        zone.size.data(), 
                        zone.coordinates.at(j).vertices.data()
            ) != CG_OK)
            {
                cg_close(file_index);
                throw std::runtime_error(std::format("Could not read vertex data for zone {}", zone.name));
            }
        }

        if (cg_nbocos(file_index, base_index, zone.index, &zone.number_of_boundaries) != CG_OK)
        {
            cg_close(file_index);
            throw std::runtime_error(std::format("Could not read the number of boundary conditions for zone {}", zone.name));
        }

        for (int32_t j = 0; j < zone.number_of_boundaries; j++)
        {
            Boundary<T> &boundary = zone.boundaries.emplace_back();
            boundary.name.resize(1024);

            if (cg_boco_info(file_index, 
                        base_index, 
                        zone.index, 
                        j+1, 
                        boundary.name.data(), 
                        &boundary.type, 
                        &boundary.pointset_type,
                        &boundary.number_of_points,
                        boundary.normal_index.data(),
                        &boundary.normal_list_size,
                        nullptr,
                        &boundary.number_of_datasets
            ) != CG_OK)
            {
                cg_close(file_index);
                throw std::runtime_error(std::format("Could not read BC index {} info in zone {}", j + 1, zone.name));
            }

            boundary.name.resize(strlen(boundary.name.data()));
            boundary.point_indices.resize(boundary.number_of_points * 3);
            boundary.normal_list.resize(boundary.normal_list_size);

            if (cg_boco_read(file_index, 
                        base_index, 
                        zone.index, 
                        j + 1, 
                        boundary.point_indices.data(), 
                        boundary.normal_list.data()
            ) != CG_OK)
            {
                cg_close(file_index);
                throw std::runtime_error(std::format("Could not read BC index {} data in zone {}", j + 1, zone.name));
            }
        }
    }
}

template<OpenDynamo::Floating T>
inline OpenDynamo::Mesh<T>::~Mesh()
{
    cg_close(file_index);
}