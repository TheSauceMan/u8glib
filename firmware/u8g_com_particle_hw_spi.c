/*
  
  u8g_com_particle_hw_spi.c

  Universal 8bit Graphics Library
  
  Copyright (c) 2015, sosman@terratron.com
  Copyright (c) 2012, olikraus@gmail.com
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
  
  Note on Particle Photon A5 is MOSI and A3 is CLK.  (A4 is MISO)
*/

#include "u8g.h"



#if defined(PARTICLE)

#include "spi_hal.h"

uint8_t u8g_com_particle_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
  case U8G_COM_MSG_STOP:
    HAL_SPI_End(HAL_SPI_INTERFACE1);
    break;
    
  case U8G_COM_MSG_INIT:
    HAL_SPI_Init(HAL_SPI_INTERFACE1);
    HAL_SPI_Set_Clock_Divider(HAL_SPI_INTERFACE1, 0);
    HAL_SPI_Begin(HAL_SPI_INTERFACE1, u8g->pin_list[U8G_PI_CS]);
    u8g_SetPIOutput(u8g, U8G_PI_A0);
    u8g_SetPIOutput(u8g, U8G_PI_RESET);
    
    break;
    
  case U8G_COM_MSG_ADDRESS:
    u8g_SetPILevel(u8g, U8G_PI_A0, arg_val);
      break;
      
  case U8G_COM_MSG_CHIP_SELECT:
    if (arg_val == 0)
      {
	u8g_SetPILevel(u8g, U8G_PI_CS, 1);
      }
    else
      {
	u8g_SetPILevel(u8g, U8G_PI_CS, 0);
      }
    break;
    
  case U8G_COM_MSG_RESET:
    u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
    break;
    
  case U8G_COM_MSG_WRITE_BYTE:
    HAL_SPI_Send_Receive_Data(HAL_SPI_INTERFACE1, arg_val);
    break;
    
  case U8G_COM_MSG_WRITE_SEQ:
    for (register uint8_t *ptr = arg_ptr; arg_val > 0; arg_val--, ptr++)
      {
	HAL_SPI_Send_Receive_Data(HAL_SPI_INTERFACE1, *ptr);
      }

    break;
    
  case U8G_COM_MSG_WRITE_SEQ_P:
    for (register uint8_t *ptr = arg_ptr; arg_val > 0; arg_val--, ptr++)
      {
	HAL_SPI_Send_Receive_Data(HAL_SPI_INTERFACE1, u8g_pgm_read(ptr));
      }
    break;
  }
  return 1;
}

#else

uint8_t u8g_com_particle_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif


