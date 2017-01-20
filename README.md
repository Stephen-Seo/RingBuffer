
# Dependencies

The compiler must support C++14.

To build, cmake is required.

# Compiling

```
git clone RingBuffer
cd RingBuffer
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=./install_here -DCMAKE_BUILD_TYPE=Release ..
make
make install
```

