# Test libav

This is a sample how to use libav to load raw image and convert format and build it using cmake.

## Install

Install libav from https://libav.org/download/ with KVAZAAR.

apt-get install build-essential autoconf libtool cmake git vim wget

## Build

```
mkdir build
cd build
cmake ..
make
```

## Warning
This is a test of API only. Care must be taken to release all the resources.
Valgrind shows leaks for this sample.