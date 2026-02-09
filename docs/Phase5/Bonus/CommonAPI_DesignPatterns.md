# Design Patterns in CommonAPI SOME/IP

- In this document I will describe the core design patterns (that I noticed) used in the CommonAPI SOME/IP C++ library, demonstrated through the GpuUsageData interface implementation. 

## Creational Design Patterns:
- **Factory Pattern:**
  - The Factory pattern is used to create proxy and stub instances without exposing the instantiation logic(like if its SOME/IP or DBUS or method/instance IDs)  to users .
    -  Where exactly used?
       -  ***GpuUsageDataSomeIPProxy*** - (SOME/IP specific proxy implementation) 
       -  ***GpuUsageDataSomeIPStubAdapter*** - (Binds stub to SOME/IP transport & registers methods and events with specific IDs)

## Behavioral Design Patterns:
- **Proxy Pattern:**
  - The Proxy pattern provides a placeholder for a remote service, making remote method "invoke" appear as local function calls.
  - So we have Access to the remote service's methods inside our Client  but the handler is on the server side.
       -  Where exactly used?
          -  ***GpuUsageDataSomeIPProxy*** (obviously)
  <br></br>
- ***Adaptor Pattern:***
  - The Adapter converts the generic CommonAPI stub interface into a SOME/IP specific implementation (if we use DBUS it will be DBUS specific), allowing incompatible interfaces to work together.
  - Where exactly used?
     -  ***GpuUsageDataSomeIPStubAdapter*** 