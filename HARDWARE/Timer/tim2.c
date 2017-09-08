#include "tim2.h"

void timer2_init(void)
{		
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef  TIM2_ICInitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM2时钟使能 

  //GPIOA1  复用功能,上拉
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2); //GPIOA6复用为TIM13
	
	TIM_TimeBaseStructure.TIM_Prescaler=83;  ////预分频器,1M的计数频率,1us加1.	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=10000;   //设定计数器自动重装值 最大10ms溢出  
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); 
  	  
 	//初始化TIM13输入捕获参数
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM2_ICInitStructure.TIM_ICFilter = 0x03;//IC1F=0003 8个定时器时钟周期滤波
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);//初始化定时器2输入捕获通道
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
  TIM_Cmd(TIM2,ENABLE ); 	 	//使能定时器1
 
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//初始化NVIC寄存器

}

u8 	RmtSta_tim2=0;	  	  
u16 Dval_tim2;		//下降沿时计数器的值
u32 RmtRec_tim2=0;	//红外接收到的数据	   		    
u8  RmtCnt_tim2=0;	//按键按下的次数	 

u32 Remote_Scan_tim2(void)
{        
	u8 sta=0;       
    u8 t1,t2;  
	u32 temp_rmt_rec = 0;
	if(RmtSta_tim2&(1<<6))//得到一个按键的所有信息了
	{ 
	    t1=RmtRec_tim2>>24;			//得到地址码
	    t2=(RmtRec_tim2>>16)&0xff;	//得到地址反码 
// 	    if((t1==(u8)~t2)&&t1==REMOTE_ID)//检验遥控识别码(ID)及地址 
		if((t1 & 0xFFFFFF00) == 0x00)
		{ 
	        t1=RmtRec_tim2>>8;
	        t2=RmtRec_tim2; 	
//	        if(t1==(u8)~t2)
//						sta=t1;//键值正确	 
					sta = RmtRec_tim2 & (0xFF);
					temp_rmt_rec = RmtRec_tim2;
		}   
//		if((sta==0)||((RmtSta_tim1&0X80)==0))//按键数据错误/遥控已经没有按下了
		{
		 	RmtSta_tim2&=~(1<<6);//清除接收到有效按键标识
			RmtCnt_tim2=0;		//清除按键次数计数器
		}
	}  
    return temp_rmt_rec;
}
