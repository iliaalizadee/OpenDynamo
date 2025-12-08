#pragma once


#include "src/concepts.hpp"
#include <cgnslib.h>
#include <cgnstypes.h>
#include <string>
#include <vector>
#include <array>
#include <cstdint>


namespace OpenDynamo
{
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
        explicit Zone(int32_t zone_index) : index{zone_index}{};
    };

    template<Floating T>
    class Mesh
    {
    private:
        std::vector<Zone<T>> zones;

    public:
        explicit Mesh(std::vector<Zone<T>>&& zones) : zones{std::move(zones)}{};
        ~Mesh() = default;
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&&) = default;
        Mesh& operator=(Mesh&&) = default;
    };
}