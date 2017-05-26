//KeyFlagPause=255;  //PIC_NUM=12;   //在画面循环处此变量要重新赋值，否则容易出现空画面
void PassKey(u16 dlytime)
/***************************************************************************
* dlytime 被用于DelayMs(dlytime);, 以毫秒单位延时
*此函数需使用全局变量KeyFlagPause，此全局变量被赋予初值255;此时另控制画面显示的全局变量pic_num被自动加1
*KEYPAUSE 按下后KeyFlagPause被翻转为0,picnum 由 KEY_UP KEY_DOWN 控制增减
*BUG： KEYPAUSE需要温柔按下。解决办法：后续KEYPAUSE按下后该由终端处理
**************************************************************************/
{ 
	u16 mm1,mm2,mm3;
	//mm1=mm2=mm3=1;    //進入待機模式			
  mm1=KEY_UP;   //前進
  mm2=KEY_DOWN;   //後退
  mm3=KEY_PAUSE;    //進入待機模式
	//KeyFlagPause=0;
	//DelayMs(50);
	
	 while((mm1!=0)&&(mm2!=0)&&(mm3!=0))  //按键未按下时，始终读取按键状态
	 {
		mm1=KEY_DOWN;	
		mm2=KEY_UP;
		mm3=KEY_PAUSE;		
   //^^^^^^^^^^^^^^^^^^^^^
		 if( KeyFlagPause>0)
			{ 
				//KeyFlagPause=1;
		    DelayMs(dlytime);	
				pic_num++;	 
				if(pic_num>PIC_NUM)
				{
					pic_num=1;
				}
				break ;
			}	
   //^^^^^^^^^^^^^^^^^^^^			&(KeyFlagPause==0)
	 }
	 if(mm3==0)
		{   KeyFlagPause=~KeyFlagPause;}
		
	else if((mm1==0)&(KeyFlagPause==0))
			 {			 
				pic_num++;	 
				if(pic_num>PIC_NUM)
				{
					pic_num=1;
				}
			 }
	else if((mm2==0)&(KeyFlagPause==0))
			{	
				pic_num--;	   
				if(pic_num==0)
				{
					pic_num=PIC_NUM;
				}
			}
//	else if(mm3==0)
//			{   KeyFlagPause=~KeyFlagPause;
////				enter_sleep_mode();
////				DelayMs(1000);
////					while(Measure_IDD(uA) > IDD_SLEEP_LIMIT_VALUE)
////					{
////						BEEP_Dudu();
////					}
////					while(Measure_IDDIO(uA) > IOIDD_SLEEP_LIMIT_VALUE)
////					{
////						BEEP_Dudu();
////					}
////				mm3=1;				
////				while(mm3!=0)
////				{
////					mm3=KEY_PAUSE;
////				}		
////				exit_sleep_mode();
////				DelayMs(50); 
//			}	
  
			
//	while(mm1==0||mm2==0||mm3==0)
//	{
//		mm1=KEY_DOWN;
//		mm2=KEY_UP;
//		mm3=KEY_PAUSE;
//	}	
//这几句是陈峰写的，看不懂，故屏蔽

	
	DelayMs(1);		 

}  