###############
### cxxopts ### :: std::regex >= gcc 4.8
###############

hunter_add_package(cxxopts)
find_package(cxxopts CONFIG REQUIRED)

################
### boost qi ###
################

hunter_add_package(Boost COMPONENTS system)
find_package(Boost CONFIG REQUIRED system)

######################
### test-xgbooster ###
######################

set(test_app test-xgbooster)

add_executable(${test_app} test-xgbooster.cpp)
target_link_libraries(${test_app} xgboostcpp cxxopts::cxxopts Boost::system)

if(XGBOOSTER_DO_LEAN)
  target_compile_definitions(${test_app} PUBLIC XGBOOSTER_DO_LEAN=1)
endif()

# Enable this option for testing lto behavior
# target_compile_definitions(${test_app} PUBLIC XGBOOSTER_DO_LEAN=1)

set_property(TARGET ${test_app} PROPERTY FOLDER "app/console")
install(TARGETS ${test_app} DESTINATION bin)
