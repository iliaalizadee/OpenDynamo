cmake_minimum_required(VERSION 3.10.0)
project(OpenDynamo VERSION 0.1.0 LANGUAGES C CXX)

add_executable(OpenDynamo main.cpp mesh.hpp mesh.cpp)
find_package(Kokkos REQUIRED CONFIG)

find_library(cgns NAMES cgns PATHS /usr/lib64 /usr/lib)

target_link_libraries(OpenDynamo PRIVATE Kokkos::kokkos)
target_link_libraries(OpenDynamo PRIVATE cgns)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
