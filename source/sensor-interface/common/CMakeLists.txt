set(LIBRARY_NAME SensorInterfaceLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

set(LIBRARY_SOURCES src/AbstractSensorInterface.cpp)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})

target_link_libraries(${LIBRARY_NAME} PUBLIC UtilsLib)

target_include_directories(${LIBRARY_NAME} PUBLIC
                                         ${CMAKE_CURRENT_SOURCE_DIR}/template
                                         ${CMAKE_CURRENT_SOURCE_DIR}/include)

