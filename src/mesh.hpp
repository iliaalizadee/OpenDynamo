#pragma once


#include <cgnslib.h>
#include <cgnstypes.h>
#include <concepts>
#include <string>
#include <vector>
#include <array>
#include <cstdint>


namespace OpenDynamo
{
    template<typename T>
    concept Floating = std::same_as<T, float> || std::same_as<T, double>;

    template<Floating T>
    struct Coordinate
    {
        std::string name;
        std::vector<T> vertices;
    };

    template<Floating T>
    struct Boundary
    {
        std::string name;
        CG_BCType_t type;
        CG_PointSetType_t pointset_type;
        int64_t number_of_points;
        std::array<int32_t, 3> normal_index;
        int64_t normal_list_size;
        int32_t number_of_datasets;
        std::vector<int64_t> point_indices;
        std::vector<T> normal_list;
    };

    template<Floating T>
    struct Zone
    {
        int32_t index;
        std::string name;
        std::array<Coordinate<T>, 3> coordinates;
        std::vector<Boundary<T>> boundaries;
        std::array<int64_t, 9> size;
        int32_t number_of_coordinate_arrays;
        int32_t number_of_boundaries;
        Zone(int32_t index) : index{index}{};
    };

    template<Floating T>
    class Mesh
    {
    private:
        int32_t file_index;
        const int32_t base_index;
        int32_t number_of_bases, number_of_zones;
        std::vector<Zone<T>> zones;

    public:
        explicit Mesh(const std::string &mesh_file);
        ~Mesh();   
    };
}