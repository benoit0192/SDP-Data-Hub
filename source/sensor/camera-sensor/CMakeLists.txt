set(LIBRARY_NAME CameraSensorLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ABSL_PROPAGATE_CXX_STD ON)
add_compile_options(-Wall -Wextra)

# External dependencies --------------------------------------------------------
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})
# ------------------------------------------------------------------------------

set(LIBRARY_SOURCES src/SimulatedRgbCamera.cpp)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})

add_dependencies(${LIBRARY_NAME} SensorLib)

target_include_directories(${LIBRARY_NAME} PUBLIC
                               ${CMAKE_BINARY_DIR}/source/sensor/camera-sensor)

# Link external libraries
target_link_libraries(${LIBRARY_NAME} PUBLIC
                                       ${OpenCV_LIBS}
                                       ${Boost_LIBRARIES}
                                       UtilsLib
                                       SensorLib
)

target_include_directories(${LIBRARY_NAME} PUBLIC
  ${CMAKE_SOURCE_DIR}/source/utils/include
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_SOURCE_DIR}/template
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Include gRPC dependencies ----------------------------------------------------
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC     CONFIG REQUIRED)

add_library(CameraSensorProtos OBJECT
                         "${CMAKE_CURRENT_LIST_DIR}/proto/camera-sensor.proto")

target_link_libraries(CameraSensorProtos PUBLIC protobuf::libprotobuf
                                                gRPC::grpc++
                                                SensorProtos
)

set(PROTO_BINARY_DIR  "${CMAKE_CURRENT_BINARY_DIR}")
set(PROTO_IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/proto"
                      "${CMAKE_SOURCE_DIR}/source/sensor/common/proto"
)

target_include_directories(CameraSensorProtos PUBLIC
  "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>"
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/sensor/common/proto>"
)

protobuf_generate(
  TARGET         CameraSensorProtos
  IMPORT_DIRS    "${PROTO_IMPORT_DIRS}"
  PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
)

protobuf_generate(
  TARGET              CameraSensorProtos
  LANGUAGE            grpc
  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
  PLUGIN              "protoc-gen-grpc=\$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
  IMPORT_DIRS         "${PROTO_IMPORT_DIRS}"
  PROTOC_OUT_DIR      "${PROTO_BINARY_DIR}"
)

target_link_libraries(${LIBRARY_NAME} PUBLIC SensorProtos
                                             CameraSensorProtos
)
# ------------------------------------------------------------------------------

# Executables ------------------------------------------------------------------
add_executable(camera_grpc src/GrpcServer.cpp)
target_link_libraries(camera_grpc PUBLIC ${LIBRARY_NAME})

add_executable(camera_test test/main.cpp)
target_link_libraries(camera_test PUBLIC ${LIBRARY_NAME})
# ------------------------------------------------------------------------------

# Should be added only for debugging
add_executable(camera_grpc_client test/GrpcClient.cpp)
target_link_libraries(camera_grpc_client PUBLIC ${LIBRARY_NAME})

# Testing ----------------------------------------------------------------------
if (ENABLE_TESTING)
    include(GoogleTest)
    add_executable(${LIBRARY_NAME}-Test test/test.cpp)
    target_link_libraries(${LIBRARY_NAME}-Test gtest_main ${LIBRARY_NAME})

    gtest_discover_tests(${LIBRARY_NAME}-Test)
endif()
# ------------------------------------------------------------------------------

