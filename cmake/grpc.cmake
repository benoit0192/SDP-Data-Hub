
include(FetchContent)

FetchContent_Declare(
  grpc
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG v1.64.2
)

set(gRPC_BUILD_TESTS OFF)
set(gRPC_INSTALL OFF)
set(gRPC_BUILD_CODEGEN OFF)

FetchContent_MakeAvailable(grpc)

FetchContent_GetProperties(grpc)
if(NOT grpc_POPULATED)
  FetchContent_Populate(grpc)
  add_subdirectory(${grpc_SOURCE_DIR} ${grpc_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
