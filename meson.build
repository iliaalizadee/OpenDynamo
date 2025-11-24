project('OpenDynamo', 'cpp', default_options : ['cpp_std=c++23', 'buildtype=debug'])

kokkos_dep = custom_target(
  'build_kokkos',
  output: 'kokkos.a',  # or .so depending on library
  command: [
    'cmake',
    '-S', 'third_party/kokkos',
    '-B', 'third_party/kokkos/build',
    '-DCMAKE_CXX_COMPILER=clang++',
    '-DCMAKE_CXX_STANDARD=23',
    '-DCMAKE_BUILD_TYPE=Debug',
    '-DKokkos_ENABLE_OPENMP=ON',
    '-DKokkos_ARCH_NATIVE=ON',
    '-DKokkos_ENABLE_CUDA=ON',
    '-DKokkos_ENABLE_SYCL=ON'
  ] + [
    '&&', 'cmake', '--build', 'third_party/kokkos/build', '-j$(nproc)'
  ] + [
    '&&', 'cmake', '--install', 'third_party/kokkos/build', '--prefix', 'third_party/kokkos/install/'
  ],
  capture: true
)

kokkos_lib = declare_dependency(
  link_with: files('third_party/kokkos/install/libkokkos.a'),
  include_directories: include_directories('third_party/kokkos/install/include')
)


sources = files('src/*')
executable('OpenDynamo', sources, dependencies: [kokkos_lib])