# test_code_atomic_measurement

This application is a reference implementation of Atomic Measurement, which is mainly used by healthcare Device Types.

It is an APPLICATION-level code which uses and is built on top of IoTivity 1.3.1.

This code passes all Atomic Measurement Test Cases (CT1.2.15 ~ CT1.2.20).

OS: Ubuntu 16.04 LTS

Installation Guide:
1. Create a folder under the same path with IoTivity 1.3.1. (App: /home/hongki/test_code_atomic_measurement, IoTivity: /home/hongki/iotivity)
2. Clone or copy files of this repository to the created folder
3. Type ./start.sh to run the app

Important Files:
1. server.cpp: Blood pressure monitor Device Type (oic.d.bloodpressure)
2. device/bloodpressure0.cpp: Atomic Measurement (oic.r.bloodpressuremonitor-am)
3. device/bloodpressure1.cpp: Linked Resource Type: Blood Pressure (oic.r.blood.pressure)
4. device/bloodpressure2.cpp: Linked Resource Type: Pulse Rate (oic.r.pulserate)
5. PICS/PICS_BPM.json: PICS file for CTT

For bugs or queries, contact Hongki CHA (cha8476@etri.re.kr).
