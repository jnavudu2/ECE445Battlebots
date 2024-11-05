## To initialize building process for ESP32:
1. Initialize the esp-idf submodule:
```
cd esp-idf
git submodule init
git submodule update
```

2. Install Tools
```
./install.sh esp32s3
```

3. Before building, plug in ESP32 (using `UART` port) and update usb driver.
- In Windows, open Powershell as administrator and run:
```
usbipd list
```
Find "CP2102N USB to UART Bridge Controller" and note the busid.
```
usbipd attach --wsl --busid <busid>
```

**Note:** You may have to run `usbipd bind --busid <busid>` first.

- In Linux to verify usb is detected:
```
lsusb
```
- Find usb port in linux:
```
ls /dev/ttyUSB*
```
- The result should be:
```
/dev/ttyUSB0
```

## To build a library
1. Go into specified library
2. Export idf environment
```
. {path_to_esp-idf}/.export.sh
```
3. Build
```
idf.py set-target esp32s3
idf.py build
```
4. Flash target
```
idf.py -p /dev/ttyUSB0 flash
```
5. To monitor output
```
idf.py -p /dev/ttyUSB0 monitor
```
To exit monitor, press `Ctrl + ]`
