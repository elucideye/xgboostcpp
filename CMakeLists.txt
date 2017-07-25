# Copyright (c) 2015-2017, David Hirvonen, Ruslan Baratov
# All rights reserved.

# WIP C++ interface + serialization for xgboost library
# NOTE: This is using the xgboost fork in the hunter
# package manager which has some non-standard extensions
# for cross platform and size reduced serialization.
# It also uses OpenMP, whereas the lastest versions
# uses a C++11 thread interface.

# TODO: Add a more recent xgboost to hunter

cmake_minimum_required(VERSION 3.3)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.19.40.tar.gz"
  SHA1 "dd4850329b2e5ed91684f964f8bbc29328898d56"  
  FILEPATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Hunter/config.cmake"    
  )

set(XGBOOSTCPP_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

project(xgboostcpp VERSION 0.1.0) # WIP!

if(CMAKE_INTERPROCEDURAL_OPTIMIZATION)
  set(CMAKE_POLICY_DEFAULT_CMP0069 NEW) # for 3rd parties added by add_subdirectory
  cmake_policy(SET CMP0069 NEW)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# make sure these are before first hunter_add_package() call
option(XGBOOSTER_BUILD_TESTS "Build tests" ON)
option(XGBOOSTER_BUILD_APPS "Build applications" ON)
option(XGBOOSTER_SERIALIZE_WITH_CEREAL "Serialize w/ cereal" ON) # hunter update
option(XGBOOSTER_ADD_TO_STRING "Add local to_string definitions" OFF) # for android
option(XGBOOSTER_DO_LEAN "Build for prediction only (size reduction)" OFF) 

###################
## Dependencies ###
###################

# cereal
if(XGBOOSTER_SERIALIZE_WITH_CEREAL)
  hunter_add_package(cereal)
  find_package(cereal CONFIG REQUIRED)
endif()

# GTest
if(XGBOOSTER_BUILD_TESTS)
  enable_testing()
  hunter_add_package(GTest)
  find_package(GTest CONFIG REQUIRED)
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
