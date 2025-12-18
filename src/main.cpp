#include <expected>
#include <iostream>
#include <kokkos/Kokkos_Macros.hpp>
#include "utils.hpp"
#include "src/simulation.hpp"
#include <kokkos/Kokkos_Core.hpp>
#include <print>
#include <string>
#include <system_error>
#include <utility>


auto main(int argc, char* argv[]) -> int
{
    if (argc < 2)
    {
        std::println(std::cerr, "Error: No simulation path provided");
        return 1;
    }

    auto path{OpenDynamo::check_and_prepare_path(std::string{argv[1]})};
    if (!path)
    {
        std::println(std::cerr, "{}", path.error().message());
        return 1;
    }

    auto config{OpenDynamo::read_and_parse_config(path.value())};
    if (!config)
    {
        std::println(std::cerr, "{}", config.error().message());
        return 1;
    }

    OpenDynamo::Simulation simulation{std::move(config.value())};

    auto mesh_result{simulation.read_mesh()};
    if(!mesh_result)
    {
        std::println(std::cerr, "{}", mesh_result.error().message());
        return 1;
    }

    Kokkos::initialize(argc, argv);

    simulation.run();

    Kokkos::finalize();
    return 0;
}
