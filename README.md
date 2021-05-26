# GattAuth
Shows hwo to pair with GATT devices that requires authentication.

## Problem description

This demo application was created as demostration for solving the problem described in this forum post: https://forum.btframework.com/index.php?topic=3875.0

Some Bluetooth LE GATT devices may close the connection in short time or simple rejects the connection. To avoid this issue you have to pair with target device. However, if you pair with the device **before** connection establishing you may get error during connection. However it is always possible to connectiont to such device without pairing.

## How to solve the issue

To solve this problem you have to connect to your device and only then pair with it. When device disconnects your application should unpair the device. In this case device will keep connection active as long as you need.
