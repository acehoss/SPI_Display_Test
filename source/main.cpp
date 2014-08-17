/*
The MIT License (MIT)

Copyright (c) 2014 ARMstrap Community

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */
#include "main.h"

static uint32_t gSecondCounter = 0;
static volatile uint32_t gFrameCounter = 0;
static volatile uint32_t gLastFrameCount = 0;
static volatile BitAction gUserButtonState = Bit_RESET;
static enum sysclk_freq gCurrentSysclkFreq;

static char gFrameCountStr[20];
static char gPageCountStr[30];

SPI_InitTypeDef  SPI_InitStructure;
u8g_t u8g;

int main()
{
	setup();
	do
	{
		loop();
	}
	while (1);

	return 0; // never reached
}

void setup()
{
	/* Set up the initial clock speed to 168 MHz, enable clocks, and enable systick */
	set_sysclock(SYSCLK_168_MHZ);

	/* Basic setup */
	initialize_board();
	initialize_systick_interrupt();

	/* Set up SPI ports and pins */
	SPI_Config();

	/* U8G initialization - substitute second parameter with device driver for your screen */
	u8g_InitComFn(&u8g, &u8g_dev_ssd1325_nhd27oled_2x_gr_hw_spi, u8g_com_hw_spi_fn);
}

void loop()
{
	/* last known status of user button - to detect changes in button state */
	static BitAction userButtonState = Bit_RESET;
	/* counter for display animation */
	static uint8_t pos = 0;
	/* holds the number of frames between loops */
	static uint8_t pageCount = 0;
	/* counter for pages */
	uint8_t pageCounter = 0;
	/* temp variable for building a display string */
	char pageCountTempStr[10];

	/* build string for pages per frame and MPU speed */
	itoa(pageCount, pageCountTempStr, 10);
	strcat(pageCountTempStr, " ppf @ ");
	strcpy(gPageCountStr, pageCountTempStr);
	itoa(SystemCoreClock/1000000, pageCountTempStr, 10);
	strcat(pageCountTempStr, " MHz");
	strcat(gPageCountStr, pageCountTempStr);

	/* build string for frames per second */
	itoa(gLastFrameCount, gFrameCountStr, 10);
	strcat(gFrameCountStr, " fps");

	/* check user button */
	if(gUserButtonState != userButtonState)
	{
		/* Button transition occurred. On rising edge, cycle to the next sysclk frequency. */
		if(gUserButtonState == Bit_SET)
		{
			set_sysclock((enum sysclk_freq)(uint8_t)((gCurrentSysclkFreq+1)%5));

		}
		userButtonState = gUserButtonState;
	}

	/* Picture Loop */
	/* To conserve memory U8G renders the screen in pages.
	 * For each page, the entire screen is rendered but clipped to the bounds of
	 * the page. In the case of the NHD-2.7-12864 display, there are either 8 or
	 * 16 pages depending on the U8G device constructor used. U8G does not
	 * support single-page rendering out of the box, but it can with a few
	 * relatively simple tweaks. This yields a noticable performance gain. */
	u8g_FirstPage(&u8g);
	do
	{
		draw(pos/(gCurrentSysclkFreq+1));
		pageCounter++;
	} while ( u8g_NextPage(&u8g) );

	/* manage counters */
	pageCount = pageCounter;
	gFrameCounter++;
	pos = (pos+1) % (4*(gCurrentSysclkFreq+1));
}


void draw(uint8_t pos)
{
	/* Draw the pages per frame string */
	u8g_SetColorIndex(&u8g, 2);
	u8g_SetFont(&u8g, u8g_font_6x10);
	u8g_DrawStr(&u8g,  0, 10, gPageCountStr);

	/* Draw the frames per second string */
	u8g_SetColorIndex(&u8g, 3);
	u8g_SetFont(&u8g, u8g_font_timR12);
	u8g_DrawStr(&u8g,  0, 30, gFrameCountStr);

	/* Draw the spinner */
	u8g_SetColorIndex(&u8g, 1);
	u8g_SetFont(&u8g, u8g_font_6x10);
	u8g_DrawStr(&u8g, 62, 52, pos == 0 ? "|" : pos == 1 ? "/" : pos == 2 ? "-" : "\\");
}

void initialize_board()
{
	// For the ARMstrap Eagle board, the USER LED is hooked-up to GPIOC Pin 1.
	// You can see this by looking at the freely available schematics on armstrap.org
	GPIO_InitTypeDef boardGPIO;
	GPIO_StructInit(&boardGPIO);
	boardGPIO.GPIO_Mode = GPIO_Mode_OUT;
	boardGPIO.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &boardGPIO);

	// USER BUTTON is on GPIOC Pin 0. It has an external pull down resistor.
	boardGPIO.GPIO_Mode = GPIO_Mode_IN;
	boardGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	boardGPIO.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC, &boardGPIO);
}

void enable_clocks()
{
	/* NOTE: enable all clocks here, master clock speed change requires ability to reenable clocks */

	/* GPIOC: port connected to user LED, user button, and external flash. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* SPI peripheral clock, using SPI device set in main.h */
	SPIx_CLK_INIT(SPIx_CLK, ENABLE);

	/* GPIO clocks for pins used by SPI device(s) */
	RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK | SPIx_SCR_CS_GPIO_CLK | SPIx_SCR_RS_GPIO_CLK | SPIx_SCR_DC_GPIO_CLK, ENABLE);
}

void SPI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* NOTE: assumes clocks have already been enabled */

	/* SPI GPIO Configuration */
	/* Connect SPI pins to SPI AF */
	GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
	GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);
	GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
	GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

	/* SPI  MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin =  SPIx_MISO_PIN;
	GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);

	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
	GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	/* Screen CS pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPIx_SCR_CS_PIN;
	GPIO_Init(SPIx_SCR_CS_GPIO_PORT, &GPIO_InitStructure);

	/* Screen reset pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPIx_SCR_RS_PIN;
	GPIO_Init(SPIx_SCR_RS_GPIO_PORT, &GPIO_InitStructure);

	/* Screen data/command pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPIx_SCR_DC_PIN;
	GPIO_Init(SPIx_SCR_DC_GPIO_PORT, &GPIO_InitStructure);


	/* SPI configuration */
	SPI_I2S_DeInit(SPI2);
	SPI_InitStructure.SPI_Direction = SPI_Direction_Tx;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

	/* Apply SPI configuration */
	SPI_Init(SPIx, &SPI_InitStructure);

	/* Screen control lines initial state */
	GPIO_SetBits(SPIx_SCR_CS_GPIO_PORT, SPIx_SCR_CS_PIN);
	GPIO_ResetBits(SPIx_SCR_CS_GPIO_PORT, SPIx_SCR_RS_PIN);
	GPIO_SetBits(SPIx_SCR_CS_GPIO_PORT, SPIx_SCR_DC_PIN);

	/* Now that everything is configured, activate the peripheral */
	SPI_Cmd(SPIx, ENABLE);
}

void initialize_systick_interrupt()
{
	SysTick_Config(SYSTICK_CYCLES_PER_TICK);
	NVIC_SetPriority(SysTick_IRQn, 1);
}

void set_sysclock(enum sysclk_freq freq)
{
	rcc_set_frequency(freq);
	gCurrentSysclkFreq = freq;
	enable_clocks();
	initialize_systick_interrupt();
}

extern "C" {
void SysTick_Handler()
{
	/* counter for debouncing user button */
	static uint32_t buttonDebounceCounter = 0;

	/* One-second timer for frame counting */
	gSecondCounter++;
	if(gSecondCounter >= TICKS_PER_SECOND)
	{
		gSecondCounter = 0;
		gLastFrameCount = gFrameCounter;
		gFrameCounter = 0;

		/* Use one-second timer to toggle heartbeat LED as well */
		GPIO_ToggleBits(GPIOC, GPIO_Pin_1);
	}


	/* User button debounce */
	uint8_t pc0State = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0);
	if(pc0State == Bit_SET)
	{
		if(buttonDebounceCounter >= DEBOUNCE_TICKS)
			gUserButtonState = Bit_SET;
		else
			buttonDebounceCounter++;
	}
	else
	{
		if(buttonDebounceCounter == 0)
			gUserButtonState = Bit_RESET;
		else
			buttonDebounceCounter--;
	}
} }

