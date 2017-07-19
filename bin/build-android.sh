#!/bin/bash

CONFIG=MinSizeRel

# Test 1 :
# 640K	_install/android-ndk-r10e-api-19-armeabi-v7a-neon-hid-sections/bin/test-xgbooster
# TOOLCHAIN=android-ndk-r10e-api-19-armeabi-v7a-neon-hid-sections
# XGBOOSTER_DO_LEAN=ON

# Test 2 :
# 724K	_install/android-ndk-r10e-api-19-armeabi-v7a-neon-hid-sections-lto/bin/test-xgbooster
TOOLCHAIN=android-ndk-r10e-api-19-armeabi-v7a-neon-hid-sections-lto
XGBOOSTER_DO_LEAN=OFF

# Test 3 :
# 640K	_install/android-ndk-r10e-api-19-armeabi-v7a-neon-hid-sections-lto/bin/test-xgbooster
# TOOLCHAIN=android-ndk-r10e-api-19-armeabi-v7a-neon-hid-sections-lto
# XGBOOSTER_DO_LEAN=ON

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
    XGBOOSTER_DO_LEAN=${XGBOOSTER_DO_LEAN}
)

polly.py --toolchain ${TOOLCHAIN} ${ARGS[@]} ${INSTALL} --clear
