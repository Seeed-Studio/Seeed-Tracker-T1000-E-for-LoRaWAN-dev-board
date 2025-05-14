Seeed-Tracker-T1000-E-for-LoRaWAN-dev-board
=================================
This is the application on T1000-E Development Board. 

# Getting Started

1.Install SEGGER Embedded Studio IDE

2.Download the latest nRF SDK: nRF5_SDK_17.1.0_ddde560

3.Update the NEW BOOTLOADER to board

# Build and Run

1.Download the application code

2.Copy to the path: .../nRF5_SDK_17.1.0_ddde560/examples/ble_peripheral/

3.Open example project and bulid

4.Convert the HEX file to UF2 file

5.Enter the DFU mode, drop UF2 file to USB disk

6.Running

# Application List

## Example 

* 00_blinky - Led flash

* 01_button - Print button event

* 02_buzzer - Loop play sound

* 03_sensor - Print temp/lux/battery value

* 04_accelerometer - Print ax/ay/az/event value

* 05_gnss - Print latitude/longitude value

* 06_lorawan - Join through OTAA, send test data to LNS

* 07_lorawan_sensor - Join through OTAA, read temp/lux/bat/ax/ay/az, send data to LNS

* 08_lorawan_gnss - Join through OTAA, scan lat/lon, send data to LNS

* 09_lorawan_wifi - Join through OTAA, scan WiFi MAC, send data to LNS

* 10_lorawan_beacon - Join through OTAA, scan Beacon MAC, send data to LNS

* 11_lorawan_tracker - Join through OTAA, scan GNSS/Wifi/Beacon, send data to LNS, config parameter through SenseCAP APP

# Development Tutorial

* https://wiki.seeedstudio.com/open_source_lorawan/

# Hardware Support
* https://www.seeedstudio.com/SenseCAP-Card-Tracker-T1000-E-for-LoRaWAN-p-6408.html

# Note
* This SDK not support T1000-E for meshtastic

