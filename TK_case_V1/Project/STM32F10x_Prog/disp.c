 
/******************** (C) COPYRIGHT 2010 GIANTPLUS ********************
* File Name          : main.c
* Author             :  
* Version            : V1.0.0
* Date               : 2011-2-15 14:46:57
*******************************************************************************/
#include "lcd.h"
#include "FAT.h"
#include "sdcard.h"
//#include "disp.h"
 
extern BYTE FAT32_Enable; 
extern WORD SectorsPerClust;
direntry BMPInfo; 
u16 image_count = 0;
u16 image_all = 0;
//===================================================================================


//  void Disp_BMP(void);

void Disp_BMP(void)
{	u8 temp[6] ; u8 tep;  //���BMP�ļ�������Ϣ
  u16 count= 1 ;	//�����������趨�ж�BMP�ļ���RGB���ݵ�ַ
	u8 i =0   ;	    //ÿ�ذ�������������
	u8 first=0;
	u8 *buffer ;		//ÿ�ʶ�ȡ1������(512BYTE) �������buffer
	u8 rgb,color_byte,byte1,byte2,byte3 ;
	u16 x=0,y=0 ;
	u32 color=0, tmp_color ;
	u16 BmpWidthx,BmpHeighty;
	u32 p;     //��ָʾֵ//cluster
	u8 type[3]="BMP";
	u32 xy=0;  //�������ʾ�������
	color_byte=byte2=byte3=0;
	//-----------------------------------
	if( image_all == 0 )    //��ȡ��ͼƬ��
	  {
       	image_count =  0 ;
	     	byte1 = Search_man(&BMPInfo,&image_count,type) ;////��CountΪ��ʱ�������������Ŀ¼��"type"�����ļ�����,��Ϊ��ʱ��BMPInfo���ص�Count���ļ��ĵ���ϸ�ļ���Ϣ
			 /* ��image_countΪ��ʱ����������ͼƬ��������Ϊ��ʱ��BMPInfo���ص�image_count��ͼƬ��ϸ�ļ���Ϣ 
	        */
		    image_all = image_count ;
	     	image_count = 1 ;
          
         { tep=image_all;
           temp[5]='.';
           temp[4]=(tep%10+0x30);tep/=10;
           temp[3]=(tep%10+0x30);tep/=10;
           temp[2]=(tep%10+0x30);tep/=10;
           temp[1]=(tep%10+0x30);tep/=10;
           temp[0]=(tep%10+0x30);
         }
//           LCD_DisplayStringLine(0,Line3,"File read success");
//		       LCD_DisplayStringLine(0,Line4,"TotalBmp:");
//           LCD_DisplayStringLine(9*16,Line4,temp); 
	  }
	  
	else
    { 	byte1 = Search_man(&BMPInfo,&image_count,type) ;
    }

	p = (BMPInfo.deStartCluster)+(((u32)(BMPInfo.deHighClust))<<16);//���ļ��״�firstcluster of the file
	//----------------------------------------------
	x = 0 ;	y = 0 ; //ͼ����ʾ��ʼ��
	rgb   = 0 ;
	count = 0 ;
	first = 0 ;	   //ʶ���Ƿ��һ�ζ�ȡ���ݣ�first=0����ʱ��ȡBMPͷ��Ϣ����������ز���(BMP�����ݵ�ַ��ɫ��)
	buffer=malloc(512);
//	LCD_SetDisplayWindow(x,479,y,799);
//	LCD_WriteRAM_Prepare();
	while(1)
	 {
      i = 0 ;
	    for( ; i < SectorsPerClust ; i++ )      //�أ�(�����ǹ��ɴص���С��λ���˴�������С��λ512byte��С��)
	         {
	            FAT_LoadPartCluster(p,i,buffer);//��һ����������ȡ��"i+1"��������	//read a sector

	            if  (i==0 && first==0)  //�״ζ�ȡBMP�����ݵĵ�һ�������ڰ���BMP�ļ���Ϣ������ʶ��BMP�ļ���Ϣ�����ݵ�ַ��ɫ�����͡�ͼ���С�ȣ�
	        	         { count= buffer[0x0a] ; color_byte = buffer[0x1c] / 8  ;  first = 1 ;
						           BmpWidthx=buffer[0x13];  BmpWidthx <<=8; BmpWidthx +=buffer[0x12];
						           BmpHeighty=buffer[0x17];	BmpHeighty<<=8; BmpHeighty+=buffer[0x16];
//						           LCD_SetDisplayWindow(x,(BmpWidthx-1),y,(BmpHeighty-1));
//						           LCD_WriteRAM_Prepare();
						         }
	            else   { count=0 ; }
				      // count�����BMP�ļ���ʼ��λͼ���ݿ�ʼ֮�������(bitmap data)֮���ƫ����
	            while(count<512)  //��ȡһ��512���� (SectorsPerClust ÿ��4������)
	                   {
		                      if(color_byte == 3)   //24λ��ɫͼ
		          	              {
                                  switch ( rgb )	 //ͨ����rgb����������R G B��ֵƴ�ӳɡ�color��
                                     		{	//case 0: ��һ����ֵΪB
			                                    case 0 : color = buffer[count]  ;	
			                                             break ;
			                                    case 1 : tmp_color = buffer[count];
			                                    	       tmp_color<<=8;
			                                    	       color |= tmp_color ;
					                                         break ;
                                          case 2 : tmp_color = buffer[count];
			                                    	       tmp_color<<=16;
			                                    	       color |= tmp_color ;
			                                             break ;
					                                         
					                                     /*
					                                      case 0 : tmp_color = buffer[count]>>3 ;	
			                                             color |= tmp_color;
					                                         break ;
			                                          case 1 : tmp_color = buffer[count]>>2 ;
			                                             tmp_color <<= 5 ;
			                                             color |= tmp_color ;
					                                         break ;
                                                case 2 : tmp_color = buffer[count]>>3 ;
			                                             tmp_color <<= 11 ;
			                                             color |= tmp_color ;
					                                         break ;
					                                     */    
			            	                    }
                      		        rgb ++ ;
			      	                }
		                       else   // 16��32������ λɫͼ
		                       	  {
			                           if(color_byte==2)  //16λ��ɫͼ
				                              {
				                                    switch ( rgb )
					                                  {
					                                    case 0 : byte1 = buffer[count] ;
							                                         break ; 

						                                  case 1 : color = buffer[count] ;
							                                         color<<=8 ;
							                                         color |= byte1 ;
							                                         break ;
					                                   }
					                               rgb ++ ;
				                              }
			                           else
				                              {
				                                   if(color_byte==4) //32λ��ɫͼ
				                                        {
				                                            switch ( rgb )
					                                              {
					                                                  case 0 :  byte1 = buffer[count] ;
							                                                         break ; 

						                                                case 1 :  byte2 = buffer[count] ;
                                            		                       break ;

						                                                case 2 :  byte3 = buffer[count] ;
							                                                         break ;

						                                                case 3 :  tmp_color = byte1 >> 3 ;
			                                                                color |= tmp_color;
							                                                        tmp_color = byte2 >>2 ;
			                                                                tmp_color <<= 5 ;
			                                                                color |= tmp_color ;
							                                                        tmp_color = byte3 >>3 ;
			                                                                tmp_color <<= 11 ;
			                                                                color |= tmp_color ;
							                                                        break ;
					 	                                            }
					                                          rgb ++ ;
				                                        }

				                              }

	   	                        }
		                       count ++ ;
		                       if(rgb == color_byte)   //������ȣ��������ݶ�ȡƴװ��ϣ���ʾ
		                          {
			                              
//			                              LCD_DrawPoint(color);
			                              color = 0x00 ;rgb = 0 ;
			                              xy++ ;
			                              if(xy==(BmpWidthx*BmpHeighty))
				                                { free(buffer) ;	xy=0;
		              	             	          return ;
			          	                        }
			                              
		                          }
	                   }
	         }  // ��ȡ��һ������
	    free(buffer) ;
	    p=FAT_NextCluster(p);//����һ�����ݵĵ�ַ	//read next cluster
	    buffer=malloc(512);	

	    if(p == 0x0fffffff || p == 0x0ffffff8 || (FAT32_Enable == 0 && p == 0xffff))//�ж�1��BMP�ļ��Ƿ��ȡ����������޺����������
		       {    free(buffer) ;
		            return ;
		       }

   }
//	free(buffer) ;
}

 
