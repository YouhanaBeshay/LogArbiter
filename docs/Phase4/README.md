# Phase Four: Asynchronous Logging

## Goals
1. Refactor the logging logic to utilize threads for better performance.

## Topics
### C++
* `Callables` &rarr; functors vs lambdas vs `std::function` + `std::invoke`.
* `Threading` &rarr; `std::threads`.
* `Concurrency` &rarr; `std::mutex`, `std::lock_guard`, `std::condition_variable`.

### Libraries
* None.

### Design Patterns
> Thread Pool (Queues vs Ring Buffers)

## Deliverables

### Main 
* Implement a template ring buffer class called `RingBuffer` that replaces the vector used to store messages in `LogManager`. The ring buffer should be as follows:
    - supports move semantics.
    - does not support copy semantics.
    - wraps a vector of `std::optional<T>` where `T` is the template parameter.
    - has two main methods: `tryPush`, `tryPop`.
    - Note: This was bonus last phase but now it is not optional. `3:D`

* Refactor `LogManager` to utilize `RingBuffer` specialized in `LogMessage` (`RingBuffer<LogMessage>`). 
* Refactor the application to utilize `std::thread`s to support continuous & concurrent data logging.

## DP Mega Bonus (includes normal + DB bonuses)
* `ThreadPool` class that is used for multiple tasks (like logging a message from a certain source into all of its registered sinks). A `ThreadPool` class typically contains the following elements:
    * A vector of threads.
    * A queue for tasks; in this case, a task is an `std::function`.
    * A method to push a *callable* into the tasks queue.
    * Ctor &rarr; initializes threads with the same logic and makes sure that tasks are invoked at the end.
    * Dtor &rarr; cleans up everything and joins working threads into main thread for safe exit.
