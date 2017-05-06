#!/bin/bash

ARGS=(
    --verbose
    --config Release    
    --fwd HUNTER_CONFIGURATION_TYPES=Release
    XGBOOSTER_SERIALIZE_WITH_BOOST=OFF
    XGBOOSTER_SERIALIZE_WITH_CEREAL=ON
    --jobs 8
)

export HUNTER_ROOT=${HOME}/devel/ruslo/hunter
build.py --toolchain xcode ${ARGS[@]} --reconfig --install --open --test

