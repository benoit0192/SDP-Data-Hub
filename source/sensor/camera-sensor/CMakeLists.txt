set(LIBRARY_NAME CameraSensorLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

# Find OpenCV package
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Find Boost package
find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})


set(LIBRARY_SOURCES src/SimulatedCameraSensor.cpp)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC
                                ${CMAKE_CURRENT_SOURCE_DIR}/..
                                ${CMAKE_CURRENT_SOURCE_DIR}/template
                                ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link external libraries
target_link_libraries(${LIBRARY_NAME}
                                PUBLIC ${OpenCV_LIBS} 
                                       ${Boost_LIBRARIES}
                                       SensorLib)

add_executable(camera_grpc src/grpcMain.cpp)
target_link_libraries(camera_grpc PUBLIC ${LIBRARY_NAME})

add_executable(camera_test test/main.cpp)
target_link_libraries(camera_test PUBLIC ${LIBRARY_NAME})

install(TARGETS camera_test camera_grpc
    RUNTIME DESTINATION bin
)

#install(TARGETS ${LIBRARY_NAME} ${EXECUTABLE_NAME} 
#    LIBRARY  DESTINATION lib
#    ARCHIVE  DESTINATION lib
#    RUNTIME  DESTINATION bin
#    INCLUDES DESTINATION include
#)

#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
#        DESTINATION include)
