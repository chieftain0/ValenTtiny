# ValenTtiny

[Wokwi Demo](https://wokwi.com/projects/416622139343298561)

## Installation

### Prerequisites

#### Hardware

- [USBasp](https://www.fischl.de/usbasp/)
- [ValenTtiny PCB](https://github.com/chieftain0/ValenTtiny/tree/main/gerber)
- [ATTiny85](https://www.microchip.com/en-us/product/attiny85)

#### Software

- [Arduino IDE](https://www.arduino.cc/en/software)
- [ATTinyCore by SpenceKonde](https://github.com/SpenceKonde/ATTinyCore.git)
- [Git](https://git-scm.com/downloads) (optional)

### Steps

1. Install Arduino IDE and configure ATTinyCore by adding the following URL to the "Additional Board Manager URLs" in the IDE preferences:

    ```
    http://drazzy.com/package_drazzy.com_index.json
    ```

2. Load the ATTinyCore with your preferred bootloader (or without) onto the ATTiny85 using USBasp. Note: Micronucleus bootloader is preferred for this project but uses additional flash space.

3. Clone or download the ValenTtiny repository:

    ```bash
    git clone https://github.com/chieftain0/ValenTtiny.git
    ```

4. Load the ValenTtiny sketch onto the ATTiny85 using USBasp, or directly via onboard USB if Micronucleus was chosen.

5. (Optional but recommended) Find a Valentine to share your creation with.
