 
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
{	u8 temp[6] ; u8 tep;  //存放BMP文件数量信息
  u16 count= 1 ;	//计数，用于设定判断BMP文件内RGB数据地址
	u8 i =0   ;	    //每簇包含的扇区数量
	u8 first=0;
	u8 *buffer ;		//每笔读取1个扇区(512BYTE) ，存放于buffer
	u8 rgb,color_byte,byte1,byte2,byte3 ;
	u16 x=0,y=0 ;
	u32 color=0, tmp_color ;
	u16 BmpWidthx,BmpHeighty;
	u32 p;     //簇指示值//cluster
	u8 type[3]="BMP";
	u32 xy=0;  //存放已显示点的数量
	color_byte=byte2=byte3=0;
	//-----------------------------------
	if( image_all == 0 )    //读取总图片数
	  {
       	image_count =  0 ;
	     	byte1 = Search_man(&BMPInfo,&image_count,type) ;////当Count为零时，由它带回这个目录下"type"类型文件数量,不为零时由BMPInfo带回第Count个文件的的详细文件信息
			 /* 当image_count为零时，由它带回图片数量，不为零时由BMPInfo带回第image_count幅图片详细文件信息 
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

	p = (BMPInfo.deStartCluster)+(((u32)(BMPInfo.deHighClust))<<16);//读文件首簇firstcluster of the file
	//----------------------------------------------
	x = 0 ;	y = 0 ; //图像显示起始点
	rgb   = 0 ;
	count = 0 ;
	first = 0 ;	   //识别是否第一次读取数据，first=0，此时读取BMP头信息，并设置相关参数(BMP内数据地址、色深)
	buffer=malloc(512);
//	LCD_SetDisplayWindow(x,479,y,799);
//	LCD_WriteRAM_Prepare();
	while(1)
	 {
      i = 0 ;
	    for( ; i < SectorsPerClust ; i++ )      //簇，(扇区是构成簇的最小单位，此处扇区大小定位512byte大小。)
	         {
	            FAT_LoadPartCluster(p,i,buffer);//读一个扇区（读取第"i+1"个扇区）	//read a sector

	            if  (i==0 && first==0)  //首次读取BMP的数据的第一个扇区内包含BMP文件信息，从中识别BMP文件信息（数据地址、色彩类型、图像大小等）
	        	         { count= buffer[0x0a] ; color_byte = buffer[0x1c] / 8  ;  first = 1 ;
						           BmpWidthx=buffer[0x13];  BmpWidthx <<=8; BmpWidthx +=buffer[0x12];
						           BmpHeighty=buffer[0x17];	BmpHeighty<<=8; BmpHeighty+=buffer[0x16];
//						           LCD_SetDisplayWindow(x,(BmpWidthx-1),y,(BmpHeighty-1));
//						           LCD_WriteRAM_Prepare();
						         }
	            else   { count=0 ; }
				      // count储存从BMP文件开始到位图数据开始之间的数据(bitmap data)之间的偏移量
	            while(count<512)  //读取一簇512扇区 (SectorsPerClust 每簇4扇区数)
	                   {
		                      if(color_byte == 3)   //24位颜色图
		          	              {
                                  switch ( rgb )	 //通过“rgb”计数，把R G B数值拼接成”color“
                                     		{	//case 0: 第一笔数值为B
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
		                       else   // 16、32及其他 位色图
		                       	  {
			                           if(color_byte==2)  //16位颜色图
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
				                                   if(color_byte==4) //32位颜色图
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
		                       if(rgb == color_byte)   //两者相等，单点数据读取拼装完毕，显示
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
	         }  // 读取完一簇数据
	    free(buffer) ;
	    p=FAT_NextCluster(p);//读下一簇数据的地址	//read next cluster
	    buffer=malloc(512);	

	    if(p == 0x0fffffff || p == 0x0ffffff8 || (FAT32_Enable == 0 && p == 0xffff))//判断1幅BMP文件是否读取完整，如果无后续簇则结束
		       {    free(buffer) ;
		            return ;
		       }

   }
//	free(buffer) ;
}

 
