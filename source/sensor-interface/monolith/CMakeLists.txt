set(LIBRARY_NAME MonolithSensorInterfaceLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ABSL_PROPAGATE_CXX_STD ON)
add_compile_options(-Wall -Wextra)

# External dependencies --------------------------------------------------------
find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})
# ------------------------------------------------------------------------------

set(LIBRARY_SOURCES src/SensorInterface.cpp)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})

add_dependencies(${LIBRARY_NAME} SensorLib)

# Link external libraries
target_link_libraries(${LIBRARY_NAME} PUBLIC
                                       ${Boost_LIBRARIES}
                                       UtilsLib
                                       SensorLib
)

target_include_directories(${LIBRARY_NAME} PUBLIC
  ${CMAKE_SOURCE_DIR}/source/utils/include
  ${CMAKE_SOURCE_DIR}/source/sensor/common/include
  ${CMAKE_SOURCE_DIR}/source/sensor-interface/common/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/template
)

# Executables ------------------------------------------------------------------
add_executable(sensor_interface_test test/main.cpp)
target_link_libraries(sensor_interface_test PUBLIC ${LIBRARY_NAME})
# ------------------------------------------------------------------------------

