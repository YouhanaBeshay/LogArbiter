# LogArbiter
 A configurable, multi-source telemetry logging system built with modern C++.

 - This project was developed phase by phase, for each phase deliverables see the [`docs`](./docs) directory. 

---

##  Table of Contents

- [LogArbiter](#logarbiter)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Architecture](#architecture)
    - [Component Diagrams](#component-diagrams)
    - [Class Diagrams](#class-diagrams)
    - [Design Patterns used](#design-patterns-used)
  - [Features](#features)
  - [Dependencies](#dependencies)
    - [For SomeIP Integration](#for-someip-integration)
  - [Libraries Used](#libraries-used)
  - [Build](#build)
  - [Running](#running)
  - [Configuration](#configuration)
    - [Configuration Options](#configuration-options)

---
## Overview

LogArbiter is a flexible logging framework designed to:

- **Collect** telemetry data from multiple sources (files, Unix sockets, SomeIP services)
- **Format** raw data using policy-based formatters (CPU, RAM, GPU metrics)
- **Output** formatted logs to multiple sinks (console, files)
- **Configure** behavior at runtime via JSON configuration
  
The system is built with thread safety in mind, using a producer-consumer pattern with a ring buffer and thread pool for efficient, concurrent log processing.

---
## Architecture
### Component Diagrams

- Simple Component Diagram:
![Component Diagram](diagrams/Component_Diagram.png)
- Detailed Component Diagram:
[Detailed Component Diagram](diagrams/Detailed_Component_Diagram.png)
### Class Diagrams
- Main class diagram:
![Main Class Diagram](diagrams/Main_ClassDiagram.png)
>  For more detailed diagrams, see the [`/diagrams`](./diagrams) folder.
### Design Patterns used 

   - **Façade** - `LogArbiter` - The LogArbiter class simplifies the entire system
   - **Factory** - `LogSinkFactory` creates sink instances
   - **Builder** - `LogManagerBuilder` for flexible LogManager construction
   - **Strategy** -  `LogManager` takes `ILogSink` pointes  as input not the sink directly
   - **Singleton & Adapter** - `SomeIPTelemetrySourceImpl` for CommonAPI client



---
## Features

| Feature | Description |
|---------|-------------|
| **Multi-Source Input** | File, Unix Socket, SomeIP (CommonAPI) |
| **Policy-Based Formatting** | Extensible formatter with CPU/RAM/GPU policies |
| **Multiple Sinks** | Console and multiple file outputs |
| **Concurrent Writing** | Thread pool for parallel sink operations |
| **Runtime Configuration** | JSON-based configuration |
| **Graceful Shutdown** | Signal handling (Ctrl+C) |

---
## Dependencies

### For SomeIP Integration

- **vsomeip3** 
- **CommonAPI** 
- **CommonAPI-SomeIP** 
---
## Libraries Used
- single-header libraries: found in [`/external_lib`](./external_lib/)
  
| Library | Purpose | 
|---------|---------|
| **nlohmann/json** | JSON parsing for configuration |
|**magic_enum** | 	Enum reflection and string conversion|

---
## Build
1. Build the Main Application

    ```bash # Clone the repository
    git clone https://github.com/YouhanaBeshay/LogArbiter.git
    cd LogArbiter

    # Create build directory
    mkdir build && cd build

    # Configure with CMake
    cmake ..

    # Build
    cmake --build .
    ```
2. Build the vSomeIP Server

    ```bash
    # Navigate to vSomeIP server directory
    cd CommonApi/vSomeipServer

    # Create build directory
    mkdir build && cd build

    # Configure and build
    cmake ..
    make 
    ``` 
---
## Running
- **Step 1:** Start Telemetry Sources:
  - Open ***separate terminals*** for each source:
    - Terminal 1 - File Telemetry Source
        ```bash
        cd provided
        ./shell_log_src.sh
        ```
    - Terminal 2 - Socket Telemetry Source
        ```bash
        cd provided
        ./shell_log_src_socket.sh
        ```
    - Terminal 3 - SomeIP Telemetry Source
        ```bash
        cd CommonApi/vSomeipServer/build
        ./vSomeipServer_GpuUsage
        ```

- **Step 2:** Run LogArbiter using the provided `runLogArbiter.sh` script:
  -  This script sets up the multicast route and environment variable for the LogArbiter binary 
  - LogArbiter opens a multicast route to be able to connect to the vSomeIP server on the Pi the multicast and interface are set/configured in [`runLogArbiter.sh`](./runLogArbiter.sh)
    - Terminal 4 - Main Application:
        ```bash 
        ./runLogArbiter.sh
        ```
---
## Configuration
. 
- LogArbiter uses a JSON configuration file located at [`App/config.json`](./App/config.json) for runtime configuration.

### Configuration Options

| Section | Option | Description |
|---------|--------|-------------|
| `log_manager.buffer_capacity` | Ring buffer size | Number of messages to buffer |
| `log_manager.thread_pool_size` | Worker threads | Threads for parallel sink writing |
| `log_manager.sink_flush_rate_ms` | Flush interval | How often to write buffered messages |
| `sinks.console.enabled` | Enable console | Output to stdout |
| `sinks.files[].enabled` | Enable file sink | Toggle individual file output |
| `sinks.files[].path` | File path | Output file location |
| `sources.*.enabled` | Enable file source | Toggle file telemetry source |
| `sources.*.path` | File path | Path to telemetry source |
| `sources.*.parse_rate_ms` | Read interval | How often to read from source (ms) |
| `sources.*.policy` | Formatter policy | `cpu`, `ram`, or `gpu` |
