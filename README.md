# Nu, pogodi! - microcontroller game

## Description
Nu, pogodi! is a simple implementation of [the Soviet LCD game](https://en.wikipedia.org/wiki/Well,_Just_You_Wait!#Video_games) for STM32. <br>
The game is written in C without any external libraries.

## In-game footage
Do not let eggs to drop! <br>
![In-game footage](./assets/game.gif)

## Prerequisites
* arm-none-eabi-gcc v13.2.0
* st-flash v1.3 
* STM32F0_discovery board
* I2C oled display
* 4 buttons to control
* seven-segment display (optional)

## Installation
* Clone the repository:
```sh
git clone https://github.com/NikitaDzer/NuPogodi.git
```
* Compile and load the game to microcontroller:
```sh
cd NuPogodi
make
make flash
```

## Usage
Сonnect the peripherals to the following pins:
* PB6-PB7: I2C display
* PC0-PC3: 4 buttons
* PA1-PA12: seven-segment

## License
[MIT](https://choosealicense.com/licenses/mit/)
