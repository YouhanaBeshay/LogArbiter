# Phase Three: Formatter & Threshold Logic

## Goals
1. Create a zero-cost-abstraction message formatter module and integrate it with the application.

## Topic
### C++
* `enum class`.
* Templates.
* Type Safe Unions like `std::optional<>`.
* `constexpr`, `if constexpr`, `constexpr` functions. [See this.](https://www.youtube.com/watch?v=CmViCSHXToM)

### Libraries
* `magic_enum`.

### Design Patterns
> Factory

* Do not need to create an object to use it.
* Needs user to provide *basic components*, it outputs a ptr to the created object.
* Factory completely abstracts the process of constructing the object and its dependencies.
* PCB example (home made step by step vs send just gerber files to a literal factory and it arrives assembled).

> Builder

* Need to create an object to use it.
* Supports `chaining`; all of its methods (except for the `build` method) return a reference to the builder object in order to keep stacking all configurations in a single line.
* Builder guides the user through the process of constructing an object and when done, all one has to do is call `build`; which returns a ptr to the constructed object.
* Cooking example (it is easier to "build" a meal when all the instructions are laid out vs just the ingredients) (when cooking a recipe, some parts needs cooking as well so they need instructions as well).

## Deliverables

### Main
* Three `enum class`es with the following values: 
    * `LogSinkType_enum` &rarr; `CONSOLE`, `FILE`, `SOCKET`.
    * `SeverityLvl_enum` &rarr; `CRITICAL`, `WARNING`, `INFO`.
    * `TelemetrySrc_enum`&rarr; `CPU`, `GPU`, `RAM`.
* `LogFormatter` template class that:
    - can be specialized into a `Policy` (template parameter) that is implemented as the example below. Create policies for CPU, GPU, and RAM in the likeness of the given example.
        ```cpp
        struct CpuPolicy 
        {
            static constexpr TelemetrySrc context = TelemetrySrc::CPU;
            static constexpr std::string_view unit = "%";
            static constexpr float WARNING = 75.0f;
            static constexpr float CRITICAL = 90.0f;

            static constexpr SeverityLvl inferSeverity(float val) noexcept {
                return (val > CRITICAL) ? SeverityLvl::CRITICAL
                    : (val > WARNING)  ? SeverityLvl::WARNING
                    :                    SeverityLvl::INFO;
            }
        };
        ```
    - contains the following methods:
        * `formatDataToLogMsg(raw : const std::string& ) : std::optional<LogMessage>` &rarr; this function takes a value received from a telemetry source (float formatted as a string) and *infers the severity*, *fills in the timestamp*, and *constructs a full log message*.
        * `msgDescription(val : float) : std::string` &rarr; generates a message description/payload describing the received reading.
        * `currentTimeStamp() : std::string` &rarr; generates the timestamp at message construction.
        * Note: `inferSeverity` is a static method **expected to be present** in all template arguments passed during instantiation.

## DP Bonus
* Implement a factory for `LogSink` (based on a sink type, return a constructed sink). 
* Implement a builder for `LogManager` (step-by-step construction of `LogManager`).

## Bonus
* Implement a template ring buffer that replaces the vector used to store messages in `LogManager`. The ring buffer should be as follows:
    - supports move semantics.
    - does not support copy semantics.
    - wraps a vector of `std::optional<T>` where `T` is the template parameter.
    - has two main methods: `tryPush`, `tryPop`.
