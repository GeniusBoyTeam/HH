![Handheld Controller](./docs/HH.png)
# GRBL ESP32 Handheld Controller

This repository contains the source code for a handheld device designed to interface with GRBL ESP32 via a serial port. The handheld allows you to control your CNC machine, similar to the functionality provided by the GRBL web server and desktop software like GRBL Plotter. 

## Features

- **Machine Control:** Use the handheld to control your CNC machine, sending commands directly to GRBL via the serial port.
- **File Execution from SD Card:** Load and execute G-code files directly from an SD card.
- **Spindle Control:** Control the spindle speed and other parameters directly from the handheld.
- **User Interface:** Intuitive interface for easy navigation and machine control.

## Compatibility

This code is compatible with all versions of GRBL, providing a versatile solution for CNC machine control. The device has been tested with multiple GRBL versions, ensuring broad compatibility.

## Getting Started

### Hardware Requirements

- ESP32 microcontroller
- SD card module
- CNC machine with GRBL firmware
- Custom PCB (design available in a separate repository)

### Software Setup

1. Clone this repository to your local machine.
2. Open the project in your preferred development environment.
3. Flash the ESP32 with the provided firmware.
4. Connect the handheld to your CNC machine via the serial port.
5. Insert an SD card with G-code files for execution.

### Usage

- Power on the handheld and connect it to your CNC machine.
- Use the navigation buttons to control the machine and access different functions.
- Load files from the SD card and execute them directly.
- Adjust spindle speed and other parameters through the handheld interface.


## Contributing

Contributions to this project are welcome. Please feel free to submit issues, feature requests, or pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](#) file for details.
