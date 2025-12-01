#include <kokkos/Kokkos_Macros.hpp>
#include "mesh.hpp"
#include <kokkos/Kokkos_Core.hpp>
#include <print>
#include <stdexcept>

auto main(int argc, char* argv[]) -> int
{
    Kokkos::initialize(argc, argv);

    try 
    {
        OpenDynamo::Mesh<float> mesh("file.cgns");
    } 
    catch (std::runtime_error &e) 
    {
        std::println("{}",e.what());
        return -1;
    }

    {
        Kokkos::parallel_for("HelloKokkos", 5, KOKKOS_LAMBDA(int i) {
            std::println("Hello from iteration %d", i);
        });
    }

    Kokkos::finalize();

    return 0;
}
