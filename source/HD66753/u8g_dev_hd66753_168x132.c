/*

  u8g_dev_hd66753_168x132.c

  Portions from U8Glib

  Universal 8bit Graphics Library
  
  Copyright (c) 2011, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
  
*/

#include "../U8G/u8g.h"

#define WIDTH 168l
#define HEIGHT 132l
#define BPP 2l
#define PIXELS_PER_BLOCK 8l
#define MEM_SIZE (WIDTH * HEIGHT / BPP)
#define BLOCK_MEM_SIZE (PIXELS_PER_BLOCK * BPP / 8l)
#define WIDTH_BLOCKS (WIDTH / PIXELS_PER_BLOCK)

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08


//static unsigned char Read_Block_Address_Macro[] = {0x74, 0x00, 0x12, 0x77, 0x00, 0x00};
static unsigned char Draw_Block_Value_Macro[] = {0x74, 0x00, 0x12, 0x76, 0xFF, 0xFF};
static unsigned char Draw_Block_Address_Macro[] = {0x74, 0x00, 0x11, 0x76, 0x00, 0x00};


static uint8_t LcdInitMacro[] = {

  0x74, 0x00, 0x00, 0x76, 0x00, 0x01,            // R00 start oscillation
  0x74, 0x00, 0x01, 0x76, 0x00, 0x0D,            // R01 driver output control
  0x74, 0x00, 0x02, 0x76, 0x00, 0x4C,            // R02 LCD - driving waveform control
  0x74, 0x00, 0x03, 0x76, 0x12, 0x14,            // R03 Power control
  0x74, 0x00, 0x04, 0x76, 0x04, 0x66,            // R04 Contrast control
  0x74, 0x00, 0x05, 0x76, 0x00, 0x10,            // R05 Entry mode
  0x74, 0x00, 0x06, 0x76, 0x00, 0x00,            // R06 Rotation
  0x74, 0x00, 0x07, 0x76, 0x00, 0x15,            // R07 Display control
  0x74, 0x00, 0x08, 0x76, 0x00, 0x03,            // R08 Cursor Control
  0x74, 0x00, 0x09, 0x76, 0x00, 0x00,            // R09 NOOP
  0x74, 0x00, 0x0A, 0x76, 0x00, 0x00,            // R0A NOOP
  0x74, 0x00, 0x0B, 0x76, 0x00, 0x03,            // R0B Horizontal Cursor Position
  0x74, 0x00, 0x0C, 0x76, 0x00, 0x03,            // R0C Vertical Cursor Position
  0x74, 0x00, 0x0D, 0x76, 0x00, 0x00,            // R0D 1st Driving Position
  0x74, 0x00, 0x0E, 0x76, 0x00, 0x15,            // R0E 2nd Driving Position
  0x74, 0x00, 0x0F, 0x76, 0x00, 0x03,            // R0F
  0x74, 0x00, 0x10, 0x76, 0x00, 0x00,            // R10 RAM write mask
  0x74, 0x00, 0x11, 0x76, 0x00, 0x00,            // R11 RAM Address
};


static void LcdWriteCommand(u8g_t* u8g, u8g_dev_t* dev, uint8_t* cmd)
{
    u8g_SetChipSelect(u8g, dev, 1);
    u8g_WriteByte(u8g,dev, cmd[0]);
    u8g_WriteByte(u8g,dev, cmd[1]);
    u8g_WriteByte(u8g,dev, cmd[2]);
//    u8g_WriteSequence(u8g, dev, 3, cmd);
    u8g_SetChipSelect(u8g, dev, 0);
    //u8g_Delay(10);
    u8g_SetChipSelect(u8g,dev,1);
    u8g_WriteByte(u8g,dev, cmd[3]);
    u8g_WriteByte(u8g,dev, cmd[4]);
    u8g_WriteByte(u8g,dev, cmd[5]);
//    u8g_WriteSequence(u8g,dev,3, &cmd[3]);
    u8g_SetChipSelect(u8g,dev,0);
    //u8g_Delay(10);
}

static void LcdSetAddress(u8g_t* u8g, u8g_dev_t* dev, uint16_t Address)
{
    Draw_Block_Address_Macro[4] = Address >> 8;
    Draw_Block_Address_Macro[5] = Address & 0xFF;
    LcdWriteCommand(u8g, dev, Draw_Block_Address_Macro);
}

static void LcdDrawBlock(u8g_t* u8g, u8g_dev_t* dev, uint8_t row, uint8_t block, uint8_t* buf)
{
	if(block == 0)
	{
		uint16_t addr = (uint16_t)row * (uint16_t)0x0020 + (uint16_t)block;
		LcdSetAddress(u8g, dev, addr);
	}

	uint16_t block_index = (row*WIDTH_BLOCKS*BLOCK_MEM_SIZE) + block*BLOCK_MEM_SIZE;
    Draw_Block_Value_Macro[4] = buf[block_index+1];
    Draw_Block_Value_Macro[5] = buf[block_index];

    LcdWriteCommand(u8g, dev, Draw_Block_Value_Macro);
}

void LcdSetContrast(u8g_t* u8g, u8g_dev_t* dev, uint8_t ContrastLevel)
{
    if (ContrastLevel > 127) ContrastLevel = 127;
    if (ContrastLevel < 70) ContrastLevel = 70;
    LcdInitMacro[0x04 * 6 + 5] = ContrastLevel;
    LcdWriteCommand(u8g, dev, &LcdInitMacro[0x04 * 6]);
}

uint8_t u8g_dev_hd66753_168x132_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_NONE);

      //u8g_WriteEscSeqP(u8g, dev, u8g_dev_hd66753_168x132_init_seq0);

      u8g_SetResetHigh(u8g,dev);
      u8g_Delay(20);
      u8g_SetResetLow(u8g, dev);
      u8g_Delay(20);
      u8g_SetResetHigh(u8g,dev);
      u8g_Delay(100);

      LcdWriteCommand(u8g, dev, LcdInitMacro);         // R00 start oscillation

      // Wait a minimum of 25ms after issuing "start oscillation"
      // command (to accomodate for MCLK up to 25MHz)
      u8g_Delay(100);

      LcdInitMacro[3 * 6 + 5] |= BIT3;
      LcdInitMacro[3 * 6 + 5] &= ~BIT0;
      LcdWriteCommand(u8g, dev, &LcdInitMacro[3 * 6]);

      //u8g_WriteEscSeqP(u8g, dev, u8g_dev_hd66753_168x132_init_seq1);

      // LCD Initialization Routine Using Predefined Macros
      LcdWriteCommand(u8g, dev, &LcdInitMacro[1 * 6]);
      LcdWriteCommand(u8g, dev, &LcdInitMacro[2 * 6]);
      LcdWriteCommand(u8g, dev, &LcdInitMacro[4 * 6]);
      LcdWriteCommand(u8g, dev, &LcdInitMacro[5 * 6]);
      LcdWriteCommand(u8g, dev, &LcdInitMacro[6 * 6]);
      LcdWriteCommand(u8g, dev, &LcdInitMacro[7 * 6]);
      //LcdWriteCommand(u8g, dev, &LcdInitMacro[8 * 6]);
      //LcdWriteCommand(u8g, dev, &LcdInitMacro[11 * 6]);
      //LcdWriteCommand(u8g, dev, &LcdInitMacro[12 * 6]);
      //LcdWriteCommand(u8g, dev, &LcdInitMacro[13 * 6]);
      //LcdWriteCommand(u8g, dev, &LcdInitMacro[14 * 6]);
      LcdWriteCommand(u8g, dev, &LcdInitMacro[16 * 6]);
      LcdWriteCommand(u8g, dev, &LcdInitMacro[17 * 6]);

      //u8g_WriteEscSeqP(u8g, dev, u8g_dev_hd66753_168x132_init_seq2);


      LcdSetContrast(u8g, dev, 110);


      //LcdSetAddress(u8g, dev, 0);
      //LcdWriteCommand(u8g,dev,Draw_Block_Value_Macro);

      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        uint8_t row_after_last_row, row, block;
        uint8_t *ptr;
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

        row_after_last_row = pb->p.page_y1;
        ptr = pb->buf;



        LcdSetAddress(u8g, dev, 0);

        u8g_SetChipSelect(u8g, dev, 1);
        u8g_WriteByte(u8g, dev, Draw_Block_Value_Macro[0]);
        u8g_WriteByte(u8g, dev, Draw_Block_Value_Macro[1]);
        u8g_WriteByte(u8g, dev, Draw_Block_Value_Macro[2]);
        u8g_SetChipSelect(u8g, dev, 0);

        u8g_SetChipSelect(u8g,dev,1);
        u8g_WriteByte(u8g, dev, Draw_Block_Value_Macro[3]);

        for( row = pb->p.page_y0; row < row_after_last_row; row++)
        {
        	uint16_t row_offset = row*WIDTH_BLOCKS*BLOCK_MEM_SIZE;


            //I didn't see this in the data sheet, but evidently you can keep writing blocks to the display (docs appear to say only one at a time)
        	//as a result, the set up to write happens before the row loop, and the entire screen is sent down the wire.
            for( block = 0; block < WIDTH_BLOCKS; block++ )
            {
            	uint16_t block_offset = row_offset+block*BLOCK_MEM_SIZE;
            	//block bytes must be reversed before sent out.
            	u8g_WriteByte(u8g, dev, ptr[block_offset+1]);
            	u8g_WriteByte(u8g, dev, ptr[block_offset]);
            }


        }
        u8g_SetChipSelect(u8g,dev,0);

      }
      break;
  }
  return u8g_dev_pbxh2_base_fn(u8g, dev, msg, arg);
}


//U8G_PB_DEV(u8g_dev_hd66753_168x132_sw_spi, WIDTH, HEIGHT, HEIGHT, u8g_dev_hd66753_168x132_fn, U8G_COM_SW_SPI);
uint8_t u8g_dev_hd66753_168x132_sw_spi_buf[MEM_SIZE] U8G_NOCOMMON;
u8g_pb_t u8g_dev_hd66753_168x132_sw_spi_pb = { {HEIGHT, HEIGHT, 0, HEIGHT, 0},  WIDTH, u8g_dev_hd66753_168x132_sw_spi_buf};
u8g_dev_t u8g_dev_hd66753_168x132_sw_spi = { u8g_dev_hd66753_168x132_fn, &u8g_dev_hd66753_168x132_sw_spi_pb, U8G_COM_SW_SPI };

//U8G_PB_DEV(u8g_dev_hd66753_168x132_hw_spi, WIDTH, HEIGHT, HEIGHT, u8g_dev_hd66753_168x132_fn, U8G_COM_HW_SPI);
uint8_t u8g_dev_hd66753_168x132_hw_spi_buf[MEM_SIZE] U8G_NOCOMMON ;
u8g_pb_t u8g_dev_hd66753_168x132_hw_spi_pb = { {HEIGHT, HEIGHT, 0, HEIGHT, 0},  WIDTH, u8g_dev_hd66753_168x132_hw_spi_buf};
u8g_dev_t u8g_dev_hd66753_168x132_hw_spi = { u8g_dev_hd66753_168x132_fn, &u8g_dev_hd66753_168x132_hw_spi_pb, U8G_COM_HW_SPI };
