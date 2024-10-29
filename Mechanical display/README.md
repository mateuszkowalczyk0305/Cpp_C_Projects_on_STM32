# Mechanical Display Project Composed of an Undefined Number of Modules:
- Instructions for adding modules are available in the `Mechanical_Display_Datasheet.pdf` file.
- Each module contains a stepper motor and a Hall sensor.
- The Hall sensor is used to detect the zero position of every display's module.
- The stepper motor has a shaft attached to it, where specific graphics can be printed.
- The number of segments/graphics can be flexibly adjusted when defining an instance of each module.
- The motor is controlled via data received through the UART interface.
- A list of commands for controlling the display, as well as its modules and segments, is available in `Mechanical_Display_Datasheet.pdf`.
