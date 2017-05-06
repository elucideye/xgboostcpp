#!/bin/bash

TOOLCHAIN=android-ndk-r10e-api-19-armeabi-v7a-neon-hid-sections

ARGS=(
    --verbose
    --config Release
    --fwd ANDROID=TRUE     
    HUNTER_CONFIGURATION_TYPES=Release
    XGBOOSTER_SERIALIZE_WITH_BOOST=OFF
    XGBOOSTER_SERIALIZE_WITH_CEREAL=ON
    XGBOOSTER_ADD_TO_STRING=ON
)

export HUNTER_ROOT=${HOME}/devel/ruslo/hunter
build.py --toolchain ${TOOLCHAIN}  ${ARGS[@]} --clear
