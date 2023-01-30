project(SimulatedCauchyOptimization LANGUAGES CXX)
cmake_minimum_required(VERSION 3.0)

add_compile_options( -fconcepts )

find_package(OpenMP REQUIRED)


add_executable(main main.cpp)
add_executable(test1 travelling_salesman.cpp)

target_link_libraries(test1 PRIVATE OpenMP::OpenMP_CXX)