# xgbooster
Simple C++ interface for xgboost with cross platform and size reduce serialization


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
