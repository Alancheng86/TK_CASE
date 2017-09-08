/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "lib.h"
#include "platform_config.h"
#include "lcd.h"
#include "IICS.h"
#include "IIC.h"
#include "STORE.h"

////////////9.1. Sample Code: Definitions
////////////These are the definitions for sample code.

static unsigned int cfcrc_table[256]=
	{
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

//static unsigned int crc32(unsigned int crc, unsigned char * buffer, unsigned int size);
//static unsigned int crc32(unsigned int crc,  unsigned int size)  
 unsigned int cfcrc(unsigned int a,unsigned int b)  ; 
unsigned int cfcrc(unsigned int a,unsigned int b)  
{ unsigned char buffer[8];
	unsigned int size =8; 
	unsigned int crc;
	unsigned int i; 
	crc = 0xffffffff;   
  buffer[0]=a;
  buffer[1]=a>>8;
  buffer[2]=a>>16;
  buffer[3]=a>>24;
  buffer[4]=b;
  buffer[5]=b>>8;
  buffer[6]=b>>16;
  buffer[7]=b>>24;	
  for (i = 0; i < size; i++) 
	      {  
         crc = cfcrc_table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);  
        }  
    crc=crc-19901101;
		return crc ;  
}  
  



//=====================================================================
extern void DelayMs(__IO uint32_t nTime);
extern u16 FontC;
extern u32 ID_VAL;
u8 cmmmg;  //����ʹ��
/****************************************************
* �����p���惦ģ�K��ֹ�����Gʧ
* ���ȴ�1Ӌ���^��		           0x79-0x7C
* ���ȴ�2Ӌ���^��		           0x79-0x7C
* �Ӄȴ�Ӌ���^�� 		           0x79-0x7C

* �Ӄȴ��ѷ�����ʼֵ���I           0x78    ��0xCC��
* ���ȴ�1�ѷ�����ʼֵ���Iλ        0x78    ��0XCC��
* ���ڴ�2�ѷ�����ʼֵ���Iλ        0x78    ��OxCC��

* �Ӄȴ��ѷ����Ř��I             0x80    ��0xCC��
* ���ڴ�洢�ѷ�����ģ�����       0x81-0x84
* ���ڴ�洢������		       0x81-0x84

* ���ڴ������������           0x70
* ���ڴ��ۼƼ�������           0x79-0x7C
* ���ڴ���ʼ��ַ����           0x89-0x8C
* ���ڴ���Կ����               0xA1-0xA3
* ��Կ 0x43,0x45,0x50

* CRC1��У������               0x85-0x88
* CRC2��У������               0x95-0x98


* �Ӵ惦ģ�K��ַ�� ��0xA0   �x0xA1      
* ���惦ģ�K1��ַ����0xAE   �x0xAF
* ���惦ģ�K2��ַ����0xA6   �x0xA7 
*
*****************************************************/

#define Slave_count_Mark    0x78

#define Lifetimes_check     0x70    

#define Slave_count_P       0x79    //0x79-0x7C
#define Slave_count_S       0x89    //0x89-0x8C
#define Slave_BH_mark       0x80
#define Slave_BH_P          0x81    //0x81-0x84

#define CRC_code1           0x85	//0x85-0x88
#define CRC_code2           0x95	//0x95-0x98

#define Master_count_Mark   0x78
#define Master_count_P      0x79    //0x79-0x7C
#define Master_BH_Mark      0x80
#define Master_BH_P         0x81    //0x81-0x84


unsigned char ARR_MASTER1 = 0xA2;
unsigned char ARR_MASTER2 = 0xA6;
unsigned char ARR_SLAVE   = 0xA0;

unsigned char Name1_addr=0x60;
unsigned char Name2_addr=0x61;

unsigned char Name1=0x60;
unsigned char Name2=0x42;

unsigned char JTY[3]={0};

unsigned long LIFE_TIMES=10000;   //IDģ������10000��
/***************************************
*  �惦ģ�K��ʼ��
*  
***************************************/
void Init_External_EEPROM(void)
{                 
	EEPROM_GPIO_ConfigS();					 
}


/*************************************
*  �Ӵ惦ģ�K
*  �Ӵ惦ģ�KӋ��ֵ��ȡ
***************************************/
u32 SLAVE_EEPROM_COUNT_NUM(void)
{
	
	 u32 coun1=0,coun2=0;
	 u8 markk;
	 markk=GET_SLAVE_EEPROM_COUNT_SIGNAL(Slave_count_Mark);  //�Д��Ӵ惦ģ�K�Ƿ��ѳ�ʼ��
	 if(markk!=0xcc)
	 { 
		 Allcolor(1,0xffffff);    Delays(120);
	 	 ShowTxt("60,20,100,δʶ�𵽴洢ģ�飡��");	    Delays(100);//����Ļ����ʾ�Ñ�Ո����Ӌ����ʼֵ��
		 while(1)			
		{BEEP_Dudu();}
	 }
//------------------------------------------------------------	 
		 coun2=Check_CRC_Code();    //����CRCУ�� �����ر��
	   DelayMs(2); 
		 coun1=SLAVE_Read_Data(ARR_SLAVE,Slave_count_P);   //��ȡ��Ƭ��Ʒ���������IDֵ
	   DelayMs(15);  
//	   if(coun1==0x00000000)    //���ID_VAL����Ĭ��ֵ0x00,0x00,0x00   ;������    
//		 {coun1++;}	           //���ͺ�Ĭ��ֵ�  [ 00��00��00 ]
		 //��ʾ�û�����Ҫ�����ֵ

	  ShowTxt("32,0,150, �ر�ţ�");	  			 DelayMs(100);
	  ShowData("32,180,150",coun2) ;		       DelayMs(100);
    return 	coun1; 
}


/*************************************
*  �Ӵ惦ģ�K
*  �Ӵ惦ģ�K����Ӌ��+1
***************************************/
void SAVE_SLAVE_EEPROM_COUNT_NUM(u32 datat)
{
		u32 coun1=0,coun2=0,coun3=0,coun4=0;
		coun1=datat;   //��ȡ��Ƭ��Ƭ�����IDֵ
    coun1++;
	  Delays(5);
		SLAVE_Write_Data(ARR_SLAVE,Slave_count_P,coun1);	//���¼���ֵ + 1
	  DelayMs(15);
	  SLAVE_Write_Data(ARR_SLAVE,Lifetimes_check,coun1); 
	  DelayMs(15);
    coun4=SLAVE_Read_Data(ARR_SLAVE,Slave_count_P); 
	  DelayMs(15);
	  if(coun4==coun1)
		{
		coun2=SLAVE_Read_Data(ARR_SLAVE,Slave_BH_P);      //��ȡ�ر��
		coun3=cfcrc(coun2,coun1);                         //��������ϱ仯��CRC2
		SLAVE_Write_Data(ARR_SLAVE,CRC_code2,coun3);	    //����CRC2
		}
		else
		{
		  SSD2828_VIDEO_MODE_HS();
	    DelayMs(10);			
			Allcolor(1,0xffffff);    Delays(120);
		  ShowTxt("32,0,150, ģ���쳣��");	  			 DelayMs(100); 
	 	  ShowTxt("60,20,100,δʶ�𵽴洢ģ�飡��");	    Delays(100);//����Ļ����ʾ�Ñ�Ո����Ӌ����ʼֵ��
		  while(1)			
		  {BEEP_Dudu();}
		}
}


/*************************************
*  �Ӵ惦ģ�K
*  �Ӵ惦ģ�K��������
*  Ŀ�ģ����ģ����������ͻ������ֹ
*        ���ģ���Ƿ�������ͺ�
***************************************/
void ID_Model_Lifetimes_CHECK(void)
{
		u32 coun1=0,coun2=0;
	
	  coun1=EEPROMBYTE_ReadS(ARR_SLAVE,Lifetimes_check);         Delays(15);   //coun1  0~255����ѭ��
	  coun1++;Delays(5);
		EEPROMBYTE_WriteS(ARR_SLAVE,Lifetimes_check,coun1);        Delays(15);
	  
	  coun2=EEPROMBYTE_ReadS(ARR_SLAVE,Lifetimes_check);         Delays(15);
	
		if(coun2!=coun1)   //�Ա�ǰ��ֵ�������һ�£���ʾIDģ����������ͻ������ֹ�����������ѣ�
    {	 	 
			  Allcolor(1,0xffffff);    Delays(120);
				ShowTxt("60,20,100,IDģ���쳣��");	    Delays(100);
			  ShowTxt("60,20,200,�����Ի��߸���ģ�飡��");
        ShowTxt("60,20,300,δ�Ըò�Ʒ��������");	
			  while(1)			
				{BEEP_Dudu();}	   
		}
//		Read_LCD_MODEL();  
}

/***************************************
*  �Ӵ惦ģ�K
*  �xȡ�Ӵ惦ģ�K��Կ
***************************************/
void GET_SLAVE_EEPROM_KEY()
{	 
	 JTY[0]=EEPROMBYTE_ReadS(ARR_SLAVE,0xA1);        Delays(10);
	 JTY[1]=EEPROMBYTE_ReadS(ARR_SLAVE,0xA2);        Delays(10); 
	 JTY[2]=EEPROMBYTE_ReadS(ARR_SLAVE,0xA3);        Delays(10); 		
}

/***************************************
*  �Ӵ惦ģ�K
*  �xȡ�Ӵ惦ģ�K��Կ
***************************************/
u8 SLAVE_EEPROM_KEY_CHECK()
{
	 GET_SLAVE_EEPROM_KEY();
	 if((JTY[0]==0x43)&&(JTY[1]==0x45)&&(JTY[2]==0x50))	
	 {
	    return 1;
	 }
	 else
	 {
		 return 0;
	 }
}



/***************************************
*  �Ӵ惦ģ�K
*  ������Կ
***************************************/
void WRITE_SLAVE_EEPROM_KEY(void)
{	
   EEPROMBYTE_WriteS(ARR_SLAVE,0xA1,0x43);  		Delays(10);
   EEPROMBYTE_WriteS(ARR_SLAVE,0xA2,0x45);  		Delays(10);
   EEPROMBYTE_WriteS(ARR_SLAVE,0xA3,0x50);  		Delays(10);
}
/***************************************
*  �Ӵ惦ģ�K
*  �xȡ�Ӵ惦ģ�K���Iλ
***************************************/
u8 GET_SLAVE_EEPROM_COUNT_SIGNAL(u8 adrr)
{
	 u8 signn;
	 signn=EEPROMBYTE_ReadS(ARR_SLAVE,adrr);        Delays(10);
   return signn;		
}
/***************************************
*  �Ӵ惦ģ�K
*  ��ȡCRC�� //��������
***************************************/
u32 Check_CRC_Code(void)
{
   u32 tep1,tep2,tep3,crc1,crc2;
   u32 Get_crc1,Get_crc2;
	
   tep1=SLAVE_Read_Data(ARR_SLAVE,Slave_BH_P);	   //��оƬ���
   tep2=SLAVE_Read_Data(ARR_SLAVE,Slave_count_S);  //��оƬ��ʼ����ֵ
   tep3=SLAVE_Read_Data(ARR_SLAVE,Slave_count_P);  //��оƬ�ۼƼ���ֵ
	 
	 if(tep3>((tep1*LIFE_TIMES)-1))
	 {
		 Allcolor(1,0xffffff);    Delays(120);
	   ShowTxt("60,20,100,�洢ģ��ռ�������");            Delays(100);
		 ShowTxt("60,20,250,������洢ģ�飡");              Delays(100);
		 ShowTxt("60,20,300,��Ƭ��ƷIDδ�գ�����ģ������ԣ�");            Delays(100);
		 ;//��ʾ�Ñ����Q�Ӵ惦оƬ��
		 while(1)			
		{BEEP_Dudu();}
	 }	 
  
   crc1=cfcrc(tep1,tep2);             //��������CRC1��
   crc2=cfcrc(tep1,tep3); 

	 Get_crc1=SLAVE_Read_Data(ARR_SLAVE,CRC_code1);   //��ȡCRC1��
	 Get_crc2=SLAVE_Read_Data(ARR_SLAVE,CRC_code2); 
	
	 if((crc1!=Get_crc1)||(crc2!=Get_crc2))
	 {
		 Allcolor(1,0xffffff);    Delays(120);
	   ShowTxt("60,20,100,оƬ����");            Delays(100);
     //�����Լҵ�оƬ���ߣ���ֹʹ�ã�  
		 ;//��ʾ�Ñ����Q�Ӵ惦оƬ��
		 while(1)			
		{BEEP_Dudu();}
	 }

    return tep1;   //���ظô洢ģ����
}
/***************************************
*  �Ӵ惦ģ�K
*  �xȡ�Ӵ惦ģ�K��ǰӋ��ֵ
***************************************/
u32 GET_SLAVE_EEPROM_COUNT_NUM()
{
	 u32 cou_num=0;
	 cou_num=SLAVE_Read_Data(ARR_SLAVE,Slave_count_P);
	 return cou_num;				 
}

/***************************************
*  �Ӵ惦ģ�K
*  �Ӵ惦ģ�K������ʼӋ��ֵ
***************************************/
u8 RENEW_SLAVE_EEPROM_NUM(u32 count)
{
	 u32 countt=count;
	 SLAVE_Write_Data(ARR_SLAVE,Slave_count_P,countt);
	 SLAVE_Write_Data(ARR_SLAVE,Slave_count_S,countt);
	 EEPROMBYTE_WriteS(ARR_SLAVE,Slave_count_Mark,0xcc);		Delays(10);	
}

/***************************************
*  �Ӵ惦ģ�K
*  �Ӵ惦ģ�K��ȡ��ʼӋ��ֵ
***************************************/
u32 START_SLAVE_EEPROM_NUM(void)
{
	 u32 cou_num=0;
	 cou_num=SLAVE_Read_Data(ARR_SLAVE,Slave_count_S);
	 return cou_num;
}

/***************************************
*  �Ӵ惦ģ�K
*  ��ȡ�ͺ���
***************************************/
void Read_LCD_MODEL(void)
{
	 u8 M_name1,M_name2;
	 M_name1=EEPROMBYTE_ReadS(ARR_SLAVE,Name1_addr);
	 M_name2=EEPROMBYTE_ReadS(ARR_SLAVE,Name2_addr);
	 if((M_name1!=Name1)||(M_name2!=Name2))
	 {
		 Allcolor(1,0xffffff);    Delays(120);
	   ShowTxt("60,20,100,�洢ģ�鲻���䣡");            Delays(100);
		 ;//��ʾ�Ñ����Q�Ӵ惦оƬ��
		 while(1)			
		{BEEP_Dudu();}
	 }
	 return ;
}

/***************************************
*  �Ӵ惦ģ�K
*  �Ӵ惦ģ�K�����ݺ���
***************************************/
u32  SLAVE_Read_Data(u8 slave,u8 adrr)
{
	 u8 tep1,tep2,tep3,tep4;
	 u32 cou_num=0;
	 tep1=EEPROMBYTE_ReadS(slave,adrr);          Delays(10);
	 tep2=EEPROMBYTE_ReadS(slave,adrr+1);        Delays(10);
	 tep3=EEPROMBYTE_ReadS(slave,adrr+2);        Delays(10);
	 tep4=EEPROMBYTE_ReadS(slave,adrr+3);        Delays(10);
	 cou_num=(tep1<<24)+(tep2<<16)+(tep3<<8)+tep4; Delays(10);
	 return cou_num;
}

/***************************************
*  �Ӵ惦ģ�K
*  �Ӵ惦ģ�Kд���ݺ���
***************************************/
void  SLAVE_Write_Data(u8 slave,u8 adrr,u32 data)
{
	 EEPROMBYTE_WriteS(slave,adrr,  data>>24);  Delays(10);
	 EEPROMBYTE_WriteS(slave,adrr+1,data>>16);  Delays(10);
	 EEPROMBYTE_WriteS(slave,adrr+2,data>>8);   Delays(10);
	 EEPROMBYTE_WriteS(slave,adrr+3,data);		Delays(10);
}


void initial_Slave_model(void)
{
	SLAVE_Write_Data(ARR_SLAVE,Slave_count_P,0xffffffff);			 //����
	EEPROMBYTE_WriteS(ARR_SLAVE,Slave_count_Mark,0xff);    Delays(5);	  //��ʼ����־λ
	SLAVE_Write_Data(ARR_SLAVE,Slave_BH_P,0xffffffff);			 //���
	EEPROMBYTE_WriteS(ARR_SLAVE,Slave_BH_mark ,0xff);    Delays(5);	  //����������־λ
}


void Delays(u32 nCount)
{ 
	 u16 i,j;
	for(j=0;j<nCount;j++)
	{
   for(i=0;i<6666;i++)
	{;;;}
	}
}