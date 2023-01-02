#### Glossary
MCU - microcontroller


#### About
IMBootloader is a universal bootloader for embedded devices created by [IMProject](https://github.com/IMProject) Development Team.

Idea is to save developers time for writing bootloader and flasher applications.
This bootloader works together with the platform agnostic flasher app [IMFlasher](https://github.com/IMProject/IMFlasher) created in QT and WEB page [https://imtech.hr](https://imtech.hr) where a manufacturer can store firmware and updated bootloader.

#### Support and Developers Channel

There's a dedicated Discord server here: https://discord.gg/kxwY8SkAjh

#### Firmware example
Each MCU has its own [IMLedBlink](https://github.com/IMProject/IMLedBlink) example. It is a good starting point for developing an firmware or to get an idea of how to jump from the firmware to IMBootloader.

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
Add drivers in the **Drivers** folder and inside the **Bootloader/Adapters** glue the drivers to the IMBootloader code.

#### Self-update feature
To use this feature make a RAM variant of IMBootlaoder (example: `make matek_H7_slim_ram`). Then flash it with [IMFlasher](https://github.com/IMProject/IMFlasher) application. Once when RAM variant is running, FLASH sectors for the Bootloader are free to be erased and rewritten.

![self-update](https://user-images.githubusercontent.com/10188706/194720114-d39ddc2a-a962-4396-94ca-5645fa6a8c5d.gif)

### Integration with [imtech.hr](https://imtech.hr)

#### Manufacturer id

Every user on the page gets its own generated 44 characters long base64 manufacturer id. This id is needed to separate different users from each other.
![image](https://user-images.githubusercontent.com/10188706/194776688-b1c06866-bd50-445e-a001-8f3e6e8a446a.png)

Generated manufacturer id should be changed here: \
https://github.com/IMProject/IMBootloader/blob/master/Bootloader/Inc/board_info.h#L58

#### Product types
Each manufacturer id can have multiple product types. This means the purpose of software on the same hardware can be different. It is also useful to identify different board versions (hardware variants) to keep track of what firmware and bootloader go where. 
![image](https://user-images.githubusercontent.com/10188706/194776847-4d568155-354f-41ae-9397-74830effa5cd.png)

Product type can be changed or added here:
https://github.com/IMProject/IMBootloader/blob/master/Bootloader/Inc/board_info.h#L60-L66

#### File versions
Each product type is then connected with a file version that represents different firmware or bootloaders. Each of these can be uploaded through the WEB interface. 
![image](https://user-images.githubusercontent.com/10188706/194776895-d88782be-5f49-492a-b0c4-c7b13c82480b.png)

#### Pre-shared key
This is the key that servers for key exchange. The key is never to send over communication. Its purpose is to verify newly generated keys from the bootloader and server side. Only bootloaders and servers know the key and only they can create secure communication. This insure protection against a man-in-the-middle attack. For full protection make sure your bootloader has enabled the read protection mechanism.
On the bootloader side, the pre-shared key is set here: \
https://github.com/IMProject/IMBootloader/blob/master/Bootloader/Inc/security.h#L59 \

On the server side, it is set at WEB in the product type section. 

