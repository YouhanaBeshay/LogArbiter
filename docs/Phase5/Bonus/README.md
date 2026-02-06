## How to generate stub + proxy interfaces?
1. Create a directory to store the generated stuff (name it `app_trial` for example --- it is assumed that the generator executables' folders are located next to the other directories). The directory tree should resemble something like this:
    ```
    ├── commonapi_core_generator
    │   └── commonapi-core-generator-linux-x86_64 (core generator executable)
    ├── commonapi_someip_generator
    │   └──  commonapi-someip-generator-linux-x86_64 (someip generator executable)
    ├── fdepl
    ├── fidl
    │   └── omnimetron
    │       └── gpu
    ├── app_trial
        ├── proxy
        ├── src-gen
        │   └── v1
        │       └── app
        │           └── gpu
        └── stub
    ```
1. `cd` into `app_trial`
1. Using `commonapi_core_generator`:
    ```shell
    ../commonapi_core_generator/commonapi-core-generator-linux-x86_64 -sk ../fidl/omnimetron/gpu/gpu.fidl
    ```
1. Using `commonapi_someip_generator`:
    ```shell
    ../commonapi_someip_generator/commonapi-someip-generator-linux-x86_64 ../fdepl/gpu.fdepl
    ```
1. The two interfaces of interest are `StubDefault.hpp` and `Proxy.hpp`.
