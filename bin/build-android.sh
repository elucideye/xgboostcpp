#!/bin/bash

TOOLCHAIN=android-ndk-r10e-api-19-armeabi-v7a-neon-hid-sections-lto

CONFIG=MinSizeRel

INSTALL="--strip"

ARGS=(
    --verbose
    --config ${CONFIG}
    --fwd
    ANDROID=TRUE
    HUNTER_CONFIGURATION_TYPES=${CONFIG}
    XGBOOSTER_SERIALIZE_WITH_CEREAL=ON
    XGBOOSTER_ADD_TO_STRING=ON
    XGBOOSTER_DO_LEAN=${XGBOOSTER_DO_LEAN}
)

polly.py --toolchain ${TOOLCHAIN} ${ARGS[@]} ${INSTALL} --clear
