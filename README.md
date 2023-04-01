#### Glossary
MCU - microcontroller


#### About
IMBootloader is a universal bootloader for embedded devices created by the [IMProject](https://github.com/IMProject) Development Team .

The idea behind this project is to save developers time by eliminating the need to write separate bootloader and flasher applications. IMBootloader works in tandem with the platform-agnostic flasher app, [IMFlasher](https://github.com/IMProject/IMFlasher) , which is created using QT. Additionally, manufacturers can store firmware and updated bootloader on the webpage https://imtech.hr.

#### Support and Developers Channel

A dedicated Discord server is available at https://discord.gg/kxwY8SkAjh

#### Firmware example
Each MCU has its own example of [IMLedBlink](https://github.com/IMProject/IMLedBlink). This example provides a good starting point for firmware development or understanding how to transition from firmware to IMBootloader.

#### Currently supported MCUs

* STM32L4xx
* STM32H7xx
* STM32F7xx

#### Currently supported boards

| Board                                                                      |MCU       |
| -------------------------------------------------------------------------- |:--------:|
| [Mateksys h743-slim](http://www.mateksys.com/?portfolio=h743-slim)         |STM32H7xx |
| [Pixhawk 4](https://docs.px4.io/master/en/flight_controller/pixhawk4.html) |STM32F7xx |

#### How to add a new MCU?
To add drivers, place them in the **Drivers** folder. Then, inside the **Bootloader/Adapters** folder, integrate the drivers with the IMBootloader code.

#### Self-update feature
To use this feature, create a RAM variant of IMBootloader (e.g., `make matek_H7_slim_ram`) and flash it using the [IMFlasher](https://github.com/IMProject/IMFlasher) application. Once the RAM variant is running, the FLASH sectors for the bootloader can be freely erased and rewritten.

![self-update](https://user-images.githubusercontent.com/10188706/194720114-d39ddc2a-a962-4396-94ca-5645fa6a8c5d.gif)

### Integration with [imtech.hr](https://imtech.hr)

#### Manufacturer id

Each user on the page receives a unique base64 manufacturer ID, which is 44 characters long. This ID is necessary to distinguish one user from another.
![image](https://user-images.githubusercontent.com/10188706/194776688-b1c06866-bd50-445e-a001-8f3e6e8a446a.png)

Generated manufacturer id should be changed here: \
https://github.com/IMProject/IMBootloader/blob/master/Bootloader/Inc/board_info.h#L58

#### Product types
Each manufacturer ID can have multiple product types, which means that software on the same hardware can serve different purposes. Additionally, it is useful for identifying different board versions (hardware variants) and keeping track of which firmware and bootloader belong to each.
![image](https://user-images.githubusercontent.com/10188706/194776847-4d568155-354f-41ae-9397-74830effa5cd.png)

Product type can be changed or added here:
https://github.com/IMProject/IMBootloader/blob/master/Bootloader/Inc/board_info.h#L60-L66

#### File versions
Each product type is then linked to a file version that represents different firmware or bootloaders. Each of these can be uploaded through the web interface.
![image](https://user-images.githubusercontent.com/10188706/194776895-d88782be-5f49-492a-b0c4-c7b13c82480b.png)

#### Pre-shared key
This is the key that is used for key exchange. The key should never be sent over communication channels. Its purpose is to verify newly generated keys from both the bootloader and the server side. Only the bootloaders and servers know the key, and only they can create a secure communication channel. This ensures protection against man-in-the-middle attacks. For full protection, make sure your bootloader has enabled the read protection mechanism. On the bootloader side, the pre-shared key can be set here: \
https://github.com/IMProject/IMBootloader/blob/master/Bootloader/Inc/security.h#L59 \

On the server side, it can be set in the product type section of the website. 

