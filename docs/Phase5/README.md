# Phase Five: Telemetry Over Network

## Goals
1. Create a data source that fetches data through vSOME/IP.

## Topics

### Libraries
* `vsomeip`.
* `CommonAPI` (Core + vSomeIP Runtime).

### Design Patterns
> Singleton

> Adaptor

> Others will be observed through use of generated interfaces (Bonus).

## Deliverables

### Main 
* Create a `SomeIPTelemetrySourceImpl` class (client) that communicates with a `vsomeip` service. The client requests to execute a certain method and the response is sent by the service. The response payload should contain a load percentage (0-100). The communication model used is method request/response.

## DP Bonus 
* Implement an Adapter for `SomeIPTelemetrySourceImpl` to match the `ITelemetrySource` interface with the underlying client structure.
* Implement `SomeIPTelemetrySourceImpl` as a Singleton. Since there must be only one instance of it in the entire application and this instance should be alive for the entire lifetime of the application.

## Mega Bonus
* Utilize the given `.fidl` and `.fdepl` files to generate stub and proxy for service and client respectively. Then, use the appropriate interface to create `SomeIPTelemetrySourceImpl`. This way, the communication model used will be event-based.
* From the generated interfaces, identify the used design patterns throughout the files. In the documentation, write a short description, for the usage of each.
  
