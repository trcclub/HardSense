
AddItemToDisplayQueue(DisplayCommands::UpdateValue, String <payload>);
payload = "<key>,<value>"



Custom Partition for HardSenseESP

Default direcectory for existing partisions is:
(Windows):
C:\Users\<USERID>\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.4\tools\partitions

New partion file is called:
hardsense_esp_partition.csv


The file 'boards.txt' is also important:
C:\Users\<USERID>\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.4\boads.txt


esp32.menu.PartitionScheme.hardsenseESP=HardSense ESP Partition (1.xMB APP with OTA/xxxKB SPIFFS)
esp32.menu.PartitionScheme.hardsenseESP.build.partitions=hardsense_esp_partition
esp32.menu.PartitionScheme.hardsenseESP.upload.maximum_size=1966080