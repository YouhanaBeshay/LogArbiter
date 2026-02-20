# GpuDataUsage vSomeIP Server 

- This is a seperate App that uses CommonAPI to create a vSomeIP Server that provides the (Randomly Generated) Gpu Usage Data.

### Build on PC
- `cmake -S . -B build_pi --toolchain toolchain/toolchain-rpi3.cmake`
- `cmake --build build_pi/`

## Deploy to RPi

1. **rsync** the server and config to the Pi
2. Create `lib/` directory inside the build folder
3. Copy cross-compiled libraries (vsomeip , commonapi , commonapi-someip , boost) `.so` files into `lib/`

> This works because CMake uses `set(CMAKE_BUILD_RPATH "$ORIGIN/lib")`
- <small>(This is instead of using LD_LIBRARY_PATH)</small>


### Run on PI

- `./runServer.sh`
- <small>(to avoid writing the VSOMEIP_CONFIGURATION environment variable and the multicast route every time)</small>
