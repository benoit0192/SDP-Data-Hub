set(LIBRARY_NAME UtilsLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ABSL_PROPAGATE_CXX_STD ON)
add_compile_options(-Wall -Wextra)


set(LIBRARY_SOURCES src/Utils.cpp
                    src/AtomicPtrSwap.cpp)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC
                                ${CMAKE_CURRENT_SOURCE_DIR}/include)

