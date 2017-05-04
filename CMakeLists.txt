# Copyright (c) 2015-2017, David Hirvonen, Ruslan Baratov
# All rights reserved.

# WIP C++ interface + serialization for xgboost library
# NOTE: This is using the xgboost fork in the hunter
# package manager which has some non-standard extensions
# for cross platform and size reduced serialization.
# It also uses OpenMP, whereas the lastest versions
# uses a C++11 thread interface.

# TODO: Add a more recent xgboost to hunter

cmake_minimum_required(VERSION 3.0)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.18.44.tar.gz"
    SHA1 "a78f0b377b8e53c038f12fc18b0c02564c4534c8"
    FILEPATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Hunter/config.cmake"    
    )

set(XGBOOSTCPP_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

project(xgboostcpp VERSION 0.1.0) # WIP!

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(XGBOOSTER_BUILD_TESTS "Build tests" ON)
option(XGBOOSTER_SERIALIZE_WITH_BOOST "Serialize w/ boost" ON)
option(XGBOOSTER_SERIALIZE_WITH_CEREAL "Serialize w/ cereal" ON)

###################
## Dependencies ###
###################

hunter_add_package(xgboost)
find_package(xgboost CONFIG REQUIRED)

if(XGBOOSTER_SERIALIZE_WITH_BOOST)
  hunter_add_package(boost-pba)
  find_package(boost-pba CONFIG REQUIRED)
endif()

if(XGBOOSTER_SERIALIZE_WITH_CEREAL)
  hunter_add_package(cereal)
  find_package(cereal CONFIG REQUIRED)
endif()


if(NOT XCODE AND NOT MSVC) # We'll never have openmp on ios, and this test is really slow
  # This is only used for xgboost training and it will be removed once the
  # xgboost interface is updated.
  find_package(OpenMP)
  if(OPENMP_FOUND)
    # Set the globally for now (no clean cmake way to propagate OpenMP_EXE_LINKER_FLAGS
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  endif()
endif()

##############
## Project ###
##############

add_subdirectory(src)