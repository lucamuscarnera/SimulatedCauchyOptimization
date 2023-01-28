project(SimulatedCauchyOptimization LANGUAGES CXX)
cmake_minimum_required(VERSION 3.0)

add_compile_options( -fconcepts )

find_package(OpenMP REQUIRED)


add_executable(main main.cpp)
add_executable(GreedyBenchmark GreedyBenchmark.cpp)

target_link_libraries(GreedyBenchmark PRIVATE OpenMP::OpenMP_CXX)