# GpuDataUsage vSomeIP Server 

- This is a seperate App that uses CommonAPI to create a vSomeIP Server that provides the (Randomly Generated) Gpu Usage Data.

### Build
- `cmake -S . -B build`
- `cmake --build build`

### Run

1.  **Note**: To run the server you need the provide some config for the CommonAPI (the serviceID an instanceID in hex format).
    -  2 Options: 
       1. `cp commonapi-someip.ini build/` 
       2. `export COMMONAPI_SOMEIP_CONFIG=/path/to/commonapi-someip.ini`


2. `./build/vSomeipServer_GpuUsage`
