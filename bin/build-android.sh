#!/bin/bash

CONFIG=MinSizeRel
TOOLCHAIN=android-ndk-r10e-api-19-armeabi-v7a-neon-hid-sections-lto
INSTALL=--strip

ARGS=(
    --verbose
    --config ${CONFIG}
    --fwd
    ANDROID=TRUE
    HUNTER_CONFIGURATION_TYPES=${CONFIG}
    XGBOOSTER_SERIALIZE_WITH_BOOST=OFF
    XGBOOSTER_SERIALIZE_WITH_CEREAL=ON
    XGBOOSTER_ADD_TO_STRING=ON
)

polly.py --toolchain ${TOOLCHAIN} ${ARGS[@]} ${INSTALL} --reconfig # --test
