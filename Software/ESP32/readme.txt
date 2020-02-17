
AddItemToDisplayQueue(DisplayCommands::UpdateValue, String <payload>);
payload = "<key>,<value>"

Value (when needed) must be at least 3 characters long.
This means to have an empty value, you must use a space.
examples:
AddItemToDisplayQueue(DisplayCommands::UpdateValue, String ("a, "));
AddItemToDisplayQueue(DisplayCommands::UpdateValue, String ("a,12.3"));
AddItemToDisplayQueue(DisplayCommands::UpdateValue, String ("a, Hello"));

These will fail:
AddItemToDisplayQueue(DisplayCommands::UpdateValue, String ("a,"));
AddItemToDisplayQueue(DisplayCommands::UpdateValue, String ("a"));



Custom Partition for HardSenseESP

Default direcectory for existing partisions is:
(Windows):
C:\Users\<USERID>\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.4\tools\partitions

New partion file is called:
hardsense_esp_partition.csv


'hardsense_esp_partition.csv' is based on the file 'min_spiffs.csv' which can be found
in the default partitions directory.  This file is a good starting point when creating
a custom project. 


The file 'boards.txt' is also important:
C:\Users\<USERID>\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.4\boads.txt


esp32.menu.PartitionScheme.hardsenseESP=HardSense ESP Partition (1.9MB APP with OTA/190KB SPIFFS)
esp32.menu.PartitionScheme.hardsenseESP.build.partitions=hardsense_esp_partition
esp32.menu.PartitionScheme.hardsenseESP.upload.maximum_size=1966080



Copy the file 'hardsense_esp_partition.csv' to your partitions directory.
Add the 3 'esp32.menu.' lines above to your boards.txt file.
Restart arduino.
Select 'HardSense ESP Partition' from the tools or board config  menus.


Reference links:
https://robotzero.one/arduino-ide-partitions/
https://lastminuteengineers.com/esp32-ota-updates-arduino-ide/


Sketch uses 1387078 bytes (70%) of program storage space. Maximum is 1966080 bytes.
1388578
1387426


-------------
Added basic OTA (Merged branch 19 into master, this was this build size)
    Sketch uses 1490546 bytes (75%) of program storage space. Maximum is 1966080 bytes.

After adding all OTA and Display screen related code:
    Sketch uses 1491802 bytes (75%) of program storage space. Maximum is 1966080 bytes.

That's only 1,256 bytes to add the working OTA display screen and realtime updates.  NICE!


------------------
Sketch uses 1491786 bytes (75%) of program storage space. Maximum is 1966080 bytes.