# ESP32-Nat Router +

## OTA Handler Component

The OTA Handler is a firmware updater, that allows you to update  ESP32 device over the air. It uses the HTTP POST method to upload the new firmware, making it easy to update devices without needing physical access.

## Usage

To use the OTA Handler, you need to include the `ota_update_handler` function in your code. Here's an example of how to use it:

```c
    #include "ota_handler.h"
    ................
    //send post request to this function
   ota_update_handler(req); 


// run initial in main()

ota_update_task(void *Param); 
```

In `CMakeLists.txt` update `REQUIRES ota_handler`  

The `ota_update_handler` function checks if the firmware binary is present in the request body, verifies that it's valid, and updates the firmware if everything is correct. If the update is successful, it sends a response indicating that the firmware update was successful.

In addition to the `ota_update_task(void *Param)` function, the OTA Handler also provides a task that can be used for continuous checking. If you need to perform continuous checking for firmware updates and also if problem occur it will restore back to old version.

## Credit's

- The OTA update code was inspired by [@Kecin Wolfe](https://gitlab.com/kevinwolfe/esp32_template)'s implementation. 
