# Nexion Display Firmware Upgrade using STM Microcontroller through UART from USB

The Nextion Upload Protocol is an advanced technique. Understanding the Nextion Upload Protocol is not a requirement if you only upload your firmware through the Nextion Editor or use the microSD card method. The Nextion Upload Protocol v1.0 can still be used without issues provided:

1.Addressing Mode is not implemented on the Nextion device
2.Protocol Reparse is not implemented on the Nextion device
Both Address Mode and Protocol Reparse are advanced techniques and should not be used if not fully understood. However, additional considerations are needed in the Nextion Upload Protocol should either of these advanced techniques be used.


# Step 1. Know your Nextion’s serial port

In order to set up a serial connection between Nextion and your MCU, you will first have to find which serial port their Nextion is connected to, and figure out the connection parameters. Nextion operates at TTL N81, so primarily you are looking for which serial port, and the Nextion’s operating baud rate.

A commonly used technique to locate the Nextion is to send the connect instruction in different connection parameters until a valid connect string is returned. However, with the introduction of Protocol Reparse and Address Mode in v0.53, newer considerations need to be made. If Protocol Reparse mode is active, connectÿÿÿ would be interpreted as incoming data, and you would first need to return to passive mode before starting the Upload Protocol. If Address Mode is active, then Nextion would need to be addressed via its specific address. Therefore, send the following:

  DRAKJHSUYDGBNCJHGJKSHBDNÿÿÿ
  connectÿÿÿ
  ÿÿconnectÿÿÿ
The first line will cause Nextion to exit active Protocol Reparse and return to passive mode. The second line is the traditional connect instruction. The third line is the connect instruction using the broadcast address of 65535. In this combination, the Nextion will be able to respond with its connect string regardless of which mode it may have been in.

# Step 2. Nextion’s current configuration and address.

Nextion will return a connect string when the right serial port and connection parameters are used. For example, NX4024T032_011R will return 8 groups (as below)

comok 1,38024-2556,NX4024T032_011R,99,61488,D264B8204F0E1828,16777216ÿÿÿ
The connect string description:

comok: the handshaking protocol is setup.
1: Nextion touch abilities: 1 with touch panel, 0 without touch panel
38024-2556: Partially reserved. Portion after hyphen is address. 2556 = 0x09FC
NX4024T032_011R: Nextion device model
99: Firmware version S99
61488: MCU code
D264B8204F0E1828: Nextion device serial number
16777216: FLASH Size in bytes

At this point, Protocol Reparse is in passive, but Address Mode may need handling. If Nextion address is 0, then the remainder of the instructions can be sent as normal. But if Nextion is in Address mode where the address is other than 0, then the remainder of the instructions will need to be sent with the two byte address prefixed to each instruction. In the above example connect string, the Nextion address is 2556, or hex 0x09FC. This would require the remaining instructions to be sent prefixed with the two bytes 0xFC 0x09.

# Step 3. Conditionally handling sleep and dim

It is best to handle any necessary steps needed to counter sleep functions or low dim at this this point. If your HMI project does not make use of low dim or sleep functions, you can skip this step and proceed to Step 4.

Using the get instruction, query the necessary system variables for your sleep functions and dim setting used and conditionally adjust them as desired to avoid any potential conflicts with the upload process. As these are user project dependent and all simple instructions from the Nextion Instruction Set, there is no need to cover in depth here.

# Step 4. Upload the firmware

Now that the serial port and current baud rate is known – send the upload command.

whmi-wri filesize,baud,res0ÿÿÿ
filesize: filesize of the tft file (byte)
baud: baud rate
res0: reserved, it can be filled with any ASCII character

The Nextion device will send a 0x05 byte within 500ms after receiving the whmi-wri instruction notifying that it is ready to begin receiving the .tft file contents. Users must wait until this 0x05 byte is received before sending or risk that the data sent before Nextion was ready will be deemed lost (and therefore a corrupted and unusable upload). Users must split the .tft file contents into 4096 byte sized packets with the final partial packet size equal to the last remaining bytes (<4096 bytes).

Users then sends out each packet, and in turn wait for the Nextion to return its 0x05 byte confirming reception and readiness to receive the next pack. Once the final partial packet has been sent and the confirmation 0x05 has been received, the upload portion is complete.

# Step 5. After the firmware upload

After the firmware is complete, Nextion will reset and make any necessary firmware adjustments internal to the Nextion device. It is important to wait for these procedures to complete, they are not instant, nor is their time to complete foreknown. As per the Nextion Instruction Set, Nextion will send an 0x88 Return Data notification indicating it is now ready. It is wise to wait for such notification before proceeding with your normal upgraded HMI processing.

# If Firmware Upload got Failed

if your firmware upload is failed and display is showing "System Data Error" then , the device baud rate will be chnaged to default baudrate which is 9600 , so if you try to connect with the previous baud rate you were using then the device will not respond , so you need to do the previous steps with 9600 baud rate and can change the baud rate at Step 4 .
