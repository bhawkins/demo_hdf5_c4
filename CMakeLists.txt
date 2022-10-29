cmake_minimum_required(VERSION 3.0.0)
project(demo_hdf5_c4 VERSION 0.1.0)

find_package(HDF5 REQUIRED COMPONENTS C)
include_directories(${HDF5_INCLUDE_DIRS})
add_executable(demo_hdf5_c4 src/main.c)
target_link_libraries(demo_hdf5_c4 ${HDF5_LIBRARIES})
