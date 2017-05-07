set(test_name xgboostcpp_regression_test)
set(test_app test-xgboostcpp-regression)

enable_testing()

set(test_srcs
  test-xgboostcpp-regression.cpp
  boston_train.h
  boston_train.cpp
  boston_test.h
  boston_test.cpp  
  )

add_executable(${test_app} ${test_srcs})
target_link_libraries(${test_app} PUBLIC xgboostcpp GTest::gtest)

set_property(TARGET ${test_app} PROPERTY FOLDER "app/tests")

add_test(
  NAME "${test_name}"
  COMMAND "${test_app}"
  )
