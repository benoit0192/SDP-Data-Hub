set(LIBRARY_NAME PlatformLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

# External dependencies --------------------------------------------------------
find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})
# ------------------------------------------------------------------------------

set(LIBRARY_SOURCES src/Platform.cpp)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})

add_dependencies(${LIBRARY_NAME} SensorInterfaceLib)

# Link external libraries
target_link_libraries(${LIBRARY_NAME} PUBLIC
                                       ${Boost_LIBRARIES}
                                       UtilsLib
                                       SensorInterfaceLib
)

target_include_directories(${LIBRARY_NAME} PUBLIC
  ${CMAKE_SOURCE_DIR}/source/utils/include
  ${CMAKE_SOURCE_DIR}/source/sensor-interface/common/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Executables ------------------------------------------------------------------
add_executable(platform_test test/multi-sensors.cpp)
target_link_libraries(platform_test PUBLIC ${LIBRARY_NAME}
                                           CameraSensorLib
                                           MonolithSensorInterfaceLib)
# ------------------------------------------------------------------------------

