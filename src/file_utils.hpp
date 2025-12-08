#pragma once

#include "src/simulation.hpp"
#include <expected>
#include <filesystem>
#include <string>
#include <system_error>

namespace OpenDynamo
{
    [[nodiscard]] auto check_and_prepare_path(const std::string &path_string) -> std::expected<std::filesystem::path, std::error_code>;
    [[nodiscard]] auto read_and_parse_config(const std::filesystem::path &config_path) -> std::expected<Config, std::error_code>;
}