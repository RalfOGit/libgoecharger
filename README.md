# libgoecharger
A C++ library to access settings of a go-eCharger wallbox.

It provides methods to query and set all defined properties/settings from a go-eCharger wallbox via the local http network api.

**Warning: The go-eCharger wallbox uses eeprom memory as non-volatile storage. This kind of memory supports only a limited number of write-cycles. You can modify properties/settings like the app is doing, but you must NOT repetitively call any of the set accessor methods a large number of times.**

The software comes as is. No warrantees whatsoever are given and no responsibility is assumed in case of failure or damage being caused to your wallbox.

The API is coded in class GoEChargerAPI. The wallbox provides properties/settings as a block of data values. Therefore the implementation queries the full block of data values and stores it into an in-memory key-value map. Get accessor methods retrieve data from the in-memory map. A refresh method is provided to update the in-memory map if needed. Set accessor methods change individual properties/settings in the wallbox. The http response to a set accessor method contains the full block of data values; i.e. the in-memory key-value map is updated with each call to a set accessor method.

Usage is rather straightforward:

        GoEChargerAPI api("http://192.168.178.19/");

        // send http status request
        if (api.refreshMap() == true) {
            std::string version = api.getVersion();
            std::string date = api.getDateTime();
            uint32_t rbc = api.getRebootCounter();
            uint32_t rbt = api.getRebootTimer();
            uint8_t  car = api.getVehicleState();
            uint8_t  amp = api.getMaximumChargeCurrent();
            uint8_t  amt = api.getMaximumChargeCurrentTemperatureLimited();
            uint8_t  err = api.getError();
            uint8_t  ast = api.getAccessState();
            uint8_t  alw = api.getAllowChargingState();
            ...
            
            uint8_t amx = 3;
            bool success = api.setVolatileMaximumChargeCurrent(amx);
        }
    
It relies on the very small footprint json parser written by James McLaughlin: https://github.com/udp/json-parser.

The source code contains doxygen comments, so that you can generate documentation for the library.

The code has been tested against the following environment:

        OS: CentOS 8(TM), IDE: VSCode (TM)
        OS: Windows 10(TM), IDE: Visual Studio Community Edition 2019 (TM)

Writable and readable properties of the go-eCharger wallbox are documented here:
- https://github.com/goecharger/go-eCharger-API-v1/blob/master/go-eCharger%20API%20v1%20DE.md - partially outdated
- https://www.loxwiki.eu/pages/viewpage.action?pageId=72122962 - description of tmp, tma, amp, txi
