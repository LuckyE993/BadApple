 /********************************************************************************
   * ����Ӳ������������ɽ�ɿ�����GD32F470ZGT6    ʹ����Ƶ200Mhz    ����25Mhz
   * �� �� ��: V1.0
   * �޸�����: LC
   * �޸�����: 2023��06��12��
   * ���ܽ���:      
   ******************************************************************************
   * ��ɽ����Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ  
   * �����������www.lckfb.com   
   * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ  
   * ������̳��club.szlcsc.com   
   * ����ģ����ֲ�ֲ᣺����������ɽ�ɿ����塿ģ����ֲ�ֲ�
   * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
   * ��������׬Ǯ���������й�����ʦΪ����
 *********************************************************************************/

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "bsp_systick.h"
#define  delay_ms  delay_1ms

u8 OLED_GRAM[144][8];

/******************************************************************
 * �� �� �� �ƣ�OLED_ColorTurn
 * �� �� ˵ ������Ļ����
 * �� �� �� �Σ�i=0������ʾ  i=1��ɫ��ʾ
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע�����ڳ�ʼ�����֮����з�������
******************************************************************/
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//��ɫ��ʾ
		}
}

/******************************************************************
 * �� �� �� �ƣ�OLED_DisplayTurn
 * �� �� ˵ ������Ļ��ת180��
 * �� �� �� �Σ�i=0������ʾ  i=1��ת��ʾ
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע�����ڳ�ʼ�����֮�������ת�޸�
******************************************************************/
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//������ʾ
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//��ת��ʾ
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}

/******************************************************************
 * �� �� �� �ƣ�OLED_WR_Byte
 * �� �� ˵ ����LCD��������д�뺯��
 * �� �� �� �Σ�dat  Ҫд��Ĵ�������   cmd  ������������ݿ���
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע��ע����ʹ�����SPI����Ӳ��SPI
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
 * �� �� �� �ƣ�OLED_DisPlay_On
 * �� �� ˵ ��������OLED��ʾ 
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
	OLED_WR_Byte(0x14,OLED_CMD);//������ɱ�
	OLED_WR_Byte(0xAF,OLED_CMD);//������Ļ
}

/******************************************************************
 * �� �� �� �ƣ�OLED_DisPlay_Off
 * �� �� ˵ �����ر�OLED��ʾ 
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       ע����
******************************************************************/
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
	OLED_WR_Byte(0x10,OLED_CMD);//�رյ�ɱ�
	OLED_WR_Byte(0xAE,OLED_CMD);//�ر���Ļ
}

/******************************************************************
 * �� �� �� �ƣ�OLED_Refresh
 * �� �� ˵ ���������Դ浽OLED	
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע���޸�����Ļ��ʾ���ݺ������øú���������Ļ���ݸ��� 
******************************************************************/
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //��������ʼ��ַ
	   OLED_WR_Byte(0x00,OLED_CMD);   //���õ�����ʼ��ַ
	   OLED_WR_Byte(0x10,OLED_CMD);   //���ø�����ʼ��ַ
	   for(n=0;n<128;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}
/******************************************************************
 * �� �� �� �ƣ�OLED_Clear
 * �� �� ˵ ��������
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע��ȫ�����
******************************************************************/
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++) //8��
	{
	   for(n=0;n<128;n++) //128��
			{
			 OLED_GRAM[n][i]=0;//�����������
			}
  }
	OLED_Refresh();//������ʾ
}

/******************************************************************
 * �� �� �� �ƣ�OLED_DrawPoint
 * �� �� ˵ ��������
 * �� �� �� �Σ�x��λ��:0~127  y��λ��:0~63  t:1 ��� 0,���	
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
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
 * �� �� �� �ƣ�OLED_DrawLine
 * �� �� ˵ ��������
 * �� �� �� �Σ�x1,y1:�������    x2,y2:��������
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//����
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
 * �� �� �� �ƣ�OLED_DrawCircle
 * �� �� ˵ ������Բ
 * �� �� �� �Σ�x,y:Բ������   r:Բ�İ뾶
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
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
        num = (a * a + b * b) - r*r;//���㻭�ĵ���Բ�ĵľ���
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}


/******************************************************************
 * �� �� �� �ƣ�OLED_ShowChar
 * �� �� ˵ ������ָ��λ����ʾһ���ַ�,���������ַ�
 * �� �� �� �Σ�x��λ��:0~127  
 *             y��λ��:0~63  
 *             size1:ѡ������ 8/12/16/24
 *             mode:0,��ɫ��ʾ;1,������ʾ
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr1=chr-' ';  //����ƫ�ƺ��ֵ
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //����0806����
		else if(size1==12)
        {temp=asc2_1206[chr1][i];} //����1206����
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //����1608����
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //����2412����
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
 * �� �� �� �ƣ�OLED_ShowString
 * �� �� ˵ ������ʾ�ַ���
 * �� �� �� �Σ�x,y:�������
 *             size1:�����С  
 *             *chr:�ַ�����ʼ��ַ
 *             mode:0,��ɫ��ʾ;1,������ʾ
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
	while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
	{
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
  }
}

/******************************************************************
 * �� �� �� �ƣ�OLED_Pow
 * �� �� ˵ ����m��n�η���ֵ
 * �� �� �� �Σ�m��n
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
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
 * �� �� �� �ƣ�OLED_ShowNum
 * �� �� ˵ ������ʾ����
 * �� �� �� �Σ�x,y :�������
 *             num :Ҫ��ʾ������
 *             len :���ֵ�λ��
 *             size:�����С
 *             mode:0,��ɫ��ʾ;1,������ʾ
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
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
 * �� �� �� �ƣ�OLED_ShowChinese
 * �� �� ˵ ������ʾ����
 * �� �� �� �Σ�x,y :�������
 *             num:���ֶ�Ӧ�����
 *             size1:�����С  16/24/32/64
 *             mode:0,��ɫ��ʾ;1,������ʾ
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	for(i=0;i<size3;i++)
	{
		if(size1==16)
				{temp=Hzk1[num][i];}//����16*16����
		else if(size1==24)
				{temp=Hzk2[num][i];}//����24*24����
		else if(size1==32)       
				{temp=Hzk3[num][i];}//����32*32����
		else if(size1==64)
				{temp=Hzk4[num][i];}//����64*64����
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
 * �� �� �� �ƣ�OLED_ScrollDisplay
 * �� �� ˵ ������������
 * �� �� �� �Σ�num ��ʾ���ֵĸ���
 *             space ÿһ����ʾ�ļ��
 *             mode:0,��ɫ��ʾ;1,������ʾ
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(128,24,t,16,mode); //д��һ�����ֱ�����OLED_GRAM[][]������
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //��ʾ���
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
		for(i=1;i<144;i++)   //ʵ������
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
 * �� �� �� �ƣ�OLED_ShowPicture
 * �� �� ˵ ������ʾͼƬ
 * �� �� �� �Σ�x,y���������
 *             sizex,sizey,ͼƬ����
 *             BMP[]��Ҫд���ͼƬ����
 *             mode:0,��ɫ��ʾ;1,������ʾ
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
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
 * �� �� �� �ƣ�OLED_Init
 * �� �� ˵ ����OLED�ĳ�ʼ��
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void OLED_Init(void)
{
	spi_parameter_struct spi_init_struct={0};
		/* ����������ʱ�� */
	rcu_periph_clock_enable(RCU_LCD_D0);
	rcu_periph_clock_enable(RCU_LCD_D1);
	rcu_periph_clock_enable(RCU_LCD_CS);
	rcu_periph_clock_enable(RCU_LCD_DC);
	rcu_periph_clock_enable(RCU_LCD_RES);

		 /* ����DC */
	gpio_mode_set(PORT_LCD_DC,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_DC);
	gpio_output_options_set(PORT_LCD_DC,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_DC);
	gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET);
		 /* ����RES */
	gpio_mode_set(PORT_LCD_RES,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_RES);
	gpio_output_options_set(PORT_LCD_RES,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_RES);
	gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET);
		 /* ����CS */
	gpio_mode_set(PORT_LCD_CS,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_LCD_CS);
	gpio_output_options_set(PORT_LCD_CS,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_CS);
	gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET); 
    
#if USE_SOFTWARE
        /* ����D0 */
    gpio_mode_set(PORT_LCD_D0,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_D0);
    gpio_output_options_set(PORT_LCD_D0,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_D0);
    gpio_bit_write(PORT_LCD_D0, GPIO_LCD_D0, SET);
    
        /* ����D1 */
    gpio_mode_set(PORT_LCD_D1,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_D1);
    gpio_output_options_set(PORT_LCD_D1,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_D1);
    gpio_bit_write(PORT_LCD_D1, GPIO_LCD_D1, SET);
#else    
    
	/* ���� SPI��SCK GPIO */
	gpio_af_set(PORT_LCD_D0, LINE_AF_SPI, GPIO_LCD_D0);
	gpio_mode_set(PORT_LCD_D0, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_D0);
	gpio_output_options_set(PORT_LCD_D0, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_D0);
	gpio_bit_set(PORT_LCD_D0,GPIO_LCD_D0);
	/* ���� SPI��MOSI  GPIO */
	gpio_af_set(PORT_LCD_D1, LINE_AF_SPI, GPIO_LCD_D1);
	gpio_mode_set(PORT_LCD_D1, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_D1);
	gpio_output_options_set(PORT_LCD_D1, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_D1);
	gpio_bit_set(PORT_LCD_D1, GPIO_LCD_D1);         
    
	/* ʹ��SPIʱ�� */
	rcu_periph_clock_enable(RCU_SPI_HARDWARE); 
	/* ���� SPI ���� */
	spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;  // ����ģʽȫ˫��
	spi_init_struct.device_mode          = SPI_MASTER;   // ����Ϊ����
	spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT; // 8λ����
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
	spi_init_struct.nss                  = SPI_NSS_SOFT;  // ���cs
	spi_init_struct.prescale             = SPI_PSC_2;//2��Ƶ
	spi_init_struct.endian               = SPI_ENDIAN_MSB;
	spi_init(PORT_SPI, &spi_init_struct);
	/* ʹ�� SPI */
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
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
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

