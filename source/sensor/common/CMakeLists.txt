set(LIBRARY_NAME SensorLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

add_library(${LIBRARY_NAME} INTERFACE)

target_include_directories(${LIBRARY_NAME} INTERFACE
                                ${CMAKE_CURRENT_SOURCE_DIR}/template
                                ${CMAKE_CURRENT_SOURCE_DIR}/include)

