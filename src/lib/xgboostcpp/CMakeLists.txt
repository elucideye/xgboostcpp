set(xgboostcpp_SRCS
  Booster.h
  XGBooster.cpp
  XGBooster.h
  XGBoosterIOArchiveBoost.cpp
  XGBoosterIOArchiveCereal.cpp
  XGBoosterImpl.h
  )

add_library(xgboostcpp "${xgboostcpp_SRCS}")
target_link_libraries(xgboostcpp xgboost::xgboost)

if(XGBOOSTER_SERIALIZE_WITH_BOOST)
  target_link_libraries(xgboostcpp boost-pba::boost-pba)
endif()

if(XGBOOSTER_SERIALIZE_WITH_CEREAL)
  target_link_libraries(xgboostcpp cereal::cereal)
endif()

# USE_XGBOOST_WITH_BOOST
if(XGBOOSTER_SERIALIZE_WITH_BOOST OR XGBOOSTER_SERIALIZE_WITH_CEREAL)
  set(use_xgboost_with_boost 1)
else()
  set(use_xgboost_with_boost 0)
endif()
target_compile_definitions(xgboostcpp PUBLIC USE_XGBOOST_WITH_BOOST=${use_xgboost_with_boost})

set_property(TARGET xgboostcpp PROPERTY FOLDER "libs")

target_include_directories(xgboostcpp
  PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../>"
  )

### Generate the export header
include(GenerateExportHeader)
generate_export_header(xgboostcpp EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/lib/xgboostcpp_export.h")

string(COMPARE EQUAL "${CMAKE_OSX_SYSROOT}" "iphoneos" is_ios)
if(NOT is_ios)
  set_target_properties(xgboostcpp PROPERTIES
    VERSION "${hunter_sandbox_VERSION}"
    SOVERSION "${hunter_sandbox_VERSION_MAJOR}"
    )
endif()

###################
##### install #####
###################

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(include_install_dir "include")
set(config_install_dir "share/${CMAKE_PROJECT_NAME}")
set(version_config "${generated_dir}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${CMAKE_PROJECT_NAME}Config.cmake")
set(targets_export_name "${CMAKE_PROJECT_NAME}Targets")
set(namespace "${CMAKE_PROJECT_NAME}::")

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${version_config}"
  VERSION "${hunter_sandbox_VERSION}"
  COMPATIBILITY SameMajorVersion)

# Use variables:
#   * targets_export_name
#   * PROJECT_NAME
configure_package_config_file(
    "${XGBOOSTCPP_ROOT_DIR}/cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

install(
  TARGETS xgboostcpp
  EXPORT "${targets_export_name}"
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib"
  RUNTIME DESTINATION "bin"
  INCLUDES DESTINATION "${include_install_dir}"
  )

install(
  FILES
  XGBooster.h
  "${CMAKE_BINARY_DIR}/lib/xgboostcpp_export.h"
  DESTINATION "${include_install_dir}/xgboostcpp/"
  )
