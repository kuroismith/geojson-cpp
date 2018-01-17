cmake_minimum_required(VERSION 3.4)
project(geojson_cpp)

set(CMAKE_CXX_STANDARD 14)
    
set(LIBRARY_NAME geojson_cpp)
set(UNITTEST_NAME unittest)

file(GLOB INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gago/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gago/geometry/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gago/geojson/*.h
        )
file(GLOB TEST_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cc)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor)

add_executable(${UNITTEST_NAME} ${TEST_SRC})
