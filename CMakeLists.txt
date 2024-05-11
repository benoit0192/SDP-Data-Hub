cmake_minimum_required(VERSION 3.12)

project(Sensor-Data-Platform)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" 
        CACHE PATH 
        "Installation prefix" 
        FORCE)
endif()

add_subdirectory(source)

