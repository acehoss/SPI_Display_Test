/*
 * ports_and_pins.h
 *
 *  Created on: Aug 16, 2014
 *      Author: aaron
 */

#ifndef PORTS_AND_PINS_H_
#define PORTS_AND_PINS_H_

/* SPI settings */
#define SPIx                           SPI1
#define SPIx_CLK                       RCC_APB2Periph_SPI1
#define SPIx_CLK_INIT                  RCC_APB2PeriphClockCmd
#define SPIx_IRQn                      SPI2_IRQn
#define SPIx_IRQHANDLER                SPI2_IRQHandler

#define SPIx_SCK_PIN                   GPIO_Pin_5
#define SPIx_SCK_GPIO_PORT             GPIOA
#define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define SPIx_SCK_SOURCE                GPIO_PinSource5
#define SPIx_SCK_AF                    GPIO_AF_SPI1

#define SPIx_MISO_PIN                  GPIO_Pin_6
#define SPIx_MISO_GPIO_PORT            GPIOA
#define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define SPIx_MISO_SOURCE               GPIO_PinSource6
#define SPIx_MISO_AF                   GPIO_AF_SPI1

#define SPIx_MOSI_PIN                  GPIO_Pin_7
#define SPIx_MOSI_GPIO_PORT            GPIOA
#define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define SPIx_MOSI_SOURCE               GPIO_PinSource7
#define SPIx_MOSI_AF                   GPIO_AF_SPI1

#define SPIx_SCR_CS_PIN                GPIO_Pin_4
#define SPIx_SCR_CS_GPIO_PORT          GPIOA
#define SPIx_SCR_CS_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define SPIx_SCR_CS_SOURCE             GPIO_PinSource4

#define SPIx_SCR_RS_PIN                GPIO_Pin_1
#define SPIx_SCR_RS_GPIO_PORT          GPIOA
#define SPIx_SCR_RS_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define SPIx_SCR_RS_SOURCE             GPIO_PinSource1

#define SPIx_SCR_DC_PIN                GPIO_Pin_2
#define SPIx_SCR_DC_GPIO_PORT          GPIOA
#define SPIx_SCR_DC_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define SPIx_SCR_DC_SOURCE             GPIO_PinSource2



#endif /* PORTS_AND_PINS_H_ */
