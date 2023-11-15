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
