# GattAuth
Shows how to pair with GATT devices that requires authentication. The project uses **Bluetooth Framework**

## Problem description

This demo application was created as demostration for solving the problem described in this forum post: https://forum.btframework.com/index.php?topic=3875.0

Some Bluetooth LE GATT devices may close the connection in short time or simple rejects the connection. To avoid this issue you have to pair with target device. However, if you pair with the device **before** connection establishing you may get error during connection. However it is always possible to connectiont to such device without pairing.

## How to solve the issue

To solve this problem you have to connect to your device and only then pair with it. When device disconnects your application should unpair the device. In this case device will keep connection active as long as you need.

* Delphi samples is designed for Delphi/CBuilder (RAD Studio) 10
* C++ samples is designed for Visual Studio 2015
* .NET samples is designed for .NET Framework 4.0 and above and created in Visual Studio 2015

Required Bluetooth Framework **7.19.0.0** or above. You can download Bluetooth Framework [here](https://www.btframework.com/bluetoothframework.htm).

Should you have any questions please do not hesitate to contact us at support@btframework.com
