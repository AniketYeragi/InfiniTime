#pragma once

#include <cstdint>
#include <atomic>
#include <memory>

#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#undef max
#undef min
#include "AlertNotificationClient.h"
#include "AlertNotificationService.h"
#include "BatteryInformationService.h"
#include "CurrentTimeClient.h"
#include "CurrentTimeService.h"
#include "DeviceInformationService.h"
#include "DfuService.h"
#include "ImmediateAlertService.h"
#include "MusicService.h"
#include "NavigationService.h"
#include "ServiceDiscovery.h"
#include "HeartRateService.h"
#include "components/heartrate/HeartRateController.h"

namespace Pinetime {
  namespace Drivers {
    class SpiNorFlash;
  }

  namespace System {
    class SystemTask;
  }

  namespace Controllers {
    class Ble;
    class DateTime;
    class NotificationManager;

    class NimbleController {

    public:
      NimbleController(Pinetime::System::SystemTask& systemTask,
                       Pinetime::Controllers::Ble& bleController,
                       DateTime& dateTimeController,
                       Pinetime::Controllers::NotificationManager& notificationManager,
                       Controllers::Battery& batteryController,
                       Pinetime::Drivers::SpiNorFlash& spiNorFlash,
                       Controllers::HeartRateController& heartRateController);
      void Init();
      void StartAdvertising();
      int StopAdvertising();
      void BeaconAdvertising();
      int OnGAPEvent(ble_gap_event* event);

      int OnDiscoveryEvent(uint16_t i, const ble_gatt_error* pError, const ble_gatt_svc* pSvc);
      int OnCTSCharacteristicDiscoveryEvent(uint16_t connectionHandle, const ble_gatt_error* error, const ble_gatt_chr* characteristic);
      int OnANSCharacteristicDiscoveryEvent(uint16_t connectionHandle, const ble_gatt_error* error, const ble_gatt_chr* characteristic);
      int OnCurrentTimeReadResult(uint16_t connectionHandle, const ble_gatt_error* error, ble_gatt_attr* attribute);
      int OnANSDescriptorDiscoveryEventCallback(uint16_t connectionHandle,
                                                const ble_gatt_error* error,
                                                uint16_t characteristicValueHandle,
                                                const ble_gatt_dsc* descriptor);

      void StartDiscovery();

      Pinetime::Controllers::MusicService& music() {
        return musicService;
      };
      Pinetime::Controllers::NavigationService& navigation() {
        return navService;
      };
      Pinetime::Controllers::AlertNotificationService& alertService() {
        return anService;
      };

      uint16_t connHandle();
      uint8_t myArray[4] = {0x59,0x00,0x00,0x00};
      uint8_t emergencyCounter;
      bool ConnectEmergency = false;

    private:
      static constexpr const char* deviceName = "InfiniTime";
      static constexpr const char* deviceNameBeacon = "GEMTEC";
      Pinetime::System::SystemTask& systemTask;
      Pinetime::Controllers::Ble& bleController;
      DateTime& dateTimeController;
      Pinetime::Controllers::NotificationManager& notificationManager;
      Pinetime::Drivers::SpiNorFlash& spiNorFlash;
      Pinetime::Controllers::DfuService dfuService;

      DeviceInformationService deviceInformationService;
      CurrentTimeClient currentTimeClient;
      AlertNotificationService anService;
      AlertNotificationClient alertNotificationClient;
      CurrentTimeService currentTimeService;
      MusicService musicService;
      NavigationService navService;
      BatteryInformationService batteryInformationService;
      ImmediateAlertService immediateAlertService;
      HeartRateService heartRateService;
      HeartRateController& heartRateController2;

      uint8_t addrType; // 1 = Random, 0 = PUBLIC
      uint16_t connectionHandle = 0;
      std::atomic<bool> beaconToggle {false};

      ble_uuid128_t dfuServiceUuid {
        .u {.type = BLE_UUID_TYPE_128},
        .value = {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x30, 0x15, 0x00, 0x00}};

      ServiceDiscovery serviceDiscovery;
    };
  }
}
