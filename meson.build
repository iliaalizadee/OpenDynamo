project('OpenDynamo', 'cpp', default_options : ['c_std=c23', 'cpp_std=c++23', 'buildtype=debug', 'cpp_args=-fopenmp'])


kokkos_dep = dependency('Kokkos', method: 'cmake')


cgns_inc = include_directories('/usr/include')

cc = meson.get_compiler('cpp')
cgns_lib = cc.find_library('cgns')


sources = files('src/main.cpp', 'src/mesh.cpp', 'src/mesh.hpp')
executable('OpenDynamo', sources, include_directories: [cgns_inc], dependencies: [kokkos_dep, cgns_lib])