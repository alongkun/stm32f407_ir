#include "tim10.h"

void timer10_init(void)
{		
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef  TIM10_ICInitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);//TIM13ʱ��ʹ�� 

  //GPIOA6  ���ù���,����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM10); //GPIOA6����ΪTIM13
	
	TIM_TimeBaseStructure.TIM_Prescaler=167;  ////Ԥ��Ƶ��,1M�ļ���Ƶ��,1us��1.	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=10000;   //�趨�������Զ���װֵ ���10ms���  
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseStructure); 
  	  
 	//��ʼ��TIM13���벶�����
	TIM10_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM10_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM10_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM10_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM10_ICInitStructure.TIM_ICFilter = 0x03;//IC1F=0003 8����ʱ��ʱ�������˲�
  TIM_ICInit(TIM10, &TIM10_ICInitStructure);//��ʼ����ʱ��2���벶��ͨ��
	
	TIM_ITConfig(TIM10,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
  TIM_Cmd(TIM10,ENABLE ); 	 	//ʹ�ܶ�ʱ��1
 
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//��ʼ��NVIC�Ĵ���

}

u8 	RmtSta_tim10=0;	  	  
u16 Dval_tim10;		//�½���ʱ��������ֵ
u32 RmtRec_tim10=0;	//������յ�������	   		    
u8  RmtCnt_tim10=0;	//�������µĴ���	 

u32 Remote_Scan_tim10(void)
{        
	u8 sta=0;       
    u8 t1,t2;  
	u32 temp_rmt_rec = 0;
	if(RmtSta_tim10&(1<<6))//�õ�һ��������������Ϣ��
	{ 
	    t1=RmtRec_tim10>>24;			//�õ���ַ��
	    t2=(RmtRec_tim10>>16)&0xff;	//�õ���ַ���� 
// 	    if((t1==(u8)~t2)&&t1==REMOTE_ID)//����ң��ʶ����(ID)����ַ 
		if((t1 & 0xFFFFFF00) == 0x00)
		{ 
	        t1=RmtRec_tim10>>8;
	        t2=RmtRec_tim10; 	
//	        if(t1==(u8)~t2)
//						sta=t1;//��ֵ��ȷ	 
					sta = RmtRec_tim10 & (0xFF);
					temp_rmt_rec = RmtRec_tim10;
		}   
//		if((sta==0)||((RmtSta_tim1&0X80)==0))//�������ݴ���/ң���Ѿ�û�а�����
		{
		 	RmtSta_tim10&=~(1<<6);//������յ���Ч������ʶ
			RmtCnt_tim10=0;		//�����������������
		}
	}  
    return temp_rmt_rec;
}







