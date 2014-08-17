/*
 * ports_and_pins.h
 *
 *  Created on: Aug 16, 2014
 *      Author: aaron
 */

#ifndef PORTS_AND_PINS_H_
#define PORTS_AND_PINS_H_

/* SPI settings */
#define SPIx                           SPI2
#define SPIx_CLK                       RCC_APB1Periph_SPI2
#define SPIx_CLK_INIT                  RCC_APB1PeriphClockCmd
#define SPIx_IRQn                      SPI2_IRQn
#define SPIx_IRQHANDLER                SPI2_IRQHandler

#define SPIx_SCK_PIN                   GPIO_Pin_10
#define SPIx_SCK_GPIO_PORT             GPIOB
#define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define SPIx_SCK_SOURCE                GPIO_PinSource10
#define SPIx_SCK_AF                    GPIO_AF_SPI2

#define SPIx_MISO_PIN                  GPIO_Pin_14
#define SPIx_MISO_GPIO_PORT            GPIOB
#define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define SPIx_MISO_SOURCE               GPIO_PinSource14
#define SPIx_MISO_AF                   GPIO_AF_SPI2

#define SPIx_MOSI_PIN                  GPIO_Pin_15
#define SPIx_MOSI_GPIO_PORT            GPIOB
#define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define SPIx_MOSI_SOURCE               GPIO_PinSource15
#define SPIx_MOSI_AF                   GPIO_AF_SPI2

#define SPIx_SCR_CS_PIN                GPIO_Pin_12
#define SPIx_SCR_CS_GPIO_PORT          GPIOB
#define SPIx_SCR_CS_GPIO_CLK           RCC_AHB1Periph_GPIOB
#define SPIx_SCR_CS_SOURCE             GPIO_PinSource12

#define SPIx_SCR_RS_PIN                GPIO_Pin_13
#define SPIx_SCR_RS_GPIO_PORT          GPIOB
#define SPIx_SCR_RS_GPIO_CLK           RCC_AHB1Periph_GPIOB
#define SPIx_SCR_RS_SOURCE               GPIO_PinSource13

#define SPIx_SCR_DC_PIN                GPIO_Pin_11
#define SPIx_SCR_DC_GPIO_PORT          GPIOB
#define SPIx_SCR_DC_GPIO_CLK           RCC_AHB1Periph_GPIOB
#define SPIx_SCR_DC_SOURCE             GPIO_PinSource11



#endif /* PORTS_AND_PINS_H_ */
