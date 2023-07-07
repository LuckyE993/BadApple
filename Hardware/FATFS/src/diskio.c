/*!
    \file    diskio.c
    \brief   Low level disk interface driver

    \version 2021-05-20, V1.0.0, firmware for GD32 USBFS&USBHS
*/

/*
    Copyright (c) 2021, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

/* Includes ------------------------------------------------------------------*/
#include "diskio.h"
#include "ff_gen_drv.h"
#include "sdcard.h"
#include "bsp_usart.h"

#define ATA 0
#define SPI_FLASH 1

#define BLOCKSIZE 512
#define BUSMODE_4BIT


#if defined ( __GNUC__ )
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#endif

extern sd_card_info_struct sd_cardinfo;
extern Disk_drvTypeDef  disk;

/*!
    \brief      Gets Disk Status
    \param[in]  pdrv: Physical drive number (0..)
    \param[out] none
    \retval     DSTATUS: Operation status
*/
DSTATUS disk_status (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    DSTATUS stat = STA_NOINIT;

    switch(pdrv)
	{
		case ATA:
			stat = RES_OK;
			break;
		case SPI_FLASH:
			stat = STA_NOINIT;
	}

    return stat;
}


/*!
    \brief      Initializes a Drive
    \param[in]  pdrv: Physical drive number (0..)
    \param[out] none
    \retval     DSTATUS: Operation status
*/
DSTATUS disk_initialize (
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat = STA_NOINIT;
	uint32_t cardstate = 0;
	
    switch(pdrv)
	{
		case ATA:
			printf("SD_Init...\r\n");
			stat = sd_init();
			if(SD_OK == stat)
			{
				stat = sd_card_information_get(&sd_cardinfo);
				printf("sd_card_information_get\r\n");
			}else
			{
				return STA_NOINIT;
			}
			if(SD_OK == stat)
			{
				stat = sd_card_select_deselect(sd_cardinfo.card_rca);
				printf("sd_card_select_deselect\r\n");
				
			}else
			{
				return STA_NOINIT;
			}
			stat = sd_cardstatus_get(&cardstate);
			if(cardstate & 0x02000000)
			{
				while(1)
				{}
			}
			
			if(SD_OK == stat)
			{
				#ifdef BUSMODE_4BIT
					stat = sd_bus_mode_config(SDIO_BUSMODE_4BIT);
				#else 
					stat = sd_bus_mode_config(SDIO_BUSMODE_1BIT);
				#endif
				printf("sd_bus_mode_config\r\n");
			}else
			{
				return STA_NOINIT;
			}
			
			if(SD_OK == stat)
			{
				#ifdef DMA_MODE
					stat = sd_bus_mode_config(SD_DMA_MODE);
					nvic_irq_enable(SDIO_IRQn,0,0);
				#else 
					stat = sd_transfer_mode_config(SD_POLLING_MODE);
                    printf("SD_POLLING_MODE\r\n");
				#endif
				
			}else
			{
				return STA_NOINIT;
			}
			
			if(SD_OK == stat)
			{
				return 0;
				printf("FATFS Init Success...\r\n");
			}else
			{
				return STA_NOINIT;
			}
		default:
			stat = STA_NOINIT;
	}

    return stat;
}


/*!
    \brief      Reads Sector(s)
    \param[in]  pdrv: Physical drive number (0..)
    \param[in]  buff: Data buffer to store read data
    \param[in]  sector: Sector address (LBA)
    \param[in]  count: Number of sectors to write (1..128)
    \param[out] none
    \retval     DRESULT: Operation result
*/
DRESULT disk_read (
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
)
{
	
    sd_error_enum status = SD_ERROR;
	if(NULL == buff)
	{
		return RES_PARERR;
	}
	if(!count)
	{
		return RES_PARERR;
	}
	switch(pdrv)
    {
        case ATA:
            if(count == 1)
            {
                status = sd_block_read((uint32_t*)(&buff[0]), (uint32_t)(sector<<9),BLOCKSIZE);
            }
            else
            {
                status = sd_multiblocks_read((uint32_t*)(&buff[0]), (uint32_t)(sector<<9),BLOCKSIZE,(uint32_t)count);
            }
            if(SD_OK == status)
            {
                return RES_OK;
            }
            return RES_ERROR;

        default:
            return RES_PARERR;
    }

}
/*!
    \brief      Writes Sector(s)
    \param[in]  pdrv: Physical drive number (0..)
    \param[in]  buff: Data to be written
    \param[in]  sector: Sector address (LBA)
    \param[in]  count: Number of sectors to write (1..128)
    \param[out] none
    \retval     DRESULT: Operation result
*/
#if _USE_WRITE == 1
DRESULT disk_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
)
{
    sd_error_enum status = SD_ERROR;
    if(!count){
        return RES_PARERR;
    }

	switch(pdrv)
    {
        case ATA:
            if(count == 1)
            {
                status = sd_block_write((uint32_t*)buff, sector<<9,BLOCKSIZE);
            }
            else
            {
                status = sd_multiblocks_read((uint32_t*)buff,sector<<9,BLOCKSIZE,(uint32_t)count);
            }
            if(SD_OK == status)
            {
                return RES_OK;
            }
            return RES_ERROR;

        default:
            return RES_PARERR;
    }

}
#endif /* _USE_WRITE == 1 */

/*!
    \brief      I/O control operation
    \param[in]  pdrv: Physical drive number (0..)
    \param[in]  cmd: Control code
    \param[in]  buff: Buffer to send/receive control data
    \param[out] none
    \retval     DRESULT: Operation result
*/
#if _USE_IOCTL == 1
DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;

    res = disk.drv[pdrv]->disk_ioctl(disk.lun[pdrv], cmd, buff);

    return res;
}
#endif /* _USE_IOCTL == 1 */

/*!
    \brief      Gets Time from RTC
    \param[in]  none
    \param[out] none
    \retval     Time in DWORD
*/
__weak DWORD get_fattime (void)
{
    return 0;
}
