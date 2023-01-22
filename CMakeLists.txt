project(SimulatedCauchyOptimization)
cmake_minimum_required(VERSION 3.0)

add_compile_options( -fconcepts )
add_executable(main main.cpp)
add_executable(permutationGroup permutationGroup.cpp)
