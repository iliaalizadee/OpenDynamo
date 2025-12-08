#include <iostream>
#include <kokkos/Kokkos_Macros.hpp>
#include "src/cgns_reader.hpp"
#include "src/file_utils.hpp"
#include <kokkos/Kokkos_Core.hpp>
#include <print>
#include <string>


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

    Kokkos::initialize(argc, argv);

    

    {
        Kokkos::parallel_for("HelloKokkos", 5, KOKKOS_LAMBDA(int i) {
            std::println("Hello from iteration %d", i);
        });
    }

    Kokkos::finalize();

    return 0;
}
