
#include "stm32f10x.h"
#include <TSLIIC_Brightness.h>
#include "TSLIIC_REV01.h"

/*******************************************************************************
* Function Name  : Delay_ms //延时N毫秒
* Description    : 延时
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//extern void DelayMs(__IO uint32_t nTime);
void Dly_1ms (u32 k);

#define lvtimes 16				   //测亮度时光感采样次数
#define S1_addr 0X92
#define S2_addr 0X72
#define S3_addr 0X52 
//static 
//u16 aver [3][lvtimes]={0}; 



float ave_Bright (void)				 //光感部分子程序，计算出光感读数
{ 
	//u32 max[3]={0,0,0};		
	u8 t=0,t1=0,count=0;
	u16 aver [3][lvtimes];
	u16 i=0;
	//u16 avern1,avern2,avern3;
	u16 temp1,temp2,temp3;
	float avern=0;
	
	
	//avern1=avern2=avern3=0;
	
	//max[0]=max[1]=max[2]=0;
	

	
	  for (i=0;i<lvtimes;i++)
	  {
		  aver[0][i]=Get_Bright(S1_addr);
	   // max[0]+=aver[0][i];
	    Dly_1ms (2);   //┑5ms

		  aver[1][i]=Get_Bright(S2_addr);
	    //max[1]+=aver[1][i];
	    Dly_1ms (2);   //┑5ms

		  aver[2][i]=Get_Bright(S3_addr);
	    //max[2]+=aver[2][i];
	    Dly_1ms (2);   //┑5ms
	  }
	  
	  count=lvtimes;
	  do//将数据X升序排列
		{	
			t1=0;		  
			for(t=0;t<count-1;t++)
			{
				if(aver[0][t]>aver[0][t+1])//升序排列
				{
					temp1=aver[0][t+1];
					aver[0][t+1]=aver[0][t];
					aver[0][t]=temp1;
					t1=1; 
				}  

				if(aver[1][t]>aver[1][t+1])//升序排列
				{
					temp2=aver[1][t+1];
					aver[1][t+1]=aver[1][t];
					aver[1][t]=temp2;
					t1=1; 
				} 

				if(aver[2][t]>aver[2][t+1])//升序排列
				{
					temp3=aver[2][t+1];
					aver[2][t+1]=aver[2][t];
					aver[2][t]=temp3;
					t1=1; 
				} 
			}
		}while(t1);   
//	//去掉最大和最小4个数
//		max[0]=max[0]-(aver[0][0]+aver[0][1]+aver[0][2] +aver[0][3]   
//		       +aver[0][lvtimes-1]+aver[0][lvtimes-2]+aver[0][lvtimes-3]+aver[0][lvtimes-4]);
//			
//		max[1]=max[1]-(aver[1][0]+aver[1][1]+aver[1][2]+aver[1][3]
//				   +aver[1][lvtimes-1]+aver[1][lvtimes-2]+aver[1][lvtimes-3]+aver[1][lvtimes-4]);

//		max[2]=max[2]-(aver[2][0]+aver[2][1]+aver[2][2]+aver[2][3]
//				   +aver[2][lvtimes-1]+aver[2][lvtimes-2]+aver[2][lvtimes-3]+aver[2][lvtimes-4]);
    avern=0;
		for (i=0;i<lvtimes-8;i++)  //扣掉8笔数据
		{ avern+=aver[0][8-4];      //从第四笔数据开始算起
			avern+=aver[1][8-4];
			avern+=aver[2][8-4];
		}
		//avern1=max[0]/(lvtimes-8);	     avern2=max[1]/(lvtimes-8);		   avern3=max[2]/(lvtimes-8);	
		avern /=(3*(lvtimes-8));
		return (avern);
}

void Dly_1ms (u32 k)			 //延时1ms
{
  volatile u32 i,j;
  for (i=0; i<k; i++)
  {
     for (j=0; j<8010; j++);			//32
  }
}
