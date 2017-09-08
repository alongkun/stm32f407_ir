#include "timer_interrupt.h"

//----------------------TIM13-------------------------

extern u8 	RmtSta_tim13;	  	  
extern u16 Dval_tim13;		//下降沿时计数器的值
extern u32 RmtRec_tim13;	//红外接收到的数据	   		    
extern u8  RmtCnt_tim13;	//按键按下的次数	 

void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM13,TIM_IT_Update)==SET) //溢出中断
	{
		if(RmtSta_tim13&0x80)//上次有数据被接收到了
		{	
			RmtSta_tim13&=~0X10;						//取消上升沿已经被捕获标记
			if((RmtSta_tim13&0X0F)==0X00)RmtSta_tim13|=1<<6;//标记已经完成一次按键的键值信息采集
			if((RmtSta_tim13&0X0F)<14)RmtSta_tim13++;
			else
			{
				RmtSta_tim13&=~(1<<7);//清空引导标识
				RmtSta_tim13&=0XF0;	//清空计数器	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM13,TIM_IT_Update);  //清除中断标志位 				    
	}

	if(TIM_GetITStatus(TIM13,TIM_IT_CC1)==SET) //处理捕获(CC1IE)中断
	{	  
		if(RDATA_TIM13)//上升沿捕获
		{
			TIM_OC1PolarityConfig(TIM13,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			TIM_SetCounter(TIM13,0);	   	//清空定时器值
			RmtSta_tim13|=0X10;					//标记上升沿已经被捕获
		}else //下降沿捕获
		{
			Dval_tim13=TIM_GetCapture1(TIM13);//读取CCR1也可以清CC1IF标志位
			TIM_OC1PolarityConfig(TIM13,TIM_ICPolarity_Rising); //CC1P=0	设置为上升沿捕获
			if(RmtSta_tim13&0X10)					//完成一次高电平捕获 
			{
 				if(RmtSta_tim13&0X80)//接收到了引导码
				{
					
					if(Dval_tim13>300&&Dval_tim13<800)			//560为标准值,560us
					{
						RmtRec_tim13<<=1;	//左移一位.
						RmtRec_tim13|=0;	//接收到0	   
					}else if(Dval_tim13>1400&&Dval_tim13<1800)	//1680为标准值,1680us
					{
						RmtRec_tim13<<=1;	//左移一位.
						RmtRec_tim13|=1;	//接收到1
					}else if(Dval_tim13>2200&&Dval_tim13<2600)	//得到按键键值增加的信息 2500为标准值2.5ms
					{
						RmtCnt_tim13++; 		//按键次数增加1次
						RmtSta_tim13&=0XF0;	//清空计时器		
					}
 				}else if(Dval_tim13>4200&&Dval_tim13<4700)		//4500为标准值4.5ms
				{
					RmtSta_tim13|=1<<7;	//标记成功接收到了引导码
					RmtCnt_tim13=0;		//清除按键次数计数器
				}						 
			}
			RmtSta_tim13&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM13,TIM_IT_CC1);  //清除中断标志位 		 		     	    					   
	}

}

// ----------------------TIM2-------------------------
extern u8 	RmtSta_tim2;	  	  
extern u16 Dval_tim2;		//下降沿时计数器的值
extern u32 RmtRec_tim2;	//红外接收到的数据	   		    
extern u8  RmtCnt_tim2;	//按键按下的次数	 

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		if(RmtSta_tim2&0x80)//上次有数据被接收到了
		{	
			RmtSta_tim2&=~0X10;						//取消上升沿已经被捕获标记
			if((RmtSta_tim2&0X0F)==0X00)RmtSta_tim2|=1<<6;//标记已经完成一次按键的键值信息采集
			if((RmtSta_tim2&0X0F)<14)RmtSta_tim2++;
			else
			{
				RmtSta_tim2&=~(1<<7);//清空引导标识
				RmtSta_tim2&=0XF0;	//清空计数器	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位 				    
	}

	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)==SET) //处理捕获(CC1IE)中断
	{	  
		if(RDATA_TIM2)//上升沿捕获
		{
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			TIM_SetCounter(TIM2,0);	   	//清空定时器值
			RmtSta_tim2|=0X10;					//标记上升沿已经被捕获
		}else //下降沿捕获
		{
			Dval_tim2=TIM_GetCapture2(TIM2);//读取CCR1也可以清CC1IF标志位
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0	设置为上升沿捕获
			if(RmtSta_tim2&0X10)					//完成一次高电平捕获 
			{
 				if(RmtSta_tim2&0X80)//接收到了引导码
				{
					
					if(Dval_tim2>300&&Dval_tim2<800)			//560为标准值,560us
					{
						RmtRec_tim2<<=1;	//左移一位.
						RmtRec_tim2|=0;	//接收到0	   
					}else if(Dval_tim2>1400&&Dval_tim2<1800)	//1680为标准值,1680us
					{
						RmtRec_tim2<<=1;	//左移一位.
						RmtRec_tim2|=1;	//接收到1
					}else if(Dval_tim2>2200&&Dval_tim2<2600)	//得到按键键值增加的信息 2500为标准值2.5ms
					{
						RmtCnt_tim2++; 		//按键次数增加1次
						RmtSta_tim2&=0XF0;	//清空计时器		
					}
 				}else if(Dval_tim2>4200&&Dval_tim2<4700)		//4500为标准值4.5ms
				{
					RmtSta_tim2|=1<<7;	//标记成功接收到了引导码
					RmtCnt_tim2=0;		//清除按键次数计数器
				}						 
			}
			RmtSta_tim2&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);  //清除中断标志位 		 		     	    					   
	}

}

//---------------------------TIM9----------------------------
extern u8  RmtSta_tim9;	  	  
extern u16 Dval_tim9;		//下降沿时计数器的值
extern u32 RmtRec_tim9;	//红外接收到的数据	   		    
extern u8  RmtCnt_tim9;	//按键按下的次数	 

void TIM1_BRK_TIM9_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM9,TIM_IT_Update)==SET) //溢出中断
	{
		if(RmtSta_tim9&0x80)//上次有数据被接收到了
		{	
			RmtSta_tim9&=~0X10;						//取消上升沿已经被捕获标记
			if((RmtSta_tim9&0X0F)==0X00)RmtSta_tim9|=1<<6;//标记已经完成一次按键的键值信息采集
			if((RmtSta_tim9&0X0F)<14)RmtSta_tim9++;
			else
			{
				RmtSta_tim9&=~(1<<7);//清空引导标识
				RmtSta_tim9&=0XF0;	//清空计数器	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM9,TIM_IT_Update);  //清除中断标志位 				    
	}

	if(TIM_GetITStatus(TIM9,TIM_IT_CC1)==SET) //处理捕获(CC1IE)中断
	{	  
		if(RDATA_TIM9)//上升沿捕获
		{
			TIM_OC1PolarityConfig(TIM9,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			TIM_SetCounter(TIM9,0);	   	//清空定时器值
			RmtSta_tim9|=0X10;					//标记上升沿已经被捕获
		}else //下降沿捕获
		{
			Dval_tim9=TIM_GetCapture1(TIM9);//读取CCR1也可以清CC1IF标志位
			TIM_OC1PolarityConfig(TIM9,TIM_ICPolarity_Rising); //CC1P=0	设置为上升沿捕获
			if(RmtSta_tim9&0X10)					//完成一次高电平捕获 
			{
 				if(RmtSta_tim9&0X80)//接收到了引导码
				{
					
					if(Dval_tim9>300&&Dval_tim9<800)			//560为标准值,560us
					{
						RmtRec_tim9<<=1;	//左移一位.
						RmtRec_tim9|=0;	//接收到0	   
					}else if(Dval_tim9>1400&&Dval_tim9<1800)	//1680为标准值,1680us
					{
						RmtRec_tim9<<=1;	//左移一位.
						RmtRec_tim9|=1;	//接收到1
					}else if(Dval_tim9>2200&&Dval_tim9<2600)	//得到按键键值增加的信息 2500为标准值2.5ms
					{
						RmtCnt_tim9++; 		//按键次数增加1次
						RmtSta_tim9&=0XF0;	//清空计时器		
					}
 				}else if(Dval_tim9>4200&&Dval_tim9<4700)		//4500为标准值4.5ms
				{
					RmtSta_tim9|=1<<7;	//标记成功接收到了引导码
					RmtCnt_tim9=0;		//清除按键次数计数器
				}						 
			}
			RmtSta_tim9&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM9,TIM_IT_CC1);  //清除中断标志位 		 		     	    					   
	}
}

//----------------------TIM10--------------------------

extern u8  RmtSta_tim10;	  	  
extern u16 Dval_tim10;		//下降沿时计数器的值
extern u32 RmtRec_tim10;	//红外接收到的数据	   		    
extern u8  RmtCnt_tim10;	//按键按下的次数	 

void TIM1_UP_TIM10_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET) //溢出中断
	{
		if(RmtSta_tim10&0x80)//上次有数据被接收到了
		{	
			RmtSta_tim10&=~0X10;						//取消上升沿已经被捕获标记
			if((RmtSta_tim10&0X0F)==0X00)RmtSta_tim10|=1<<6;//标记已经完成一次按键的键值信息采集
			if((RmtSta_tim10&0X0F)<14)RmtSta_tim10++;
			else
			{
				RmtSta_tim10&=~(1<<7);//清空引导标识
				RmtSta_tim10&=0XF0;	//清空计数器	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM10,TIM_IT_Update);  //清除中断标志位 				    
	}

	if(TIM_GetITStatus(TIM10,TIM_IT_CC1)==SET) //处理捕获(CC1IE)中断
	{	  
		if(RDATA_TIM10)//上升沿捕获
		{
			TIM_OC1PolarityConfig(TIM10,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			TIM_SetCounter(TIM10,0);	   	//清空定时器值
			RmtSta_tim10|=0X10;					//标记上升沿已经被捕获
		}else //下降沿捕获
		{
			Dval_tim10=TIM_GetCapture1(TIM10);//读取CCR1也可以清CC1IF标志位
			TIM_OC1PolarityConfig(TIM10,TIM_ICPolarity_Rising); //CC1P=0	设置为上升沿捕获
			if(RmtSta_tim10&0X10)					//完成一次高电平捕获 
			{
 				if(RmtSta_tim10&0X80)//接收到了引导码
				{
					
					if(Dval_tim10>300&&Dval_tim10<800)			//560为标准值,560us
					{
						RmtRec_tim10<<=1;	//左移一位.
						RmtRec_tim10|=0;	//接收到0	   
					}else if(Dval_tim10>1400&&Dval_tim10<1800)	//1680为标准值,1680us
					{
						RmtRec_tim10<<=1;	//左移一位.
						RmtRec_tim10|=1;	//接收到1
					}else if(Dval_tim10>2200&&Dval_tim10<2600)	//得到按键键值增加的信息 2500为标准值2.5ms
					{
						RmtCnt_tim10++; 		//按键次数增加1次
						RmtSta_tim10&=0XF0;	//清空计时器		
					}
 				}else if(Dval_tim10>4200&&Dval_tim10<4700)		//4500为标准值4.5ms
				{
					RmtSta_tim10|=1<<7;	//标记成功接收到了引导码
					RmtCnt_tim10=0;		//清除按键次数计数器
				}						 
			}
			RmtSta_tim10&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM10,TIM_IT_CC1);  //清除中断标志位 		 		     	    					   
	}
}

//----------------------TIM11--------------------------

extern u8  RmtSta_tim11;	  	  
extern u16 Dval_tim11;		//下降沿时计数器的值
extern u32 RmtRec_tim11;	//红外接收到的数据	   		    
extern u8  RmtCnt_tim11;	//按键按下的次数	 

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM11,TIM_IT_Update)==SET) //溢出中断
	{
		if(RmtSta_tim11&0x80)//上次有数据被接收到了
		{	
			RmtSta_tim11&=~0X10;						//取消上升沿已经被捕获标记
			if((RmtSta_tim11&0X0F)==0X00)RmtSta_tim11|=1<<6;//标记已经完成一次按键的键值信息采集
			if((RmtSta_tim11&0X0F)<14)RmtSta_tim11++;
			else
			{
				RmtSta_tim11&=~(1<<7);//清空引导标识
				RmtSta_tim11&=0XF0;	//清空计数器	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM11,TIM_IT_Update);  //清除中断标志位 				    
	}

	if(TIM_GetITStatus(TIM11,TIM_IT_CC1)==SET) //处理捕获(CC1IE)中断
	{	  
		if(RDATA_TIM11)//上升沿捕获
		{
			TIM_OC1PolarityConfig(TIM11,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			TIM_SetCounter(TIM11,0);	   	//清空定时器值
			RmtSta_tim11|=0X10;					//标记上升沿已经被捕获
		}else //下降沿捕获
		{
			Dval_tim11=TIM_GetCapture1(TIM11);//读取CCR1也可以清CC1IF标志位
			TIM_OC1PolarityConfig(TIM11,TIM_ICPolarity_Rising); //CC1P=0	设置为上升沿捕获
			if(RmtSta_tim11&0X10)					//完成一次高电平捕获 
			{
 				if(RmtSta_tim11&0X80)//接收到了引导码
				{
					
					if(Dval_tim11>300&&Dval_tim11<800)			//560为标准值,560us
					{
						RmtRec_tim11<<=1;	//左移一位.
						RmtRec_tim11|=0;	//接收到0	   
					}else if(Dval_tim11>1400&&Dval_tim11<1800)	//1680为标准值,1680us
					{
						RmtRec_tim11<<=1;	//左移一位.
						RmtRec_tim11|=1;	//接收到1
					}else if(Dval_tim11>2200&&Dval_tim11<2600)	//得到按键键值增加的信息 2500为标准值2.5ms
					{
						RmtCnt_tim11++; 		//按键次数增加1次
						RmtSta_tim11&=0XF0;	//清空计时器		
					}
 				}else if(Dval_tim11>4200&&Dval_tim11<4700)		//4500为标准值4.5ms
				{
					RmtSta_tim11|=1<<7;	//标记成功接收到了引导码
					RmtCnt_tim11=0;		//清除按键次数计数器
				}						 
			}
			RmtSta_tim11&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM11,TIM_IT_CC1);  //清除中断标志位 		 		     	    					   
	}
}

//----------------------TIM14--------------------------

extern u8  RmtSta_tim14;	  	  
extern u16 Dval_tim14;		//下降沿时计数器的值
extern u32 RmtRec_tim14;	//红外接收到的数据	   		    
extern u8  RmtCnt_tim14;	//按键按下的次数	 

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //溢出中断
	{
		if(RmtSta_tim14&0x80)//上次有数据被接收到了
		{	
			RmtSta_tim14&=~0X10;						//取消上升沿已经被捕获标记
			if((RmtSta_tim14&0X0F)==0X00)RmtSta_tim14|=1<<6;//标记已经完成一次按键的键值信息采集
			if((RmtSta_tim14&0X0F)<14)RmtSta_tim14++;
			else
			{
				RmtSta_tim14&=~(1<<7);//清空引导标识
				RmtSta_tim14&=0XF0;	//清空计数器	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //清除中断标志位 				    
	}

	if(TIM_GetITStatus(TIM14,TIM_IT_CC1)==SET) //处理捕获(CC1IE)中断
	{	  
		if(RDATA_TIM14)//上升沿捕获
		{
			TIM_OC1PolarityConfig(TIM14,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			TIM_SetCounter(TIM14,0);	   	//清空定时器值
			RmtSta_tim14|=0X10;					//标记上升沿已经被捕获
		}else //下降沿捕获
		{
			Dval_tim14=TIM_GetCapture1(TIM14);//读取CCR1也可以清CC1IF标志位
			TIM_OC1PolarityConfig(TIM14,TIM_ICPolarity_Rising); //CC1P=0	设置为上升沿捕获
			if(RmtSta_tim14&0X10)					//完成一次高电平捕获 
			{
 				if(RmtSta_tim14&0X80)//接收到了引导码
				{
					
					if(Dval_tim14>300&&Dval_tim14<800)			//560为标准值,560us
					{
						RmtRec_tim14<<=1;	//左移一位.
						RmtRec_tim14|=0;	//接收到0	   
					}else if(Dval_tim14>1400&&Dval_tim14<1800)	//1680为标准值,1680us
					{
						RmtRec_tim14<<=1;	//左移一位.
						RmtRec_tim14|=1;	//接收到1
					}else if(Dval_tim14>2200&&Dval_tim14<2600)	//得到按键键值增加的信息 2500为标准值2.5ms
					{
						RmtCnt_tim14++; 		//按键次数增加1次
						RmtSta_tim14&=0XF0;	//清空计时器		
					}
 				}else if(Dval_tim14>4200&&Dval_tim14<4700)		//4500为标准值4.5ms
				{
					RmtSta_tim14|=1<<7;	//标记成功接收到了引导码
					RmtCnt_tim14=0;		//清除按键次数计数器
				}						 
			}
			RmtSta_tim14&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM14,TIM_IT_CC1);  //清除中断标志位 		 		     	    					   
	}
}



