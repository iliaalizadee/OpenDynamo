#pragma once

#include "src/concepts.hpp"
#include "src/mesh.hpp"
#include "src/enums.hpp"
#include <utility>

namespace OpenDynamo
{
    struct Config
    {
        Precision precision;
        SimulationType simulation_type;
        explicit Config(Precision parsed_precision, SimulationType parsed_simtype) : precision{parsed_precision}, simulation_type{parsed_simtype}{};
    };

    template<Floating T>
    class Simulation
    {
    private:
        Config config;
        Mesh<T> mesh;

    public:
        explicit Simulation(Config parsed_config, Mesh<T> mesh) : config{std::move(parsed_config)}{};
        ~Simulation() = default;
        auto run() -> void;
        auto stop() -> void;
        auto run_simulation_learning() -> void;
        auto run_simulation_solving() -> void;
    };
}