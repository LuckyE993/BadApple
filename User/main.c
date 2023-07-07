#include "gd32f4xx.h"
#include <stdio.h>
#include "main.h"
#include "bsp_led.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "gd32f4xx_it.h"
#include "string.h"
#include "bsp_dma.h"
#include "sdcard.h"
#include "diskio.h"
#include "ff.h"
#include "oled.h"
#include "bmp.h"
//#define DATA_PRINT                                          /* uncomment the macro to print out the data */


FATFS fs;

sd_card_info_struct sd_cardinfo;                  

void sd_fatfs_init (void);
//ErrStatus memory_compare(uint8_t* src,uint8_t* dst,uint16_t length);
//void Fat32_SD_test(void);
void video_player(void);

uint32_t buf_write[512];                                    /* store the data written to the card */
uint32_t buf_read[512];                                     /* store the data read from the card */

void nvic_config(void);
void card_info_get(void);

int main(void)
{
    systick_config();
    LED_Init();
    USART_Config();
    
    #if USART_DMA_INT_Control
    DMA_Config();
    printf("DMA OPEN SUCCESS\n");
    #endif
	
	sd_fatfs_init();
    card_info_get();
//    Fat32_SD_test();
	OLED_Init();				//初始化OLED
	OLED_Clear();
	
	
	OLED_Refresh();
	OLED_Clear();
	
	video_player();
	
	
	
    while(1)
    {
		//显示图片
        OLED_ShowPicture(0,0,128,64,BMP1,1);
        //更新屏幕显示
        OLED_Refresh();
        delay_1ms(2000);
        //清屏
        OLED_Clear();
        
        OLED_ScrollDisplay(11,4,1);
    }
}

/*!
    \brief      configure the NVIC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(SDIO_IRQn, 0, 0);
}


/*!
    \brief      get the card information and print it out by USRAT
    \param[in]  none
    \param[out] none
    \retval     none
*/
void card_info_get(void)
{
    uint8_t sd_spec, sd_spec3, sd_spec4, sd_security;
    uint32_t block_count, block_size;
    uint16_t temp_ccc;
    printf("\r\n Card information:");
    sd_spec = (sd_scr[1] & 0x0F000000) >> 24;
    sd_spec3 = (sd_scr[1] & 0x00008000) >> 15;
    sd_spec4 = (sd_scr[1] & 0x00000400) >> 10;
    if(2 == sd_spec) {
        if(1 == sd_spec3) {
            if(1 == sd_spec4) {
                printf("\r\n## Card version 4.xx ##");
            } else {
                printf("\r\n## Card version 3.0x ##");
            }
        } else {
            printf("\r\n## Card version 2.00 ##");
        }
    } else if(1 == sd_spec) {
        printf("\r\n## Card version 1.10 ##");
    } else if(0 == sd_spec) {
        printf("\r\n## Card version 1.0x ##");
    }

    sd_security = (sd_scr[1] & 0x00700000) >> 20;
    if(2 == sd_security) {
        printf("\r\n## SDSC card ##");
    } else if(3 == sd_security) {
        printf("\r\n## SDHC card ##");
    } else if(4 == sd_security) {
        printf("\r\n## SDXC card ##");
    }

    block_count = (sd_cardinfo.card_csd.c_size + 1) * 1024;
    block_size = 512;
    printf("\r\n## Device size is %dKB ##", sd_card_capacity_get());
    printf("\r\n## Block size is %dB ##", block_size);
    printf("\r\n## Block count is %d ##", block_count);

    if(sd_cardinfo.card_csd.read_bl_partial) {
        printf("\r\n## Partial blocks for read allowed ##");
    }
    if(sd_cardinfo.card_csd.write_bl_partial) {
        printf("\r\n## Partial blocks for write allowed ##");
    }
    temp_ccc = sd_cardinfo.card_csd.ccc;
    printf("\r\n## CardCommandClasses is: %x ##", temp_ccc);
    if((SD_CCC_BLOCK_READ & temp_ccc) && (SD_CCC_BLOCK_WRITE & temp_ccc)) {
        printf("\r\n## Block operation supported ##");
    }
    if(SD_CCC_ERASE & temp_ccc) {
        printf("\r\n## Erase supported ##");
    }
    if(SD_CCC_WRITE_PROTECTION & temp_ccc) {
        printf("\r\n## Write protection supported ##");
    }
    if(SD_CCC_LOCK_CARD & temp_ccc) {
        printf("\r\n## Lock unlock supported ##");
    }
    if(SD_CCC_APPLICATION_SPECIFIC & temp_ccc) {
        printf("\r\n## Application specific supported ##");
    }
    if(SD_CCC_IO_MODE & temp_ccc) {
        printf("\r\n## I/O mode supported ##");
    }
    if(SD_CCC_SWITCH & temp_ccc) {
        printf("\r\n## Switch function supported ##");
    }
}

void sd_fatfs_init (void)
{
    uint16_t result = 0;
    result = f_mount(&fs,"0:",1);
    if(result == FR_NO_FILESYSTEM)
    {
        printf("No file system in SD...\r\n");
        while (1);
        
    }
    else if(result != FR_OK)
    {
        printf("File mount error for SD! Result:(%d)\r\n",result);
        printf("Initilization error maybe!\r\n");
        while (1);
        
    }
    else
    {
        printf("File system existed in SD!\r\n");
    }
}


void video_player(void)
{
	unsigned char G_Bin[1024];
	/*BIN图形显存 */
	FRESULT res;
	/*用来指示运行情况 */
	FIL fsrc;
	/* 文件对象指针 */
	uint32_t ls_move = 0;
	/* 存储文件指针移动 */
	UINT br;
	/*成功读取的字节个数 */
	res = f_open(&fsrc, "0:/badapple.bin", FA_READ);
	if (res == FR_OK) {
		//文件的路径要对应，即前面把badapple.bin放在SD根目录下
		while (1) {
			res = f_lseek(&fsrc, ls_move);
			res = f_read(&fsrc, G_Bin, sizeof(G_Bin), &br);
			if (br != 1024) {
				/* 已经把数据读取完成 */
				ls_move = 0;
				break;
			}
			OLED_Refresh();
			
			OLED_ShowPicture(0, 0, 128, 64, G_Bin,1);/*显示帧数据 */

			delay_1ms(26);  /*加点延迟，看起来不会太鬼畜*/
			ls_move += 1024;   /*bin件数据位置偏移 */
		}
	}
}


//ErrStatus memory_compare(uint8_t* src,uint8_t* dst,uint16_t length)
//{
//    while(length--)
//    {
//        if(*src++ != *dst++)
//        {
//            return ERROR;
//        }
//        return SUCCESS;
//    }
//	return ERROR;
//}

//void Fat32_SD_test(void)
//{
//    FRESULT result;
//    FIL fdst;
//    FIL fsrc;
//    UINT br,bw;
//    unsigned int a;
//    FRESULT res;
//    FILINFO finfo;
//    DIR dirs;
//    uint16_t count = 0;

//    BYTE textfilebuffer[] = "GD32 FATFS TEST";
//    BYTE buffer[512];
//    
//    result = f_open(&fdst,"0:/FATFS.TXT",FA_CREATE_NEW|FA_WRITE);
//    
//    if((FR_OK == result)||(FR_EXIST == result))
//    {
//        if(FR_OK == result)
//        {
//            result = f_write(&fdst,textfilebuffer,sizeof(textfilebuffer),&bw);
//            printf("\r\nFATFS File Create Success\r\n");
//            f_close(&fdst);
//        }
//        else
//        {
//            if(FR_EXIST == result)
//            {
//                printf("\r\nFATFS File Already Exits\r\n");
//                f_open(&fdst,"0:/FATFS.TXT",FA_OPEN_EXISTING|FA_READ);
//                br = 1;
//                for(;;)
//                {
//                    for(count = 0;count < 512;count++)
//                    {
//                        buffer[count] = 0;
//                    }
//                    result = f_read(&fdst,buffer,sizeof(buffer),&br);
//                    if((0 == result)||(0 == br))
//                    {
//                        break;
//                    }
//                }
//                if(SUCCESS == memory_compare(buffer,textfilebuffer,sizeof(textfilebuffer)))
//                {
//                    printf("\r\nFATFS Read File Success\r\n");
//                }
//                if(f_opendir(&dirs,"0:") == FR_OK)
//                {
//                    while(f_readdir(&dirs,&finfo) == FR_OK)
//                    {
//                        if(finfo.fattrib & AM_ARC)
//                        {
//                            uint8_t temp[20];
//                            char *ptr = (char*)temp;
//                            if(!finfo.fname[0]) break;
//                            printf("finfo.fname: %s\r\n",finfo.fname);
//                            sprintf(ptr,"0:/%s",finfo.fname);
//                            res = f_open(&fsrc,ptr,FA_OPEN_EXISTING|FA_READ);
//                            br = 1;
//                            a = 0;
//                            printf("Content:\r\n");
//                            for(a=0;a<512;a++)buffer[a] = 0;
//                            res = f_read(&fsrc,buffer,sizeof(buffer),&br);
//                            printf("%s\r\n",buffer);
//                            if(res|| br == 0)break;
//                            f_close(&fsrc);
//                        }
//                    }
//                }
//                f_close(&fsrc);
//            }
//        }
//    }else if(FR_NO_FILESYSTEM == result)
//    {
//        printf("\r\n Please Format First!\r\n");
//    }else
//    {
//        printf("\r\n Open File Failed!\r\n");
//        
//    }
//    
//}
