## test_code_atomic_measurement

This blood pressure monitor application is a reference implementation of Atomic Measurement, a key feature supported by OCF healthcare Device Types.
- OCF: https://openconnectivity.org/

It is an APPLICATION-level code which uses and is built on top of IoTivity 1.3.1.
- IoTivity: https://iotivity.org/

This code passes all Atomic Measurement Test Cases (CT1.2.15 ~ CT1.2.20).

OS: Ubuntu 16.04 LTS

## Installation Guide
1. Create a folder under the same path with IoTivity 1.3.1. (App: /home/hjourdain/OCF/test_code_atomic_measurement, IoTivity: /home/hjourdain/OCF/iotivity-1.3.1)
2. Clone or copy files of this repository to the created folder
3. Type ./build_release.sh to build the app
4. Type ./start.sh to run the app
5. If CTT prompts "Please initiate device to revert to read for OTM", stop the app, replace server.dat with RFOTM/server.dat, run the app again, and then press OK


## Important Files

| File                      |  Description                                                 |
| --------------------------| ------------------------------------------------------------ |
| server.cpp                |  Blood pressure monitor Device Type (oic.d.bloodpressure)    |
| server.idd.dat            |  Blood pressure monitor Introspection Device Data (IDD)      |
| device/bloodpressure0.cpp |  Atomic Measurement (oic.r.bloodpressuremonitor-am)          |
| device/bloodpressure1.cpp |  Linked Resource Type: Blood Pressure (oic.r.blood.pressure) |
| device/bloodpressure2.cpp |  Linked Resource Type: Pulse Rate (oic.r.pulserate)          |
| PICS/PICS_BPM.json        |  PICS file for CTT                                           |
| RFOTM/server.dat          |  Security file to revert app into the RFOTM state            |

