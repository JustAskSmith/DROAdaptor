This is code to use a Seeed SAMD21 XIAO as a translator for a weird scale and DIY TouchDRO bluetooth adaptor.
The 6" quill scale I have (iGaging T32271 I think) generates its own clock signal so 
it is incompatible with the TouchDRO_Univrsal_DIY_V1.6 firmware on an ESP32. The Touch DRO
adaptor will not properly recognize the self clocked 21 bit 2's complement data stream and fail to initialize.
The purpose of the adaptor is to sit between the scale and the Touch DRO adaptor
and translate the scale output to an externally clocked 21 bit 2's complement data stream.
The adaptor also inverts the data bit as the quill scale emit ones as +VSS where the standard
longer scales on my table emit ones as ground.

The XIAO and the quill scale is easily powered from the 3v3 output of the Touch DRO adaptor.

```Scale         XIAO       Touch DRO  
VSS ------    3v3    ----- VSS  
D-  ------  D1    D3 ----- D-   
D+  ------  D2    D4 ----- D+  
GND ------    GND    ----- GND``` 
