 /********************************************************************************
   * 测试硬件：立创·梁山派开发板GD32F470ZGT6    使用主频200Mhz    晶振25Mhz
   * 版 本 号: V1.0
   * 修改作者: LC
   * 修改日期: 2023年06月12日
   * 功能介绍:      
   ******************************************************************************
   * 梁山派软硬件资料与相关扩展板软硬件资料官网全部开源  
   * 开发板官网：www.lckfb.com   
   * 技术支持常驻论坛，任何技术问题欢迎随时交流学习  
   * 立创论坛：club.szlcsc.com   
   * 其余模块移植手册：【立创·梁山派开发板】模块移植手册
   * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
   * 不靠卖板赚钱，以培养中国工程师为己任
 *********************************************************************************/

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "bsp_systick.h"
#define  delay_ms  delay_1ms

u8 OLED_GRAM[144][8];

/******************************************************************
 * 函 数 名 称：OLED_ColorTurn
 * 函 数 说 明：屏幕反显
 * 函 数 形 参：i=0正常显示  i=1反色显示
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：请在初始化完毕之后进行反显设置
******************************************************************/
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
		}
}

/******************************************************************
 * 函 数 名 称：OLED_DisplayTurn
 * 函 数 说 明：屏幕旋转180度
 * 函 数 形 参：i=0正常显示  i=1反转显示
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：请在初始化完毕之后进行旋转修改
******************************************************************/
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}

/******************************************************************
 * 函 数 名 称：OLED_WR_Byte
 * 函 数 说 明：LCD串行数据写入函数
 * 函 数 形 参：dat  要写入的串行数据   cmd  发送命令还是数据控制
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：注意是使用软件SPI还是硬件SPI
******************************************************************/
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	//u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else
	  OLED_DC_Clr();
	
#if USE_SOFTWARE
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{
		OLED_SCL_Clr();
		if(dat&0x80)
		   OLED_SDA_Set();
		else 
		   OLED_SDA_Clr();
		OLED_SCL_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();      
#else
    
	OLED_CS_Clr();
	while(RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_TBE));
        spi_i2s_data_transmit(PORT_SPI, dat);
	while(RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_RBNE));
        spi_i2s_data_receive(PORT_SPI);									
	OLED_CS_Set(); 
#endif
    
	OLED_DC_Set();   	  
}

/******************************************************************
 * 函 数 名 称：OLED_DisPlay_On
 * 函 数 说 明：开启OLED显示 
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

/******************************************************************
 * 函 数 名 称：OLED_DisPlay_Off
 * 函 数 说 明：关闭OLED显示 
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 备       注：无
******************************************************************/
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭屏幕
}

/******************************************************************
 * 函 数 名 称：OLED_Refresh
 * 函 数 说 明：更新显存到OLED	
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：修改了屏幕显示内容后必须调用该函数进行屏幕内容更新 
******************************************************************/
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
	   OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
	   OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
	   for(n=0;n<128;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}
/******************************************************************
 * 函 数 名 称：OLED_Clear
 * 函 数 说 明：清屏
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：全屏清除
******************************************************************/
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++) //8行
	{
	   for(n=0;n<128;n++) //128列
			{
			 OLED_GRAM[n][i]=0;//清除所有数据
			}
  }
	OLED_Refresh();//更新显示
}

/******************************************************************
 * 函 数 名 称：OLED_DrawPoint
 * 函 数 说 明：画点
 * 函 数 形 参：x轴位置:0~127  y轴位置:0~63  t:1 填充 0,清空	
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}
/******************************************************************
 * 函 数 名 称：OLED_DrawLine
 * 函 数 说 明：画线
 * 函 数 形 参：x1,y1:起点坐标    x2,y2:结束坐标
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

/******************************************************************
 * 函 数 名 称：OLED_DrawCircle
 * 函 数 说 明：画圆
 * 函 数 形 参：x,y:圆心坐标   r:圆的半径
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);
 
        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}


/******************************************************************
 * 函 数 名 称：OLED_ShowChar
 * 函 数 说 明：在指定位置显示一个字符,包括部分字符
 * 函 数 形 参：x轴位置:0~127  
 *             y轴位置:0~63  
 *             size1:选择字体 8/12/16/24
 *             mode:0,反色显示;1,正常显示
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //调用0806字体
		else if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}

/******************************************************************
 * 函 数 名 称：OLED_ShowString
 * 函 数 说 明：显示字符串
 * 函 数 形 参：x,y:起点坐标
 *             size1:字体大小  
 *             *chr:字符串起始地址
 *             mode:0,反色显示;1,正常显示
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
  }
}

/******************************************************************
 * 函 数 名 称：OLED_Pow
 * 函 数 说 明：m的n次方求值
 * 函 数 形 参：m，n
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}
/******************************************************************
 * 函 数 名 称：OLED_ShowNum
 * 函 数 说 明：显示数字
 * 函 数 形 参：x,y :起点坐标
 *             num :要显示的数字
 *             len :数字的位数
 *             size:字体大小
 *             mode:0,反色显示;1,正常显示
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
{
	u8 t,temp,m=0;
	if(size1==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
			}
  }
}

/******************************************************************
 * 函 数 名 称：OLED_ShowChinese
 * 函 数 说 明：显示汉字
 * 函 数 形 参：x,y :起点坐标
 *             num:汉字对应的序号
 *             size1:字体大小  16/24/32/64
 *             mode:0,反色显示;1,正常显示
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //得到字体一个字符对应点阵集所占的字节数
	for(i=0;i<size3;i++)
	{
		if(size1==16)
				{temp=Hzk1[num][i];}//调用16*16字体
		else if(size1==24)
				{temp=Hzk2[num][i];}//调用24*24字体
		else if(size1==32)       
				{temp=Hzk3[num][i];}//调用32*32字体
		else if(size1==64)
				{temp=Hzk4[num][i];}//调用64*64字体
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size1)
		{x=x0;y0=y0+8;}
		y=y0;
	}
}

/******************************************************************
 * 函 数 名 称：OLED_ScrollDisplay
 * 函 数 说 明：汉字左移
 * 函 数 形 参：num 显示汉字的个数
 *             space 每一遍显示的间隔
 *             mode:0,反色显示;1,正常显示
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(128,24,t,16,mode); //写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //显示间隔
				 {
					for(i=1;i<144;i++)
						{
							for(n=0;n<8;n++)
							{
								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
							}
						}
           OLED_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=1;i<144;i++)   //实现左移
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}
/******************************************************************
 * 函 数 名 称：OLED_ShowPicture
 * 函 数 说 明：显示图片
 * 函 数 形 参：x,y：起点坐标
 *             sizex,sizey,图片长宽
 *             BMP[]：要写入的图片数组
 *             mode:0,反色显示;1,正常显示
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
{
	u16 j=0;
	u8 i,n,temp,m;
	u8 x0=x,y0=y;
	sizey=sizey/8+((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
		 for(i=0;i<sizex;i++)
		 {
				temp=BMP[j];
				j++;
				for(m=0;m<8;m++)
				{
					if(temp&0x01)OLED_DrawPoint(x,y,mode);
					else OLED_DrawPoint(x,y,!mode);
					temp>>=1;
					y++;
				}
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y0=y0+8;
				}
				y=y0;
     }
	 }
}

/******************************************************************
 * 函 数 名 称：OLED_Init
 * 函 数 说 明：OLED的初始化
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void OLED_Init(void)
{
	spi_parameter_struct spi_init_struct={0};
		/* 开启各引脚时钟 */
	rcu_periph_clock_enable(RCU_LCD_D0);
	rcu_periph_clock_enable(RCU_LCD_D1);
	rcu_periph_clock_enable(RCU_LCD_CS);
	rcu_periph_clock_enable(RCU_LCD_DC);
	rcu_periph_clock_enable(RCU_LCD_RES);

		 /* 配置DC */
	gpio_mode_set(PORT_LCD_DC,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_DC);
	gpio_output_options_set(PORT_LCD_DC,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_DC);
	gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET);
		 /* 配置RES */
	gpio_mode_set(PORT_LCD_RES,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_RES);
	gpio_output_options_set(PORT_LCD_RES,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_RES);
	gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET);
		 /* 配置CS */
	gpio_mode_set(PORT_LCD_CS,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_LCD_CS);
	gpio_output_options_set(PORT_LCD_CS,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_CS);
	gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET); 
    
#if USE_SOFTWARE
        /* 配置D0 */
    gpio_mode_set(PORT_LCD_D0,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_D0);
    gpio_output_options_set(PORT_LCD_D0,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_D0);
    gpio_bit_write(PORT_LCD_D0, GPIO_LCD_D0, SET);
    
        /* 配置D1 */
    gpio_mode_set(PORT_LCD_D1,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_D1);
    gpio_output_options_set(PORT_LCD_D1,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_D1);
    gpio_bit_write(PORT_LCD_D1, GPIO_LCD_D1, SET);
#else    
    
	/* 配置 SPI的SCK GPIO */
	gpio_af_set(PORT_LCD_D0, LINE_AF_SPI, GPIO_LCD_D0);
	gpio_mode_set(PORT_LCD_D0, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_D0);
	gpio_output_options_set(PORT_LCD_D0, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_D0);
	gpio_bit_set(PORT_LCD_D0,GPIO_LCD_D0);
	/* 配置 SPI的MOSI  GPIO */
	gpio_af_set(PORT_LCD_D1, LINE_AF_SPI, GPIO_LCD_D1);
	gpio_mode_set(PORT_LCD_D1, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_D1);
	gpio_output_options_set(PORT_LCD_D1, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_D1);
	gpio_bit_set(PORT_LCD_D1, GPIO_LCD_D1);         
    
	/* 使能SPI时钟 */
	rcu_periph_clock_enable(RCU_SPI_HARDWARE); 
	/* 配置 SPI 参数 */
	spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;  // 传输模式全双工
	spi_init_struct.device_mode          = SPI_MASTER;   // 配置为主机
	spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT; // 8位数据
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
	spi_init_struct.nss                  = SPI_NSS_SOFT;  // 软件cs
	spi_init_struct.prescale             = SPI_PSC_2;//2分频
	spi_init_struct.endian               = SPI_ENDIAN_MSB;
	spi_init(PORT_SPI, &spi_init_struct);
	/* 使能 SPI */
	spi_enable(PORT_SPI);
#endif

	OLED_RES_Clr();
	delay_ms(200);
	OLED_RES_Set();
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_Clear();
	OLED_WR_Byte(0xAF,OLED_CMD);
}

