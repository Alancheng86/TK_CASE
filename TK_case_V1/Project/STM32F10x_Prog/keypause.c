//KeyFlagPause=255;  //PIC_NUM=12;   //�ڻ���ѭ�����˱���Ҫ���¸�ֵ���������׳��ֿջ���
void PassKey(u16 dlytime)
/***************************************************************************
* dlytime ������DelayMs(dlytime);, �Ժ��뵥λ��ʱ
*�˺�����ʹ��ȫ�ֱ���KeyFlagPause����ȫ�ֱ����������ֵ255;��ʱ����ƻ�����ʾ��ȫ�ֱ���pic_num���Զ���1
*KEYPAUSE ���º�KeyFlagPause����תΪ0,picnum �� KEY_UP KEY_DOWN ��������
*BUG�� KEYPAUSE��Ҫ���ᰴ�¡�����취������KEYPAUSE���º�����ն˴���
**************************************************************************/
{ 
	u16 mm1,mm2,mm3;
	//mm1=mm2=mm3=1;    //�M����Cģʽ			
  mm1=KEY_UP;   //ǰ�M
  mm2=KEY_DOWN;   //����
  mm3=KEY_PAUSE;    //�M����Cģʽ
	//KeyFlagPause=0;
	//DelayMs(50);
	
	 while((mm1!=0)&&(mm2!=0)&&(mm3!=0))  //����δ����ʱ��ʼ�ն�ȡ����״̬
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
//�⼸���ǳ·�д�ģ���������������

	
	DelayMs(1);		 

}  