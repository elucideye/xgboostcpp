###############
### xgboost ###
###############

option(DRISHTI_USE_LOCAL_XGBOOST "Use local xgboost" ON)
if(DRISHTI_USE_LOCAL_XGBOOST)
  option(SKIP_INSTALL "Avoid installation of submodule libraries" OFF)

  option(SKIP_INSTALL "Skip the package install" OFF)
  option(XGBOOST_USE_BOOST "Use boost serialization" ${XGBOOSTER_SERIALIZE_WITH_BOOST})
  option(XGBOOST_USE_CEREAL "Use cereal serialization" ${XGBOOSTER_SERIALIZE_WITH_CEREAL})
  option(XGBOOST_ADD_TO_STRING "Add standard library std::to_string()" ${XGBOOSTER_ADD_TO_STRING})
  option(XGBOOST_USE_HALF "Support half precision floating point storage" ON)
  option(XGBOOST_DO_LEAN "Build lean library for evaluation only" OFF)
  
  add_subdirectory(3rdparty/xgboost)
  unset(SKIP_INSTALL CACHE)
  add_library(xgboost::xgboost ALIAS xgboost)
  set_property(TARGET xgboost PROPERTY FOLDER "libs/3rdparty")
  target_include_directories(xgboost PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/3rdparty>")
  
else()
  hunter_add_package(xgboost)
  find_package(xgboost CONFIG REQUIRED)
endif()

message("XGBOOST_USE_BOOST = ${XGBOOST_USE_BOOST}")
message("XGBOOST_USE_CEREAL = ${XGBOOST_USE_CEREAL}")
message("XGBOOSTER_SERIALIZE_WITH_BOOST = ${XGBOOSTER_SERIALIZE_WITH_BOOST}")
message("XGBOOSTER_SERIALIZE_WITH_CEREAL = ${XGBOOSTER_SERIALIZE_WITH_CEREAL}")

add_subdirectory(lib)
add_subdirectory(test)
