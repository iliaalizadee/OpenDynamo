#pragma once

#include "mesh.hpp"
#include <expected>
#include <string>
#include <system_error>

namespace OpenDynamo
{
    template<Floating T>
    [[nodiscard]] auto read_cgns_mesh(const std::string &mesh_file) -> std::expected<Mesh<T>, std::error_code>;
}