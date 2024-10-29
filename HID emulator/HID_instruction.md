# Custom HID Device Setup for Blackpill

This guide walks you through configuring a custom HID device, recognized as a keyboard, for the Blackpill microcontroller.

### Prerequisites

Ensure you have:
- STM32CubeMX,
- Blackpill microcontroller with USB as HID funstions,
- In `joystick` file you can see decriptors for keyboard and functions to sending data. You can download the project to see if that's working on your computer.

---

## Step 1: Clock Configuration

1. Open the `.ioc` file for your Blackpill project.
2. Configure clocks to ensure optimal speed. For USB, set the frequency to **48MHz**.
   - Recommended settings:
     - **RCC** -> Set **HSE Ceramic/Crystal Resonator**.
     - **SYS** -> Enable **SW** or **Trace** as needed.
     - **Clock Config** -> Set **HCLK** to max, **USB** to 48MHz.

---

## Step 2: USB and HID Configuration

1. In the `.ioc` file, configure the USB and HID settings:
   - **Connectivity** -> Enable **USB_OTG_FS** with **Mode Device Only**.
   - **Middleware and Software** -> Set **USB_DEVICE** to **Custom HID Class**.
2. Configure HID report sizes:
   - **USBD_CUSTOM_HID_Report** -> Enter HID descriptor size (ideally a keyboard descriptor, you can see main descriptor in `joystick` project).
   - **USB_CUSTOM_OUT_REPORT** -> Set the size for incoming data (PC -> HID communication).

---

## Step 3: Modify Project Parameters

1. Edit the device descriptor:
   - Navigate to: `USB_DEVICE -> App -> usbd_custom_hid_if.c`
   - In the `CUSTOM_HID_ReportDesc_FS` function, insert the descriptor for your device.
2. All of that code you cann see in .pdf file

---

## Summary:

1. Example of sending command function:

```c++ 
uint8_t your_sending_HID_data_function (uint8_t modifier, uint8_t k1, uint8_t k2, uint8_t k3, uint8_t k4, uint8_t k5, uint8_t k6){
   uint8_t report [8] = {0};

   report[0] = modifier;
   report[1] = 0; // reserved_byte (release operation)
   report[2] = k1;
   report[3] = k2;
   report[4] = k3;
   report[5] = k4;
   report[6] = k5;
   report[7] = k6;

   return USBD_CUSTOM_HID_SendReport(&hUSBDeviceFS, report, sizeof(report));
}
```

2. Example of descriptor (for keyboard):

```c++
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END = {
    /* USER CODE BEGIN 0 */
    0x05, 0x01,    // Usage Page (Generic Desktop)
    0x09, 0x06,    // Usage (Keyboard)
    0xA1, 0x01,    // Collection (Application)
    0x05, 0x07,    // Usage Page (Key Codes)
    0x19, 0xE0,    // Usage Minimum (224) - Left Control
    0x29, 0xE7,    // Usage Maximum (231) - Right Control
    0x15, 0x00,    // Logical Minimum (0)
    0x25, 0x01,    // Logical Maximum (1)
    0x75, 0x01,    // Report Size (1)
    0x95, 0x08,    // Report Count (8)
    0x81, 0x02,    // Input (Data, Variable, Absolute)
    0x95, 0x01,    // Report Count (1)
    0x75, 0x08,    // Report Size (8)
    0x81, 0x03,    // Input (Constant)
    0x95, 0x06,    // Report Count (6)
    0x75, 0x08,    // Report Size (8)
    0x15, 0x00,    // Logical Minimum (0)
    0x25, 0x65,    // Logical Maximum (101)
    0x05, 0x07,    // Usage Page (Key Codes)
    0x19, 0x00,    // Usage Minimum (0)
    0x29, 0x65,    // Usage Maximum (101)
    0x81, 0x00,    // Input (Data, Array)
    /* USER CODE END 0 */
    0xC0           // END_COLLECTION
};
```

'''