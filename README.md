# ValenTtiny

[Wokwi Demo](https://wokwi.com/projects/416622139343298561)

## Installation

### Prerequisites

- #### Hardware

  - [USBasp](https://www.fischl.de/usbasp/)
  - [ValenTtiny PCB](https://github.com/chieftain0/ValenTtiny/tree/main/gerber)
  - [ATtiny85](https://www.microchip.com/en-us/product/attiny85)

- #### Software

  - [Arduino IDE](https://www.arduino.cc/en/software)
  - [ATTinyCore by SpenceKonde](https://github.com/SpenceKonde/ATTinyCore.git)
  - [Git](https://git-scm.com/downloads) (optional)

### Steps

1. Install Arduino IDE and configure ATTinyCore by adding the following URL to the "Additional Board Manager URLs" in the IDE preferences:

    ```bash
    http://drazzy.com/package_drazzy.com_index.json
    ```

2. Load the ATTinyCore with your preferred bootloader (or without) onto the ATTiny85 using USBasp. Note: Micronucleus bootloader is preferred for this project but uses additional flash space.

3. Clone or download the ValenTtiny repository:

    ```bash
    git clone https://github.com/chieftain0/ValenTtiny.git
    ```

4. Install the required libraries using Arduino IDE's library manager.

5. Load the ValenTtiny sketch onto the ATTiny85 using USBasp, or directly via onboard USB if Micronucleus was chosen.

6. (Optional but recommended) Find a Valentine to share your creation with.

## Notes

- ### SMD and TH

  - Although the original ValenTtiny board consists mostly of through-hole components, the addressable LEDs in the original design are SMD to maintain a slim profile. In later versions, a through-hole 5 mm addressable LEDs option and almost all SMD component option have been included in the [gerber](https://github.com/chieftain0/ValenTtiny/tree/main/gerber) folder.

- ### EasyHID

  - [EasyHID](https://github.com/GyverLibs/EasyHID.git) is a great open-source library for HID emulation, to which I have contributed to ensure compatibility with ATtiny85 and ValenTtiny boards. However, the provided sketch may be too large to fit into the ATtiny85 when including HID functionality, so you may need to reduce some features. On the positive side, this allows you to use your ValenTtiny board as a rubber ducky.
  - To install EasyHID library, navigate to your libraries folder and clone the repository:

    ```bash
    git clone https://github.com/GyverLibs/EasyHID.git
    ```
