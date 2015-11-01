cmake_minimum_required(VERSION 2.8.4)
project(sypexgeo_vyvid)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -Wall -fcxx-exceptions -fexceptions")

set(
    SOURCE_FILES
    tests/test.cpp
    src/xyz/vyvid/sypexgeo/db.h
    src/xyz/vyvid/sypexgeo/db.cc
    src/xyz/vyvid/sypexgeo/location.h
    src/xyz/vyvid/sypexgeo/location.cc
    src/xyz/vyvid/sypexgeo/errors.h
    src/xyz/vyvid/sypexgeo/header.h
    src/xyz/vyvid/sypexgeo/raw_city_access.h
    src/xyz/vyvid/sypexgeo/raw_country_access.h
    src/xyz/vyvid/sypexgeo/raw_region_access.h
    src/xyz/vyvid/util/string_builder.h
    src/xyz/vyvid/util/uint24_t.h

    node_modules/nan/nan.h
)

include_directories(
    src
    ~/.node-gyp/4.2.1/include/node
    node_modules/nan
)

add_executable(sypexgeo_vyvid ${SOURCE_FILES})