## Customize linker flags
include(CheckCCompilerFlag)
if(NOT MSVC)
  check_c_compiler_flag("-Wl,-dead_strip" FLAG_dead_strip)
  if(NOT FLAG_dead_strip)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections,--icf=safe")
  else()
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-dead_strip")
    set(CMAKE_EXE_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-dead_strip")
  endif()
endif()

###############
### xgboost ###
###############

hunter_add_package(xgboost)
find_package(xgboost CONFIG REQUIRED)

add_subdirectory(lib)
add_subdirectory(test)  


