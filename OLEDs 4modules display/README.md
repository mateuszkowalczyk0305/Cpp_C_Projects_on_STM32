# Display Project Composed of 4 OLED Displays Based on the SSD1306 Controller

- The project is designed to interpret data sent via the UART interface and execute specific actions using commands available in the `commands.pdf` file (all details are in `commands.pdf`).

- The display consists of 4 independently operating OLED screens. Each display’s connection is separate, due to the way the library was implemented, originally in C. 

- The displays are controlled via the SPI interface.

- Display functionalities are available in `commands.pdf`.
