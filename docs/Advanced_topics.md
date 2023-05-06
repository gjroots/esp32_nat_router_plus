# ESP32-NAT Router +

## Advanced topics

### Interpreting the on board LED :

The on-board LED of the ESP32 NAT Router serves as an indicator of its status. If the device is connected to the upstream access point, the LED will be turned on. 

For example, 

if one device is connected to the ESP32 and the device is connected to the upstream access point, the LED will blink as follows: `*****.*****. `

Conversely, if two devices are connected to the ESP32, but the device is not connected to the upstream access point, the LED will blink as follows: `....*.*....`

### Button Menu :

The Button Menu feature of the ESP32-NATS Router + currently provides three settings that can be accessed by following these steps:

- Toggle Led   (which allows you to turn the LED on or off temporarily)

- Toggle web server  (which allows you to turn on or off the webserver temporarily

- Erase Flash   (which will erase all data from flash memory).
1. Hold down the **BOOT** button for 3 seconds to enter the menu mode. The LED will indicate that the device has entered the menu mode.

2. Click the button once to cycle through the menu options. 

3. Holding the button for 3 seconds will select the current menu option. The LED will indicate current menu step and also succes indication after long press for 3sec. 

To go back to the normal mode, perform more than 3 steps. Note that if you release the button without selecting any option, the menu will reset automatically after 15 sec. 

### OTA Update :

OTA (Over-The-Air) update is a method of updating the firmware of a device through a wireless network connection. This method eliminates the need for physical access to the device and can be done remotely, making it a convenient way to update the firmware.
 To perform an OTA update on the ESP32-NATS Router +, follow these steps:

1. Go to the "Info" page in the web interface of the router
2. Click on the "Update" button
3. Select the desired firmware for the update

Once the update process begins, the device will automatically download and install the new firmware. This process may take a few minutes to complete, depending on the size of the firmware update.

Please note that during an OTA update, the device may become temporarily unavailable or unresponsive. This is normal and should not be a cause for concern.
Also, it's important to ensure that the firmware being updated is compatible with the device and that there is a stable network connection. Failure to do so can result in errors and damage to the device.
By following these steps, you can update the firmware of your ESP32-NATS Router + remotely and keep it running smoothly with the latest features and improvements. 

### Settings :

They are several settings are provided to manage router insections :

1. Custmization

2. Admin settings

3. AP settings

4. STA settings

5. Static IP Settings

6. Advanced settings

#### Custmization :

- Enable dark mode:  This setting is used to switch the UI interface between light and dark mode

- Enable LED indicator: Enables or disables the onboard LED

- Enable web server: Enables the web interface to access settings and other features.

#### Admin Settings :

- User name: This is the username that is required to log in to the web interface of the router. 
- Password: This is the password that is required to log in to the web interface of the router. 
- Login failed attempts: This setting determines how many failed login attempts are allowed before a user is locked out of the web interface. 
- Lock-out period (minutes): This setting determines how long a user is locked out of the web interface after exceeding the maximum number of failed login attempts.

By configuring these settings, you can enhance the security of your ESP32 NAT Router and ensure that only authorized users have access to the web interface.

#### Advanced settings :

- Ap IP:  You can customize the IP address of the access point to acces Web interface. 
- Custom DNS: The advanced settings feature also allows you to add custom DNS servers to the configuration. By doing so, you can prioritize specific DNS servers or configure alternative DNS servers based on your needs.
- Random MAC Address: To configure a random MAC address for the access point. This feature enhances security and privacy by preventing the access point's MAC address from being tracked or identified by unauthorized parties.

### Command Line Interface

To configure the WiFi settings of the ESP32 NAT Router, you can use a serial console such as Putty or GtkTerm with a baud rate of 115200 bps. Use the "set_sta" and "set_ap" command to configure the WiFi settings. These changes are stored persistently in NVS and will be applied after the device is restarted. You can use the "show" command to view the current configuration. The NVS namespace used for the configuration parameters is "esp32_nat".

```
help 
  Print the list of registered commands

free 
  Get the current size of free heap memory

heap 
  Get minimum size of free heap memory that was available during program execu
  tion

version 
  Get version of chip and SDK

restart 
  Software reset of the chip

deep_sleep  [-t <t>] [--io=<n>] [--io_level=<0|1>]
  Enter deep sleep mode. Two wakeup modes are supported: timer and GPIO. If no
  wakeup option is specified, will sleep indefinitely.
  -t, --time=<t>  Wake up time, ms
      --io=<n>  If specified, wakeup using GPIO with given number
  --io_level=<0|1>  GPIO level to trigger wakeup

light_sleep  [-t <t>] [--io=<n>]... [--io_level=<0|1>]...
  Enter light sleep mode. Two wakeup modes are supported: timer and GPIO. Mult
  iple GPIO pins can be specified using pairs of 'io' and 'io_level' arguments
  . Will also wake up on UART input.
  -t, --time=<t>  Wake up time, ms
      --io=<n>  If specified, wakeup using GPIO with given number
  --io_level=<0|1>  GPIO level to trigger wakeup

tasks 
  Get information about running tasks

nvs_set  <key> <type> -v <value>
  Set key-value pair in selected namespace.
Examples:
 nvs_set VarName i32 -v 
  123 
 nvs_set VarName str -v YourString 
 nvs_set VarName blob -v 0123456789abcdef 
         <key>  key of the value to be set
        <type>  type can be: i8, u8, i16, u16 i32, u32 i64, u64, str, blob
  -v, --value=<value>  value to be stored

nvs_get  <key> <type>
  Get key-value pair from selected namespace. 
Example: nvs_get VarName i32
         <key>  key of the value to be read
        <type>  type can be: i8, u8, i16, u16 i32, u32 i64, u64, str, blob

nvs_erase  <key>
  Erase key-value pair from current namespace
         <key>  key of the value to be erased

nvs_namespace  <namespace>
  Set current namespace
   <namespace>  namespace of the partition to be selected

nvs_list  <partition> [-n <namespace>] [-t <type>]
  List stored key-value pairs stored in NVS.Namespace and type can be specified
  to print only those key-value pairs.

Following command list variables stored inside 'nvs' partition, under namespace 'storage' with type uint32_t
  Example: nvs_list nvs -n storage -t u32 

   <partition>  partition name
  -n, --namespace=<namespace>  namespace name
  -t, --type=<type>  type can be: i8, u8, i16, u16 i32, u32 i64, u64, str, blob

nvs_erase_namespace  <namespace>
  Erases specified namespace
   <namespace>  namespace to be erased

set_sta  <ssid> <passwd>
  Set SSID and password of the STA interface
        <ssid>  SSID
      <passwd>  Password
  --, -u, ----username=<ent_username>  Enterprise username
  --, -a, ----anan=<ent_identity>  Enterprise identity

set_sta_static  <ip> <subnet> <gw>
  Set Static IP for the STA interface
          <ip>  IP
      <subnet>  Subnet Mask
          <gw>  Gateway Address

set_ap  <ssid> <passwd>
  Set SSID and password of the SoftAP
        <ssid>  SSID of AP
      <passwd>  Password of AP

set_ap_ip  <ip>
  Set IP for the AP interface
          <ip>  IP

portmap  [add|del] [TCP|UDP] <ext_portno> <int_ip> <int_portno>
  Add or delete a portmapping to the router
     [add|del]  add or delete portmapping
     [TCP|UDP]  TCP or UDP port
  <ext_portno>  external port number
      <int_ip>  internal IP
  <int_portno>  internal port number

show 
  Get status and config of the router
```
