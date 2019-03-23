# Copyright 2017 Cleuton Sampaio de Melo Junior - O Bom Programador
# http://obomprogramador.com
# Instrucoes:
# Cleuton Sampaio, 2017
#   mkdir build
#   cd build
#   cmake ..
#   cmake --build . --config Release
cmake_minimum_required(VERSION 2.8.12)
project(hogcomparator)
find_package( OpenCV REQUIRED )
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(./dlib dlib_build)
add_library(hogcomparator SHARED com_obomprogramador_hog_HogComparator.cpp)
include_directories(/usr/lib/jvm/java-11-oracle/include /usr/lib/jvm/java-11-oracle/include/linux ${OpenCV_INCLUDE_DIRS})
target_link_libraries(hogcomparator dlib::dlib)
target_link_libraries(hogcomparator ${OpenCV_LIBS} )





