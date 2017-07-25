#!/bin/bash

TOOLCHAIN=xcode

CONFIG=MinSizeRel

ARGS=(
    --verbose
    --config ${CONFIG}
    --fwd HUNTER_CONFIGURATION_TYPES=${CONFIG}
    XGBOOSTER_SERIALIZE_WITH_CEREAL=ON
    --jobs 8
)

build.py --toolchain ${TOOLCHAIN} ${ARGS[@]} --reconfig --install --open --test
