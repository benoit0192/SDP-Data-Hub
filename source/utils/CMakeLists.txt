set(LIBRARY_NAME UtilsLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ABSL_PROPAGATE_CXX_STD ON)
add_compile_options(-Wall -Wextra)


set(LIBRARY_SOURCES src/Utils.cpp)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC
                                ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Testing ----------------------------------------------------------------------
#if (ENABLE_TESTING)
#    include(GoogleTest)
#    add_executable(${LIBRARY_NAME}-Test test/test.cpp)
#    target_link_libraries(${LIBRARY_NAME}-Test gtest_main ${LIBRARY_NAME})
#
#    gtest_discover_tests(${LIBRARY_NAME}-Test)
#endif()
# ------------------------------------------------------------------------------

