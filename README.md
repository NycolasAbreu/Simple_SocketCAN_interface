# Simple sokcet CAN interface

A C++ study project that provides a flexible and extensible framework for working with CAN (Controller Area Network) communication in automotive and industrial applications. It supports both raw CAN and ISO-TP (ISO 15765-2) protocols.

## Features

- Support for raw CAN and ISO-TP protocols
- Easy-to-use interface for sending and receiving CAN messages
- Configurable CAN filters
- Timeout support for read operations

## Components

1. `CanDevice`: Abstract base class for CAN devices
2. `RawCanDevice`: Implementation for raw CAN protocol
3. `IsoTpCanDevice`: Implementation for ISO-TP protocol
4. `CanDeviceFactory`: Factory for CanDevice creation
5. `CanMessage`: Class representing CAN messages

## Usage

Here's a basic example of how to use the library:

```cpp
#include "RawCanDevice.h"
#include "CanMessage.h"

int main() {
    RawCanDevice can("can0");
    can.Open();

    CanMessage::Data data{0x01, 0x02, 0x03, 0x04};
    CanMessage message(0x123, data);
    can.Send(message);

    CanMessage received_message;
    if (can.Read(received_message, 1000)) {
        // Process received message
    }

    can.Close();
    return 0;
}
```
## TO-DO

 - [X] Simple interface 
 - [X] Raw Protocol
 - [X] Basic Iso-TP Protocol
 - [ ] Full Iso-TP Protocol
 - [ ] Error handling
 - [ ] Exemple of usage
 - [ ] Unit Tests
 - [ ] Integration Tests

 ## Usefull commands

 - Opening a virtual CAN bus

``` bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

 - Dump Iso-Tp data
``` bash
            #Txid  #Rxid
isotpdump -s 111 -d 321 vcan0
```
 - Send Iso-Tp data
``` bash
                                     #Rxid  #Txid
echo 11 12 13 14 15 16 | isotpsend -s 321 -d 111 vcan0
```