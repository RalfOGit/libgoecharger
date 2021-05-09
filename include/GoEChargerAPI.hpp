#ifndef __GOECHARGERAPI_HPP__
#define __GOECHARGERAPI_HPP__

#include <cstdint>
#include <string>
#include <GoEChargerDataMap.hpp>

/**
 * Class implementing an API for the go-eCharger wallbox. 
 * It has been tested against firmware version 40.1. The getter accessors are designed
 * such that a call to refreshMap queries the full status block from the charger, stores
 * it in the internal map and individual get methods query against the internal map.
 */
class GoEChargerAPI {
protected:

    GoEChargerDataMap* map;
    std::string host_url;

    uint8_t     getUint8(const char* const key);
    uint16_t    getUint16(const char* const key);
    uint32_t    getUint32(const char* const key);
    std::string getString(const char* const key);

    bool setUint8(const char* const key, const uint8_t value);
    bool setUint16(const char* const key, const uint16_t value);
    bool setUint32(const char* const key, const uint32_t value);
    bool setString(const char* const key, const std::string& value);

    const char* al_command(const unsigned int index);
    const char* ec_command(const unsigned int index);
    const char* rc_command(const unsigned int index);
    const char* rn_command(const unsigned int index);

    void freeMap(void);

public:

    GoEChargerAPI(const std::string& host_url);
    ~GoEChargerAPI(void);

    //! Refresh the underlying data map by querying the wallbox status.
    bool refreshMap(void);

    // Get accessor methods.
    std::string getVersion(void);
    std::string getDateTime(void);
    uint32_t    getRebootCounter(void);
    uint32_t    getRebootTimer(void);
    uint8_t     getVehicleState(void);
    uint8_t     getMaximumChargeCurrent(void);
    uint8_t     getMaximumChargeCurrentTemperatureLimited(void);
    uint8_t     getError(void);
    uint8_t     getAccessState(void);
    uint8_t     getAllowChargingState(void);
    uint8_t     getStopState(void);
    uint8_t     getCableCoding(void);
    uint8_t     getRelaisState(void);
    uint8_t     getTemperature(void);
    std::array<double, 4> getTemperatureArray(void);
    uint32_t    getEnergy(void);
    uint32_t    getEnergyLimitForCharging(void);
    uint8_t     getInputAdapterState(void);
    uint8_t     getUnlockedByRFIDIndex(void);
    uint32_t    getEnergyTotal(void);
    uint8_t     getWifiState(void);
    uint8_t     getTransmitInterface(void);
    std::array<uint32_t, 16> getSensorArray(void);
    std::string getFirmwareVersion(void);
    std::string getSerialNumber(void);
    std::string getWifiSSID(void);
    std::string getWifiPassword(void);
    uint8_t     isWifiEnabled(void);
    uint8_t     isCloudDisabled(void);
    uint8_t     getTimeZoneOffset(void);
    uint8_t     getDaylightSavingTimeOffset(void);
    uint8_t     getLEDBrightness(void);
    uint8_t     getMinimumHoursForAutomaticCharging(void);
    uint8_t     getLastHourForAutomaticCharging(void);
    uint8_t     getAwattarPriceZone(void);
    uint8_t     getMaximumConfigurableCurrent(void);
    uint8_t     getCurrentSettingForButton(const unsigned i);
    uint32_t    getColorValueForIdleState(void);
    uint32_t    getColorValueForChargingState(void);
    uint32_t    getColorValueForFinishedState(void);
    uint8_t     getLEDEnergySavingState(void);
    uint8_t     getCableUnlockState(void);
    std::string getWifiHotspotPassword(void);
    uint8_t     getAPISettings(void);
    uint8_t     getRemainingTimeUntilTariffTriggeredCharging(void);
    uint8_t     getNorwegianMode(void);
    std::string getSchedulerSettings(void);
    uint8_t     getSchedulerOverrideByButtonClickSetting(void);
    uint32_t    getRFIDCardTotalEnergy(const unsigned i);
    std::string getRFIDCardIdentifier(const unsigned i);
    std::string getRFIDCardName(const unsigned i);
    uint8_t     isLoadManagementEnabled(void);
    uint8_t     getLoadManagementGroupTotalCurrent(void);
    uint8_t     getLoadManagementMinimumCurrent(void);
    uint8_t     getLoadManagementPriority(void);
    std::string getLoadManagementGroupID(void);
    uint8_t     getLoadManagementExpectedNumberOfStations(void);
    uint8_t     getLoadManagementFallbackCurrent(void);
    uint8_t     getLoadManagementCurrent(void);
    uint32_t    getLoadManagementIdleSeconds(void);
    uint8_t     isCustomMQTTEnabled(void);
    std::string getCustomMQTTServerHostname(void);
    uint16_t    getCustomMQTTServerPort(void);
    std::string getCustomMQTTUsername(void);
    std::string getCustomMQTTPassword(void);
    uint8_t     isCustomMQTTConnected(void);

    // Set accessor methods.
    bool setMaximumChargeCurrent(const uint8_t value);
    bool setMaximumChargeCurrentTemperatureLimited(const uint8_t value);
    bool setAccessState(const uint8_t value);
    bool setAllowChargingState(const uint8_t value);
    bool setStopState(const uint8_t value);
    bool setEnergyLimitForCharging(const uint32_t value);
    bool setWifiSSID(const std::string& value);
    bool setWifiPassword(const std::string& value);
    bool setWifiEnabled(const uint8_t value);
    bool setTimeZoneOffset(const uint8_t value);
    bool setDaylightSavingTimeOffset(const uint8_t value);
    bool setLEDBrightness(const uint8_t value);
    bool setMinimumHoursForAutomaticCharging(const uint8_t value);
    bool setLastHourForAutomaticCharging(const uint8_t value);
    bool setMaximumConfigurableCurrent(const uint8_t value);
    bool setCurrentSettingForButton(const unsigned i, const uint8_t value);

    //amp amx ast alw stp dwo wss wke wen tof tds lbr aho afi ama al1 al2 al3 al4 al5
    //cid cch cfi lse ust wak r1x dto nmo rna rnm rne rn4 rn5 rn6 rn7 rn8 rn9 rn1
};



//! Command strings to get the current setting for a given button.
inline const char* GoEChargerAPI::al_command(const unsigned int index) {
    static const std::array<char[4], 5> c = { "al1", "al2", "al3", "al4", "al5" };
    return ((index < c.size()) ? c[index] : "");
}

//! Command strings used to get the total energy charged by a given RFID card 1-10 - card 1 is index 0, card 2 is index 1.
inline const char* GoEChargerAPI::ec_command(const unsigned int index) {
    static const std::array<char[4], 10> c = { "eca", "ecr", "ecd", "ec4", "ec5", "ec6", "ec7", "ec8", "ec9", "ec1" };
    return ((index < c.size()) ? c[index] : "");
}

//! Command strings used to get the RFID card identifier.
inline const char* GoEChargerAPI::rc_command(const unsigned int index) {
    static const std::array<char[4], 10> c = { "rca", "rcr", "rcd", "rc4", "rc5", "rc6", "rc7", "rc8", "rc9", "rc1" };
    return ((index < c.size()) ? c[index] : "");
}

//! Command strings used to get the RFID name.
inline const char* GoEChargerAPI::rn_command(const unsigned int index) {
    static const std::array<char[4], 10> c = { "rna", "rnm", "rne", "rn4", "rn5", "rn6", "rn7", "rn8", "rn9", "rn1" };
    return ((index < c.size()) ? c[index] : "");
}

//! Get json version - returns "B" for normal operation and "C" for end-to-end encrypted operation.
inline std::string GoEChargerAPI::getVersion(void) { return getString("version"); }

//! Get date and time - returns "ddmmyyhhmm"-
inline std::string GoEChargerAPI::getDateTime(void) { return getString("tme"); }

//! Get reboot counter.
inline uint32_t GoEChargerAPI::getRebootCounter(void) { return getUint32("rbc"); }

//! Get reboot timer - returns time since last reboot in milliseconds.
inline uint32_t GoEChargerAPI::getRebootTimer(void) { return getUint32("rbt"); }

//! Get vehicle state - 1: ready, no vehicle, 2: charging, 3: waiting for vehicle, 4: charging finished, vehicle still connected.
inline uint8_t GoEChargerAPI::getVehicleState(void) { return getUint8("car"); }

//! Get maximum charge current - 6-32 Ampere.
inline uint8_t GoEChargerAPI::getMaximumChargeCurrent(void) { return getUint8("amp"); }

//! Get error code - 1: RCCB (FI-protection), 3: PHASE, 8 : NO_GROUND, default: INTERNAL.
inline uint8_t GoEChargerAPI::getError(void) { return getUint8("err"); }

//! Get access_state - 0: no restriction, 1 : RFID/app required, 2: energy price dependent / automatic
inline uint8_t GoEChargerAPI::getAccessState(void) { return getUint8("ast"); }

//! Get allow_charging state - PWM signal may be asserted  0: no, 1 : yes
inline uint8_t GoEChargerAPI::getAllowChargingState(void) { return getUint8("alw"); }

//! Get stop_state: automatic switch-off  0: disabled, 2: after energy limit for charging has reached
inline uint8_t GoEChargerAPI::getStopState(void) { return getUint8("stp"); }

//! Get type2 cable coding : 13-32: cable coding in ampere, 0: no cable attached
inline uint8_t GoEChargerAPI::getCableCoding(void) { return getUint8("cbl"); }

//! Get relais/contactor voltage status - binary flags: 0b00ABCDEF  A - phase 3 before relay  B - phase 2 before relay  C - phase 1 before relay  D - phase 3 after relay  E - phase 2 after relay  F -phase 1 after relay.
inline uint8_t GoEChargerAPI::getRelaisState(void) { return getUint8("pha"); }

//! Get overall temperature in celsius.
inline uint8_t GoEChargerAPI::getTemperature(void) { return getUint8("tmp"); }    // always return 0°C

//! Get array of 4 internal temperature measurements in celsius.
inline std::array<double, 4> GoEChargerAPI::getTemperatureArray(void) { return (map != NULL ? GoEChargerDataMap::convertToArray<double, 4>(map->find("tma")) : GoEChargerDataMap::invalidArray<double, 4>()); }

//! Get maximum charge current when limited by temperature - 6-32 Ampere.
inline uint8_t GoEChargerAPI::getMaximumChargeCurrentTemperatureLimited(void) { return getUint8("amt"); }

//! Get energy provided in this charging operation in deka Ws.
inline uint32_t GoEChargerAPI::getEnergy(void) { return getUint32("dws"); }

//! Get energy limit before this charging operation is terminated.
inline uint32_t GoEChargerAPI::getEnergyLimitForCharging(void) { return getUint32("dwo"); }

//! Get input adapter status - 0: no adapter  1: 16A adapter.
inline uint8_t GoEChargerAPI::getInputAdapterState(void) { return getUint8("adi"); }

//! Get unlocked by rfid indes - index of the RFID Karte that unlocked this charging operation.
inline uint8_t GoEChargerAPI::getUnlockedByRFIDIndex(void) { return getUint8("uby"); }

//! Get total energy provided by this wallbox - in 0.1kWh, 130 => 13kWh.
inline uint32_t GoEChargerAPI::getEnergyTotal(void) { return getUint32("eto"); }

//! Get wifi status - 3: connected,  default: not connected.
inline uint8_t GoEChargerAPI::getWifiState(void) { return getUint8("wst"); }

//! Get transmit interface - 0: for Home+.
inline uint8_t GoEChargerAPI::getTransmitInterface(void) { return getUint8("txi"); }

//! Get voltage, current, power and power factor measurements.
//! nrg[0] : Voltage L1 in volts
//! nrg[1] : Voltage L2 in volts
//! nrg[2] : Voltage L3 in volts
//! nrg[3] : Voltage N in volts
//! nrg[4] : Current L1 in 0.1A (123 => 12.3A)
//! nrg[5] : Current L2 in 0.1A
//! nrg[6] : Current L3 in 0.1A
//! nrg[7] : Power L1 in 0.1kW (36 => 3.6kW)
//! nrg[8] : Power L2 in 0.1kW
//! nrg[9] : Power L3 in 0.1kW
//! nrg[10]: Power N in 0.1kW
//! nrg[11]: Power total 0.01kW (360 => 3.6kW)
//! nrg[12]: Power factor L1 in %
//! nrg[13]: Power factor L2 in %
//! nrg[14]: Power factor L3 in %
//! nrg[15]: Power factor N in %
inline std::array<uint32_t, 16> GoEChargerAPI::getSensorArray(void) { return (map != NULL ? GoEChargerDataMap::convertToArray<uint32_t, 16>(map->find("nrg")) : GoEChargerDataMap::invalidArray<uint32_t, 16>()); }

//! Get firmware version string.
inline std::string GoEChargerAPI::getFirmwareVersion(void) { return getString("fwv"); }

//! Get serial number - integer formatted as a %06d string.
inline std::string GoEChargerAPI::getSerialNumber(void) { return getString("sse"); }

//! Get wifi ssid.
inline std::string GoEChargerAPI::getWifiSSID(void) { return getString("wss"); }

//! Get wifi password - returns "*" character sequence.
inline std::string GoEChargerAPI::getWifiPassword(void) { return getString("wke"); }

//! Is wifi enabled - 0: disabled  1: enabled.
inline uint8_t GoEChargerAPI::isWifiEnabled(void) { return getUint8("wen"); }

//! Is cloud disabled - 0: cloud enabled  1: cloud disabled.
inline uint8_t GoEChargerAPI::isCloudDisabled(void) { return getUint8("cdi"); }

//! Get time zone offset - returns 100 + offset in hours; example 101 => GMT + 1.
inline uint8_t GoEChargerAPI::getTimeZoneOffset(void) { return getUint8("tof"); }

//! Get daylight saving time offset - in hours; example 1 for summer time in Europe.
inline uint8_t GoEChargerAPI::getDaylightSavingTimeOffset(void) { return getUint8("tds"); }

//! Get LED brightness- 0: LED off, 255: LED maximum brightness.
inline uint8_t GoEChargerAPI::getLEDBrightness(void) { return getUint8("lbr"); }

//! Get minimum number of hours for automatic charging - e.g. 2: after 2 hours it is charged enough.
inline uint8_t GoEChargerAPI::getMinimumHoursForAutomaticCharging(void) { return getUint8("aho"); }

//! Get hour when automatic charging has to be completed - e.g. 7: the automatic charging of at least the minimum number of hours has be completed by 7am.
inline uint8_t GoEChargerAPI::getLastHourForAutomaticCharging(void) { return getUint8("afi"); }

//! Get awattar price zone - 0: Austria, 1. Germany.
inline uint8_t GoEChargerAPI::getAwattarPriceZone(void) { return getUint8("azo"); }

//! Get absolute maximum current in ampere that is configurable in the app.
inline uint8_t GoEChargerAPI::getMaximumConfigurableCurrent(void) { return getUint8("ama"); }

//! Get current setting for the given button in ampere - 6-32: Ampere, 0: disabled; current must monotonically increase with higher button indexes.
inline uint8_t GoEChargerAPI::getCurrentSettingForButton(const unsigned i) { return getUint8(al_command(i)); }

//! Get RGB color value for idle/standby status (no vehicle attached) - 0x00RRGGBB where RR is the hexadecimal representation of red brightness from 0 to 255.
inline uint32_t GoEChargerAPI::getColorValueForIdleState(void) { return getUint32("cid"); }

//! Get RGB color value for charging status - 0x00RRGGBB where RR is the hexadecimal representation of red brightness from 0 to 255.
inline uint32_t GoEChargerAPI::getColorValueForChargingState(void) { return getUint32("cch"); }

//! Get RGB color value for finished status - 0x00RRGGBB where RR is the hexadecimal representation of red brightness from 0 to 255.
inline uint32_t GoEChargerAPI::getColorValueForFinishedState(void) { return getUint32("cfi"); }

//! Get LED energy saving mode - 0: disabled, 1: LED automatically switches off after 10 seconds.
inline uint8_t GoEChargerAPI::getLEDEnergySavingState(void) { return getUint8("lse"); }

//! Get cable locking status - 0: locked while vehicle is connected, 1: automatically unlock when charging is completed, 2: always locked
inline uint8_t GoEChargerAPI::getCableUnlockState(void) { return getUint8("ust"); }

//! Get wifi hotspot password as a string.
inline std::string GoEChargerAPI::getWifiHotspotPassword(void) { return getString("wak"); }

//! Get miscalleneous API settings: Flags  0b1: HTTP Api activated in wifi network (0:no, 1: yes)  0b10 : end-to-end encryption activated (0:no, 1: yes).
inline uint8_t GoEChargerAPI::getAPISettings(void) { return getUint8("r1x"); }

//! Get remaining time in milliseconds until activation by tariff based app-logic:  if (json.car == 1) message = "Connect vehicle first" else message = "Remaining time: …".
inline uint8_t GoEChargerAPI::getRemainingTimeUntilTariffTriggeredCharging(void) { return getUint8("dto"); }

//! Get Norwegian mode setting - 0: deactivated (ground detection active), 1: activated (ground detection deactivated).
inline uint8_t GoEChargerAPI::getNorwegianMode(void) { return getUint8("nmo"); }

//! Get scheduler settings - base 64 encoded scheduler settings (see https://gist.github.com/peterpoetzi/6cd2fad2a915a2498776912c5aa137a8) r21 = Math.floor(encode(1));  r31 = Math.floor(encode(2));  r41 = Math.floor(encode(3)).
inline std::string GoEChargerAPI::getSchedulerSettings(void) { return getString("sch"); }

//! Get scheduler override by button double click setting: activates charging by double clicking the button, if charging was just interrupted by the scheduler 0: function deactivated, 1: function activated - immediate charging allowed.
inline uint8_t GoEChargerAPI::getSchedulerOverrideByButtonClickSetting(void) { return getUint8("sdp"); }

//! Get total energy charged by the given RFID card 1-10 - card 1 is index 0, card 2 is index 1, ... example getTotalEnergyByRFID(0) = 1400: 140kWh charged by RFID card 1.
inline uint32_t GoEChargerAPI::getRFIDCardTotalEnergy(const unsigned i) { return getUint32(ec_command(i)); }

//! Get RFID card identifier - this is a string of hex characters, e.g. 052E85473.
inline std::string GoEChargerAPI::getRFIDCardIdentifier(const unsigned i) { return getString(rc_command(i)); }

//! Get RFID card name as a string.
inline std::string GoEChargerAPI::getRFIDCardName(const unsigned i) { return getString(rn_command(i)); }

//! Get load management enabled - 0: disabled  1: enabled via cloud.
inline uint8_t GoEChargerAPI::isLoadManagementEnabled(void) { return getUint8("loe"); }

//! Get load management total current for group in ampere.
inline uint8_t GoEChargerAPI::getLoadManagementGroupTotalCurrent(void) { return getUint8("lot"); }

//! Get load management minimum current in ampere.
inline uint8_t GoEChargerAPI::getLoadManagementMinimumCurrent(void) { return getUint8("lom"); }

//! Get load management priority.
inline uint8_t GoEChargerAPI::getLoadManagementPriority(void) { return getUint8("lop"); }

//! Get load management group id as string.
inline std::string GoEChargerAPI::getLoadManagementGroupID(void) { return getString("log"); }

//! Get load management expected number of stations (not supported yet).
inline uint8_t GoEChargerAPI::getLoadManagementExpectedNumberOfStations(void) { return getUint8("lon"); }

//! Get load management fallback current in ampere.
inline uint8_t GoEChargerAPI::getLoadManagementFallbackCurrent(void) { return getUint8("lof"); }

//! Get load management allowed current in ampere - automatically controlled by load management.
inline uint8_t GoEChargerAPI::getLoadManagementCurrent(void) { return getUint8("loa"); }

//! Get load management seconds since last energy flow whíle vehicle is connected, 0: if charging is in progress.
inline uint32_t GoEChargerAPI::getLoadManagementIdleSeconds(void) { return getUint32("lch"); }

//! Get custom MQTT enabled - 0: disabled, 1: enabled.
inline uint8_t GoEChargerAPI::isCustomMQTTEnabled(void) { return getUint8("mce"); }

//! Get custom MQTT server name as string.
inline std::string GoEChargerAPI::getCustomMQTTServerHostname(void) { return getString("mcs"); }

//! Get custom MQTT server port.
inline uint16_t GoEChargerAPI::getCustomMQTTServerPort(void) { return getUint16("mcp"); }

//! Get custom MQTT server username.
inline std::string GoEChargerAPI::getCustomMQTTUsername(void) { return getString("mcu"); }

//! Get custom MQTT server password.
inline std::string GoEChargerAPI::getCustomMQTTPassword(void) { return getString("mck"); }

//! Get custom MQTT server connection state - 0: not connected, 1: connected.
inline uint8_t GoEChargerAPI::isCustomMQTTConnected(void) { return getUint8("mcc"); }


//! Set maximum charge current - 6-32 Ampere.
inline bool GoEChargerAPI::setMaximumChargeCurrent(const uint8_t value) { return setUint8("amp", value); }

//! Set volatile value of maximum charge current - 6-32 Ampere.
inline bool GoEChargerAPI::setMaximumChargeCurrentTemperatureLimited(const uint8_t value) { return setUint8("amt", value); }

//! Set access_state - 0: no restriction, 1 : RFID/app required, 2: energy price dependent / automatic
inline bool GoEChargerAPI::setAccessState(const uint8_t value) { return setUint8("ast", value); }

//! Set allow_charging state - PWM signal may be asserted  0: no, 1 : yes
inline bool GoEChargerAPI::setAllowChargingState(const uint8_t value) { return setUint8("alw", value); }

//! Set stop_state: automatic switch-off  0: disabled, 2: after energy limit for charging has reached
inline bool GoEChargerAPI::setStopState(const uint8_t value) { return setUint8("stp", value); }

//! Set energy limit before this charging operation is terminated.
inline bool GoEChargerAPI::setEnergyLimitForCharging(const uint32_t value) { return setUint32("dwo", value); }

//! Set wifi ssid.
inline bool GoEChargerAPI::setWifiSSID(const std::string& value) { return setString("wss", value); }

//! Set wifi password.
inline bool GoEChargerAPI::setWifiPassword(const std::string& value) { return setString("wke", value); }

//! Set wifi enabled - 0: disabled  1: enabled.
inline bool GoEChargerAPI::setWifiEnabled(const uint8_t value) { return setUint8("wen", value); }

//! Set time zone offset - returns 100 + offset in hours; example 101 => GMT + 1.
inline bool GoEChargerAPI::setTimeZoneOffset(const uint8_t value) { return setUint8("tof", value); }

//! Set daylight saving time offset - in hours; example 1 for summer time in Europe.
inline bool GoEChargerAPI::setDaylightSavingTimeOffset(const uint8_t value) { return setUint8("tds", value); }

//! Set LED brightness- 0: LED off, 255: LED maximum brightness.
inline bool GoEChargerAPI::setLEDBrightness(const uint8_t value) { return setUint8("lbr", value); }

//! Set minimum number of hours for automatic charging - e.g. 2: after 2 hours it is charged enough.
inline bool GoEChargerAPI::setMinimumHoursForAutomaticCharging(const uint8_t value) { return setUint8("aho", value); }

//! Set hour when automatic charging has to be completed - e.g. 7: the automatic charging of at least the minimum number of hours has be completed by 7am.
inline bool GoEChargerAPI::setLastHourForAutomaticCharging(const uint8_t value) { return setUint8("afi", value); }

//! Set absolute maximum current in ampere that is configurable in the app.
inline bool GoEChargerAPI::setMaximumConfigurableCurrent(const uint8_t value) { return setUint8("ama", value); }

//! Set current setting for the given button in ampere - 6-32: Ampere, 0: disabled; current must monotonically increase with higher button indexes.
inline bool GoEChargerAPI::setCurrentSettingForButton(const unsigned i, const uint8_t value) { return setUint8(al_command(i), value); }

#endif



