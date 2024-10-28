# Custom HID Device Setup for Blackpill

This guide walks you through configuring a custom HID device, recognized as a keyboard, for the Blackpill microcontroller.

### Prerequisites

Ensure you have:
- STM32CubeMX with the `joystick.ioc` configuration file for customization.
- Basic understanding of configuring microcontroller peripherals, especially for USB devices.

---

## Step 1: Clock Configuration

1. Open the `.ioc` file for your Blackpill project.
2. Configure clocks to ensure optimal speed. For USB, set the frequency to **48MHz**.
   - Recommended settings:
     - **RCC** -> Set **HSE** to Ceramic/Crystal Resonator.
     - **SYS** -> Enable **SW** or **Trace** as needed.
     - **Clock Config** -> Set **HCLK** to max, **USB** to 48MHz.

---

## Step 2: USB and HID Configuration

1. In the `.ioc` file, configure the USB and HID settings:
   - **Connectivity** -> Enable **USB_OTG_FS** with **Mode Device Only**.
   - **Middleware and Software** -> Set **USB_DEVICE** to **Custom HID Class**.
2. Configure HID report sizes:
   - **USBD_CUSTOM_HID_Report** -> Enter HID descriptor size (ideally a keyboard descriptor).
   - **USB_CUSTOM_OUT_REPORT** -> Set the size for incoming data (PC -> HID communication).

---

## Step 3: Modify Project Parameters

1. Edit the device descriptor:
   - Navigate to: `USB_DEVICE -> App -> usbd_custom_hid_if.c`
   - In the `CUSTOM_HID_ReportDesc_FS` function, insert the descriptor for your device.
2. Optionally, implement data transmission at the bottom of the file as shown:

   ```cpp
   // Example of data transmission function
   uint8_t sendData(uint8_t *data, uint16_t length) {
       return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, data, length);
   }
