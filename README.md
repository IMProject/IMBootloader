### Glossary
MCU - microcontroller


### About
IMBootloader is a universal bootloader for embedded devices.

Idea is to save developers time for writing bootloader and flasher applications.
This bootloader works together with the flasher app [IMFlasher](https://github.com/IMProject/IMFlasher).

#### Application example
Each MCU has its own [IMLedBlink](https://github.com/IMProject/IMLedBlink) example. It is a good starting point for developing an application or to get an idea of how to jump from the application to IMBootloader.

#### Currently supported MCUs:

* STM32L4xx
* STM32H7xx
* STM32F7xx

#### Currently supported boards:

| Board                                                                      |MCU       |
| -------------------------------------------------------------------------- |:--------:|
| [Mateksys h743-slim](http://www.mateksys.com/?portfolio=h743-slim)         |STM32H7xx |
| [Pixhawk 4](https://docs.px4.io/master/en/flight_controller/pixhawk4.html) |STM32F7xx |

In the future, we will add as many as possible MCUs. 
