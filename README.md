
# Dependencies

The compiler must support C++14.

To build, cmake is required.

UnitTests has GTest as a dependency. It will not build if it is not found.

# Compiling

Note this is a header only library.

Thus it will only build the UnitTest if GTest is found, and install the headers
when `make install` is performed.

```
git clone RingBuffer
cd RingBuffer
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=./install_here ..
make
make install
```

## Compiling with Clang

```
cd build
CC=clang CXX=clang++ cmake -DCMAKE_INSTALL_PREFIX=./install_here ..
make
```

