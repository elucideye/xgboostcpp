#!/bin/bash

find src -name "*.cpp" -or -name "*.h" -or -name "*.hpp" -or -name CMakeLists.txt | xargs etags --append
