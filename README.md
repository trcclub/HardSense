# HardSense
Monitor local system sensors and display that data on a remote 2.8" LCD screen.
Full touch screen enabled.
Multiple screens including GPU details, CPU details

https://youtu.be/yXnQ3QwIAQI


The hardware monitoring is provided by the Open Hardware Monitor library:  https://github.com/openhardwaremonitor/openhardwaremonitor

NOTE:
To buld HardSenseESP, you need to change the partition scheme.

HardSenseESP Tools configuration settings:
	Board:  "ESP32 Dev Module"
	Partition Scheme: "HardSense ESP Partition"

In Arduino IDE, these are under the 'Tools' menu item.
In vMicro (Visual Micro for Visual Studio), they are under 'vMicro' menu item.

Essential bits: 
Bodmers TFT_ESPI:  https://github.com/Bodmer/TFT_eSPI
