set(LIBRARY_NAME SensorLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

add_library(${LIBRARY_NAME} INTERFACE)

target_include_directories(${LIBRARY_NAME} INTERFACE
                                         ${CMAKE_CURRENT_SOURCE_DIR}/template
                                         ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include gRPC dependencies ----------------------------------------------------
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC     CONFIG REQUIRED)

add_library(SensorProtos OBJECT "${CMAKE_CURRENT_LIST_DIR}/proto/sensor.proto")

target_link_libraries(SensorProtos PUBLIC protobuf::libprotobuf
                                          gRPC::grpc++
)

set(PROTO_BINARY_DIR  "${CMAKE_CURRENT_BINARY_DIR}")
set(PROTO_IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/proto")

target_include_directories(SensorProtos PUBLIC
                                      "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>"
)

protobuf_generate(
  TARGET         SensorProtos
  IMPORT_DIRS    "${PROTO_IMPORT_DIRS}"
  PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
)

protobuf_generate(
  TARGET              SensorProtos
  LANGUAGE            grpc
  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
  PLUGIN              "protoc-gen-grpc=\$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
  IMPORT_DIRS         "${PROTO_IMPORT_DIRS}"
  PROTOC_OUT_DIR      "${PROTO_BINARY_DIR}"
)

# Comment out the following to preserve the protos as being an independent module
#target_sources(${LIBRARY_NAME} INTERFACE
#  $<TARGET_OBJECTS:SensorProtos>
#)
#
#target_link_libraries(${LIBRARY_NAME} INTERFACE SensorProtos
#)
#
#target_include_directories(${LIBRARY_NAME} INTERFACE
#  "$<TARGET_PROPERTY:SensorProtos,INTERFACE_INCLUDE_DIRECTORIES>"
#)
# ------------------------------------------------------------------------------
