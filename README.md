# libgoecharger
A C++ library to access settings of a go-eCharger wallbox.

It provides methods to query all defined properties/settings from a go-eCharger wallbox via the local network http api. Methods to modify settings are partly implemented; the rest will follow soon.

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
        }
    
It relies on the very small footprint json parser written by James McLaughlin: https://github.com/udp/json-parser.
