# SPI_Display_Test
Base project for ARMstrap Eagle (STM32F4x7) and U8Glib using SPI
* Includes U8G 1.16 [ARM]

## Using this project
1. Update `ports_and_pins.h` with your configuration. Out of the box, the project is set for an HD66753 display on SPI1 using some pins on PA.
2. In `main.cpp:setup()`, change the call to the initialization function of U8G to use the appropriate device function for your device.
3. Adjust the `main.cpp:draw(uint8_t)` function to draw within the boundaries of your screen.
4. Build and run.
