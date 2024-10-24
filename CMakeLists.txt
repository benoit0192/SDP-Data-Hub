cmake_minimum_required(VERSION 3.15)

project(Sensor-Data-Platform)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)
# gRPC related feature
set(ABSL_PROPAGATE_CXX_STD ON)

add_compile_options(-Wall -Wextra)

option(ENABLE_TESTING "Enable testing" OFF)

include(${CMAKE_SOURCE_DIR}/cmake/googletest.cmake)
# include(${CMAKE_SOURCE_DIR}/cmake/grpc.cmake)

add_subdirectory(source)

