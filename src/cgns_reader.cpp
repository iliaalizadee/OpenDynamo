#include "cgns_reader.hpp"
#include "mesh.hpp"
#include <cgnslib.h>
#include <cstdint>
#include <system_error>
#include <vector>

namespace OpenDynamo
{
    template<Floating T>
    auto read_cgns_mesh(const std::string &mesh_file) -> std::expected<Mesh<T>, std::error_code>
    {
        const int32_t base_index{1};

        int32_t file_index;
        if (cg_open(mesh_file.c_str(), CG_MODE_READ, &file_index) != CG_OK)
            return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));

        int32_t number_of_bases;
        if (cg_nbases(file_index, &number_of_bases) != CG_OK)
        {
            cg_close(file_index);
            return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));
        }

        if (number_of_bases != 1)
        {
            cg_close(file_index);
            return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));
        }

        int32_t number_of_zones;
        if (cg_nzones(file_index, base_index, &number_of_zones) != CG_OK)
        {
            cg_close(file_index);
            return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));
        }

        std::vector<Zone<T>> zones;
        for (auto i = 0; i < number_of_zones; i++)
        {
            Zone<T> &zone = zones.emplace_back(i + 1);
            zone.name.resize(1024);

            if (cg_zone_read(file_index, base_index, i + 1, zone.name.data(), zone.size.data()) != CG_OK)
            {
                cg_close(file_index);
                return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));
            }

            int32_t number_of_coordinate_arrays;
            if (cg_ncoords(file_index, base_index, zone.index, &number_of_coordinate_arrays) != CG_OK)
            {
                cg_close(file_index);
                return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));
            }

            if (number_of_coordinate_arrays != 3)
            {
                cg_close(file_index);
                return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));
            }

            for (auto j = 0; j < number_of_coordinate_arrays; j++)
            {
                zone.coordinates.at(j).name.resize(1024);

                if (cg_coord_info(file_index, base_index, zone.index, j + 1, nullptr, zone.coordinates.at(j).name.data()) != CG_OK)
                {
                    cg_close(file_index);
                    return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));
                }

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
                    return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));
                }
            }

            int32_t number_of_boundaries;
            if (cg_nbocos(file_index, base_index, zone.index, &number_of_boundaries) != CG_OK)
            {
                cg_close(file_index);
                return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));
            }

            for (auto j = 0; j < number_of_boundaries; j++)
            {
                Boundary<T> &boundary = zone.boundaries.emplace_back();
                boundary.name.resize(1024);
                int64_t normal_list_size;

                if (cg_boco_info(file_index, 
                            base_index, 
                            zone.index, 
                            j+1, 
                            boundary.name.data(), 
                            &boundary.type, 
                            &boundary.pointset_type,
                            &boundary.number_of_points,
                            boundary.normal_index.data(),
                            &normal_list_size,
                            nullptr,
                            &boundary.number_of_datasets
                ) != CG_OK)
                {
                    cg_close(file_index);
                    return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));
                }

                boundary.point_indices.resize(boundary.number_of_points * 3);
                boundary.normal_list.resize(normal_list_size);

                if (cg_boco_read(file_index, 
                            base_index, 
                            zone.index, 
                            j + 1, 
                            boundary.point_indices.data(), 
                            boundary.normal_list.data()
                ) != CG_OK)
                {
                    cg_close(file_index);
                    return std::unexpected(std::make_error_code(std::errc::bad_file_descriptor));
                }
            }
        }
        cg_close(file_index);
        return Mesh<T>{std::move(zones)};
    }
}