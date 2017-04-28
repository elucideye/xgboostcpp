###############
### cxxopts ###
###############

# std::regex >= gcc 4.8
hunter_add_package(cxxopts)
find_package(cxxopts CONFIG REQUIRED)

######################
### test-xgbooster ###
######################

set(test_app test-xgbooster)

add_executable(${test_app} test-xgbooster.cpp)
target_link_libraries(${test_app} xgboostcpp cxxopts::cxxopts) 
set_property(TARGET ${test_app} PROPERTY FOLDER "app/console")
install(TARGETS ${test_app} DESTINATION bin)
