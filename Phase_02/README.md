# Phase Two: Data Sources & Smart Resource Management

## Goals
1. Create actual data sources that continuously receive data from multiple source types.

## Topics
### C++
* References.
* Move vs Copy Semantics.
* Rule of 3/5/0.
* Special Member Functions.

### Libraries
* None.

### Design Idioms
* Resource Acquisition is Initialization (RAII).

### Extra
* Unix Domain Socket (UDS): a powerful tool used for interprocess communication (IPC) that is used extensively in Linux-based systems.

## Deliverables
* `ITelemetrySource` interface with three methods:
    - `openSource() : bool` &rarr; responsible for openning source/establishing connection.
    - `readSource(out : string&) : bool` &rarr; reads the source and stores the latest reading in `out`.
* At least one of these RAII-compliant classes: `SafeFile` & `SafeSocket`.
    - `SafeFile` wraps POSIX file operations into a safe environment that guarantees reliable source management. Only **move semantics should be implemented and copy semantics should be removed**. Hint: look up `<unistd.h>`.
    - `SafeSocket` wraps standard Unix Domain Socket operations into a safe environment that guarantees reliable source management. Only **move semantics should be implemented and copy semantics should be removed**. Hint: look up `<unistd.h>`.
* At least one of the following Rule-of-Zero-compliant classes (only `std` components and RAII-compliant ones should be used):
    - `FileTelemetrySourceImpl` class that implements `ITelemetrySource` and utilizes `SafeFile` class. The source should a shell script that simply writes float numbers into a file (will be provided).
    - `SocketTelemetrySourceImpl` class that implements `ITelemetrySource` and utilizes `SafeSocket` class.
* Refactor `LogMessage` to default all special member functions.

## Bonus
> In linux, there exists two files under `/proc` that record cpu load and memory usage respectively: `/proc/stat` and `/proc/meminfo` respectively. The requirements are as follows:

* Make the data sources parse the mentioned files and either pass them through writing in a file/through a socket accessed by the logging system. That guarantees that the data passed is not random and are actually meaningful.
