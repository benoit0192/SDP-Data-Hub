set(LIBRARY_NAME CameraSensorLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ABSL_PROPAGATE_CXX_STD ON)
add_compile_options(-Wall -Wextra)

# External dependencies ========================================================
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Protobuf CONFIG REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

find_package(gRPC CONFIG REQUIRED)
include_directories(${gRPC_INCLUDE_DIRS})

# import "protobuf_generate_cpp function"
include(${Protobuf_DIR}/protobuf-module.cmake)

# gRPC files building =====#====================================================
set(PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/proto/camera-sensor.proto)

# Generate C++ source files from proto files
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

foreach(FIL ${PROTO_FILES})
    get_filename_component(FIL_WE ${FIL} NAME_WE)
    set(GRPC_PROTO_SRCS ${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc)
    set(GRPC_PROTO_HDRS ${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h)
    add_custom_command(
        OUTPUT ${GRPC_PROTO_SRCS} ${GRPC_PROTO_HDRS}
        COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
             --plugin=protoc-gen-grpc=`which grpc_cpp_plugin`
             --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
             -I ${CMAKE_CURRENT_SOURCE_DIR}/proto
             ${FIL}
        DEPENDS ${FIL}
        COMMENT "Generating gRPC source and header files from ${FIL}"
    )
    list(APPEND GRPC_SRCS ${GRPC_PROTO_SRCS})
    list(APPEND GRPC_HDRS ${GRPC_PROTO_HDRS})
endforeach()
# ==============================================================================


set(LIBRARY_SOURCES src/SimulatedRgbCamera.cpp
                    src/GrpcServer.cpp)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES}
                                   ${PROTO_SRCS}
                                   ${PROTO_HDRS}
                                   ${GRPC_SRCS}
                                   ${GRPC_HDRS})

target_include_directories(${LIBRARY_NAME} PUBLIC
                                ${CMAKE_CURRENT_SOURCE_DIR}/..
                                ${CMAKE_CURRENT_SOURCE_DIR}/template
                                ${CMAKE_CURRENT_SOURCE_DIR}/include
                                ${CMAKE_BINARY_DIR}/source/sensor/camera-sensor)

# Link external libraries
target_link_libraries(${LIBRARY_NAME}
                                PUBLIC ${OpenCV_LIBS} 
                                       ${Boost_LIBRARIES}
                                       gRPC::grpc++
                                       ${Protobuf_LIBRARIES}
                                       SensorLib)
# ==============================================================================

# Executables ==================================================================
add_executable(camera_grpc src/grpcMain.cpp)
target_link_libraries(camera_grpc PUBLIC ${LIBRARY_NAME})

add_executable(camera_test test/main.cpp)
target_link_libraries(camera_test PUBLIC ${LIBRARY_NAME})
# ==============================================================================


# Testing ======================================================================
if (ENABLE_TESTING)
    include(GoogleTest)
    add_executable(${LIBRARY_NAME}-Test test/test.cpp)
    target_link_libraries(${LIBRARY_NAME}-Test gtest_main ${LIBRARY_NAME})

    gtest_discover_tests(${LIBRARY_NAME}-Test)
endif()
# ==============================================================================

