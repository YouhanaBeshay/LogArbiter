# Phase Six: System Wrap Up (Design & Configuration)

## Goals
1. Wrap up the entire system into a configurable, presentable package.

## Topics

### Libraries
* `json` ([JSON For Modern C++](https://github.com/nlohmann/json) --- **single** header library) ( ͡° ͜ʖ ͡°)

### Design Patterns
> Façade

## Deliverables

### Main (DP Bonus Included)
* Create a `YourApplicationName` class that abstracts all the setup, initializations, and functionality of the application into two simple methods: `YourApplicationName()` (class constructor) and `start()` method. 
* Create a configuration method/strategy for the application to configure the following parameters (including, but not limited to):
    * Enable/disable telemetry sources.
    * Enable/Disable log sinks for each telemetry source.
    * Set the rate of parsing data from telemetry sources.
    * Set the rate of logging data to log sinks.
* **Note:** The configuration method can be one of the three options (feel free to do one of them or a 4th one altogether):
    * Using Macros `#define` (Pre-build)
        * Ex: `#define SOCKET_TEL_SRC_RATE 200 // rate is in ms`
    * Using Structs `struct` (Post-build)
        * Ex:
            ```cpp
            struct CPUConfig 
            {
                TelemetrySrc VSOMEIP;
                TelemetrySrcParseRate 200;
                .
                .
                .
            };
            ```
    * Using JSON files (Runtime - Bonus)

## Library Bonus
* Configure the application using JSON files. Utilize the library [JSON For Modern C++](https://github.com/nlohmann/json) to parse json-formatted configuration files to configure the application during runtime.
