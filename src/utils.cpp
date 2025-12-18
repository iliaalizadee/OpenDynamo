#include "utils.hpp"
#include <expected>
#include <filesystem>
#include <fstream>
#include <system_error>

namespace OpenDynamo
{
    auto check_and_prepare_path(const std::string &path_string) -> std::expected<std::filesystem::path, std::error_code>
    {
        std::error_code fs_error_code;
        std::filesystem::path path{std::filesystem::absolute(path_string, fs_error_code)};
        if (fs_error_code)
            return std::unexpected(fs_error_code);

        if (!std::filesystem::exists(path, fs_error_code))
            return std::unexpected(fs_error_code);

        if (!std::filesystem::is_directory(path, fs_error_code))
            return std::unexpected(fs_error_code);

        std::filesystem::directory_iterator fs_iterator(path, fs_error_code);
        if (fs_error_code)
            return std::unexpected(fs_error_code);

        std::filesystem::path test_file = path / ".permission_test.tmp";
    
        std::ofstream ofs{test_file.string(), std::ios::out | std::ios::trunc};
        if (!ofs.good())
            return std::unexpected(std::make_error_code(std::errc::permission_denied));
    
        std::filesystem::remove(test_file, fs_error_code);

        return path;
    }
}