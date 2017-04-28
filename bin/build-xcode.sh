#!/bin/bash

ARGS=(
    --verbose
    --fwd HUNTER_CONFIGURATION_TYPES=Release
    --jobs 8
    --reconfig
    --install
)

build.py --toolchain xcode ${ARGS[@]} --open --nobuild
