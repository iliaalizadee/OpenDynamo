#pragma once

#include "src/mesh.hpp"
#include "src/enums.hpp"
#include <array>
#include <expected>
#include <filesystem>
#include <system_error>
#include <utility>


namespace OpenDynamo
{
    struct Physics
    {
        std::array<double, 3> v;
        double p;
        double T;
        double rho, rho_e;
        std::array<double, 3> psi;
    };

    struct Config
    {
        Physics physics;
        Precision precision;
        SimulationType simulation_type;
        std::filesystem::path path_to_mesh;
    };

    class Simulation
    {
    private:
        Config config;
        Mesh mesh;
        bool is_simulation_initialized;

    public:
        explicit Simulation(const Config &&config_in) : config{std::move(config_in)}, is_simulation_initialized{false}{};
        ~Simulation() = default;

        Simulation(const Simulation &) = delete;
        Simulation &operator=(const Simulation &) = delete;
        Simulation(Simulation &&) = default;
        Simulation &operator=(Simulation &&) = default;

        [[nodiscard]] auto read_mesh() -> std::expected<void, std::error_code>;
        [[nodiscard]] auto get_config() -> Config &;

        auto run() -> std::expected<void, std::error_code>;
        auto stop() -> std::expected<void, std::error_code>;
    };
}