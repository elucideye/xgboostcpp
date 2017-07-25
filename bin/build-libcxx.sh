#!/bin/bash

CONFIG=MinSizeRel
TOOLCHAIN=osx-10-12-lto
INSTALL=--install

ARGS=(
    --verbose
    --config ${CONFIG}
    --fwd
    HUNTER_CONFIGURATION_TYPES=${CONFIG}
    XGBOOSTER_SERIALIZE_WITH_CEREAL=ON
    XGBOOSTER_ADD_TO_STRING=ON
)

polly.py --toolchain ${TOOLCHAIN} ${ARGS[@]} --jobs 8 ${INSTALL} --reconfig #  --test




