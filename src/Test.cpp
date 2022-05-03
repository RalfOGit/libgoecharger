#if 1
#include <GoEChargerAPI.hpp>
using namespace libgoecharger;

int main(int argc, char** argv) {

    // configure go-eCharger API
    GoEChargerAPI api("http://192.168.178.19/");

    // send http status request to the go-eCharger wallbox
    if (api.refreshMap() == true) {

        // retrieve properties/settings from the go-eCharger API
        std::string version = api.getVersion();
        std::string date = api.getDateTime();
        uint32_t rbc = api.getRebootCounter();
        uint32_t rbt = api.getRebootTimer();
        uint8_t  car = api.getVehicleState();
        uint8_t  amp_ = api.getMaximumChargeCurrent();
        uint8_t  amt = api.getMaximumChargeCurrentTemperatureLimited();
        uint8_t  err = api.getError();
        uint8_t  ast = api.getAccessState();
        uint8_t  alw = api.getAllowChargingState();
        uint8_t  stp = api.getStopState();
        uint8_t  cbl = api.getCableCoding();
        uint8_t  pha = api.getRelaisState();
        uint8_t  tmp = api.getTemperature();
        std::array<double, 4> tma = api.getTemperatureArray();
        uint32_t dws = api.getEnergy();
        uint32_t dwo = api.getEnergyLimitForCharging();
        uint8_t  adi = api.getInputAdapterState();
        uint8_t  uby = api.getUnlockedByRFIDIndex();
        uint32_t eto = api.getEnergyTotal();
        uint8_t  wst = api.getWifiState();
        uint8_t  txi = api.getTransmitInterface();
        std::array<uint32_t, 16> nrg = api.getSensorArray();
        std::string fwv = api.getFirmwareVersion();
        std::string sse = api.getSerialNumber();
        std::string wss = api.getWifiSSID();
        std::string wke = api.getWifiPassword();
        uint8_t wen = api.isWifiEnabled();
        uint8_t cdi = api.isCloudDisabled();
        uint8_t tof = api.getTimeZoneOffset();
        uint8_t tds = api.getDaylightSavingTimeOffset();
        uint8_t lbr = api.getLEDBrightness();
        uint8_t aho = api.getMinimumHoursForAutomaticCharging();
        uint8_t afi = api.getLastHourForAutomaticCharging();
        uint8_t azo = api.getAwattarPriceZone();
        uint8_t ama = api.getMaximumConfigurableCurrent();
        uint8_t al1 = api.getCurrentSettingForButton(0);
        uint8_t al2 = api.getCurrentSettingForButton(1);
        uint8_t al3 = api.getCurrentSettingForButton(2);
        uint8_t al4 = api.getCurrentSettingForButton(3);
        uint8_t al5 = api.getCurrentSettingForButton(4);
        uint32_t cid = api.getColorValueForIdleState();
        uint32_t cch = api.getColorValueForChargingState();
        uint32_t cfi = api.getColorValueForFinishedState();
        uint8_t lse = api.getLEDEnergySavingState();
        uint8_t ust = api.getCableUnlockState();
        std::string wak = api.getWifiHotspotPassword();
        uint8_t r1x = api.getAPISettings();
        uint8_t dto = api.getRemainingTimeUntilTariffTriggeredCharging();
        uint8_t nmo = api.getNorwegianMode();
        std::string sch = api.getSchedulerSettings();
        uint8_t sdp = api.getSchedulerOverrideByButtonClickSetting();
        uint32_t ec0 = api.getRFIDCardTotalEnergy(0);
        std::string re0 = api.getRFIDCardIdentifier(0);
        std::string rn0 = api.getRFIDCardName(0);
        uint32_t ec9 = api.getRFIDCardTotalEnergy(9);
        std::string re9 = api.getRFIDCardIdentifier(9);
        std::string rn9 = api.getRFIDCardName(9);
        uint8_t loe = api.isLoadManagementEnabled();
        uint8_t lot = api.getLoadManagementGroupTotalCurrent();
        uint8_t lom = api.getLoadManagementMinimumCurrent();
        uint8_t lop = api.getLoadManagementPriority();
        std::string log = api.getLoadManagementGroupID();
        uint8_t lon = api.getLoadManagementExpectedNumberOfStations();
        uint8_t lof = api.getLoadManagementFallbackCurrent();
        uint8_t loa = api.getLoadManagementCurrent();
        uint32_t lch = api.getLoadManagementIdleSeconds();
        uint8_t mce = api.isCustomMQTTEnabled();
        std::string mcs = api.getCustomMQTTServerHostname();
        uint16_t mcp = api.getCustomMQTTServerPort();
        std::string mcu = api.getCustomMQTTUsername();
        std::string mck = api.getCustomMQTTPassword();
        uint8_t mcc = api.isCustomMQTTConnected();

        // set the amx property through the API - with other properties you must be careful about the write-cycle limit of the eeprom
        bool b2 = api.setVolatileMaximumChargeCurrent(3);

        //bool b1 = api.setMaximumChargeCurrent(amp_);
        //bool b2 = api.setVolatileMaximumChargeCurrent(3);
        //bool b3 = api.setAccessState(ast);
        //bool b4 = api.setAllowChargingState(alw);
        //bool b5 = api.setStopState(stp);
        //bool b6 = api.setEnergyLimitForCharging(dwo);
        ////bool b7 = api.setWifiSSID(wss);
        ////bool b8 = api.setWifiPassword(wke);
        ////bool b9 = api.setWifiEnabled(wen);
        //bool b10 = api.setTimeZoneOffset(tof);
        //bool b11 = api.setDaylightSavingTimeOffset(tds);
        //bool b12 = api.setLEDBrightness(lbr);
        //bool b13 = api.setMinimumHoursForAutomaticCharging(aho);
        //bool b14 = api.setLastHourForAutomaticCharging(afi);
        //bool b15 = api.setMaximumConfigurableCurrent(ama);
        //bool b16 = api.setCurrentSettingForButton(0, al1);
        //bool b17 = api.setCurrentSettingForButton(1, al2);
        //bool b18 = api.setCurrentSettingForButton(2, al3);
        //bool b19 = api.setCurrentSettingForButton(3, al4);
        //bool b20 = api.setCurrentSettingForButton(4, al5);
        //bool b21 = api.setColorValueForIdleState(cid);
        //bool b22 = api.setColorValueForChargingState(cch);
        //bool b23 = api.setColorValueForFinishedState(cfi);
        //bool b24 = api.setLEDEnergySavingState(lse);
        //bool b25 = api.setCableUnlockState(ust);
        ////bool b26 = api.setWifiHotspotPassword(wak);
        //bool b27 = api.setAPISettings(r1x);
        //bool b28 = api.setRemainingTimeUntilTariffTriggeredCharging(dto);
        //bool b29 = api.setNorwegianMode(nmo);
        //bool b30 = api.setRFIDCardName(0, rn0);
        //bool b31 = api.setRFIDCardName(0, rn9);
    }

    return 0;
}
#endif
