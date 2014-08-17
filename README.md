# SPI_Display_Test
Base project for ARMstrap Eagle (STM32F4x7) and U8Glib using SPI
## Using this project
1. Update `ports_and_pins.h` with your configuration. The example uses SPI2 with the following pins:
 * PB10 - SCK
 * PB11 - Data/Command 
 * PB12 - Chip Select (active low)
 * PB13 - Reset (active low)
 * PB14 - MISO
 * PB15 - MOSI
2. In `main.cpp:setup()`, change the call to the initialization function of U8G to use the appropriate device function for your device.
3. Adjust the `main.cpp:draw(uint8_t)` function to draw within the boundaries of your screen.
4. Build and run.
