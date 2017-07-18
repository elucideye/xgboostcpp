# xgboostcpp
Simple C++ interface for xgboost with cross platform and size reduce serialization

This is a work in progress, and is posted initially as a reference for using the xgboost C++ API.

[![License (3-Clause BSD)](https://img.shields.io/badge/license-BSD%203--Clause-brightgreen.svg?style=flat-square)](http://opensource.org/licenses/BSD-3-Clause)
[![HUNTER](https://img.shields.io/badge/hunter-v0.18.44-blue.svg)](http://github.com/ruslo/hunter)


| Linux/OSX/Android/iOS                           | Windows                                             |
|-------------------------------------------------|-----------------------------------------------------|
| [![Build Status][travis_status]][travis_builds] | [![Build Status][appveyor_status]][appveyor_builds] |


[travis_status]: https://travis-ci.org/elucideye/xgboostcpp.svg?branch=master
[travis_builds]: https://travis-ci.com/elucideye/xgboostcpp


[appveyor_status]: https://ci.appveyor.com/api/projects/status/vh2hu8q7s17p00et?svg=true
[appveyor_builds]: https://ci.appveyor.com/api/projects/elucideye/xgboostcpp


HOWTO
=====

# Install latest Polly toolchains and scripts for cmake (not needed but very easy)
```
wget https://github.com/ruslo/polly/archive/master.zip
unzip master.zip
POLLY_ROOT="`pwd`/polly-master"
export PATH="${POLLY_ROOT}/bin:${PATH}"
```

# Build (see poll.py for list of available toolchains)
```
polly.py --toolchain libcxx --install --reconfig --verbose
```

The standard build should use hunter xgboost.
Initial builds may still require the xgboost submodule, in which case you
may need to run:

```
git submodule update --init --recursive --quiet
```
