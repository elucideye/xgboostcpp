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

# Experimental reductions
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fomit-frame-pointer -funswitch-loops -finline-limit=300 -finline-limit=64")

###############
### xgboost ###
###############

hunter_add_package(xgboost)
find_package(xgboost CONFIG REQUIRED)

message("XGBOOST_USE_BOOST = ${XGBOOST_USE_BOOST}")
message("XGBOOST_USE_CEREAL = ${XGBOOST_USE_CEREAL}")
message("XGBOOSTER_SERIALIZE_WITH_BOOST = ${XGBOOSTER_SERIALIZE_WITH_BOOST}")
message("XGBOOSTER_SERIALIZE_WITH_CEREAL = ${XGBOOSTER_SERIALIZE_WITH_CEREAL}")

add_subdirectory(lib)
add_subdirectory(test)  


