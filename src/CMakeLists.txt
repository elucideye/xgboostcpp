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


