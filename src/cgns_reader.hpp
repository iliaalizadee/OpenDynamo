#pragma once

#include "mesh.hpp"
#include <expected>
#include <string>
#include <system_error>

namespace OpenDynamo
{
    [[nodiscard]] auto read_cgns_mesh(const std::string &mesh_file) -> std::expected<Mesh, std::error_code>;
}