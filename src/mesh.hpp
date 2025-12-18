#pragma once


#include <cgnslib.h>
#include <cgnstypes.h>
#include <string>
#include <vector>
#include <array>
#include <cstdint>


namespace OpenDynamo
{
    struct Coordinate
    {
        std::string name;
        std::vector<double> vertices;
    };

    struct Boundary
    {
        std::string name;
        CG_BCType_t type;
        CG_PointSetType_t pointset_type;
        int64_t number_of_points;
        std::array<int32_t, 3> normal_index;
        int32_t number_of_datasets;
        std::vector<int64_t> point_indices;
        std::vector<double> normal_list;
    };

    
    struct Zone
    {
        int32_t index;
        std::string name;
        std::array<Coordinate, 3> coordinates;
        std::vector<Boundary> boundaries;
        std::array<int64_t, 9> size;

        explicit Zone(int32_t index_in) : index{index_in}{};
    };

    class Mesh
    {
    private:
        std::vector<Zone> zones;

    public:
        explicit Mesh() = default;
        explicit Mesh(std::vector<Zone> &&zones_in) : zones{std::move(zones_in)}{};
        ~Mesh() = default;
        Mesh(const Mesh &) = delete;
        Mesh &operator=(const Mesh &) = delete;
        Mesh(Mesh &&) = default;
        Mesh &operator=(Mesh &&) = default;
    };
}