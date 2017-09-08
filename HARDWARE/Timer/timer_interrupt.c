#include "timer_interrupt.h"

//----------------------TIM13-------------------------

extern u8 	RmtSta_tim13;	  	  
extern u16 Dval_tim13;		//�½���ʱ��������ֵ
extern u32 RmtRec_tim13;	//������յ�������	   		    
extern u8  RmtCnt_tim13;	//�������µĴ���	 

void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM13,TIM_IT_Update)==SET) //����ж�
	{
		if(RmtSta_tim13&0x80)//�ϴ������ݱ����յ���
		{	
			RmtSta_tim13&=~0X10;						//ȡ���������Ѿ���������
			if((RmtSta_tim13&0X0F)==0X00)RmtSta_tim13|=1<<6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
			if((RmtSta_tim13&0X0F)<14)RmtSta_tim13++;
			else
			{
				RmtSta_tim13&=~(1<<7);//���������ʶ
				RmtSta_tim13&=0XF0;	//��ռ�����	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM13,TIM_IT_Update);  //����жϱ�־λ 				    
	}

	if(TIM_GetITStatus(TIM13,TIM_IT_CC1)==SET) //������(CC1IE)�ж�
	{	  
		if(RDATA_TIM13)//�����ز���
		{
			TIM_OC1PolarityConfig(TIM13,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			TIM_SetCounter(TIM13,0);	   	//��ն�ʱ��ֵ
			RmtSta_tim13|=0X10;					//����������Ѿ�������
		}else //�½��ز���
		{
			Dval_tim13=TIM_GetCapture1(TIM13);//��ȡCCR1Ҳ������CC1IF��־λ
			TIM_OC1PolarityConfig(TIM13,TIM_ICPolarity_Rising); //CC1P=0	����Ϊ�����ز���
			if(RmtSta_tim13&0X10)					//���һ�θߵ�ƽ���� 
			{
 				if(RmtSta_tim13&0X80)//���յ���������
				{
					
					if(Dval_tim13>300&&Dval_tim13<800)			//560Ϊ��׼ֵ,560us
					{
						RmtRec_tim13<<=1;	//����һλ.
						RmtRec_tim13|=0;	//���յ�0	   
					}else if(Dval_tim13>1400&&Dval_tim13<1800)	//1680Ϊ��׼ֵ,1680us
					{
						RmtRec_tim13<<=1;	//����һλ.
						RmtRec_tim13|=1;	//���յ�1
					}else if(Dval_tim13>2200&&Dval_tim13<2600)	//�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
					{
						RmtCnt_tim13++; 		//������������1��
						RmtSta_tim13&=0XF0;	//��ռ�ʱ��		
					}
 				}else if(Dval_tim13>4200&&Dval_tim13<4700)		//4500Ϊ��׼ֵ4.5ms
				{
					RmtSta_tim13|=1<<7;	//��ǳɹ����յ���������
					RmtCnt_tim13=0;		//�����������������
				}						 
			}
			RmtSta_tim13&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM13,TIM_IT_CC1);  //����жϱ�־λ 		 		     	    					   
	}

}

// ----------------------TIM2-------------------------
extern u8 	RmtSta_tim2;	  	  
extern u16 Dval_tim2;		//�½���ʱ��������ֵ
extern u32 RmtRec_tim2;	//������յ�������	   		    
extern u8  RmtCnt_tim2;	//�������µĴ���	 

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
	{
		if(RmtSta_tim2&0x80)//�ϴ������ݱ����յ���
		{	
			RmtSta_tim2&=~0X10;						//ȡ���������Ѿ���������
			if((RmtSta_tim2&0X0F)==0X00)RmtSta_tim2|=1<<6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
			if((RmtSta_tim2&0X0F)<14)RmtSta_tim2++;
			else
			{
				RmtSta_tim2&=~(1<<7);//���������ʶ
				RmtSta_tim2&=0XF0;	//��ռ�����	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ 				    
	}

	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)==SET) //������(CC1IE)�ж�
	{	  
		if(RDATA_TIM2)//�����ز���
		{
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			TIM_SetCounter(TIM2,0);	   	//��ն�ʱ��ֵ
			RmtSta_tim2|=0X10;					//����������Ѿ�������
		}else //�½��ز���
		{
			Dval_tim2=TIM_GetCapture2(TIM2);//��ȡCCR1Ҳ������CC1IF��־λ
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0	����Ϊ�����ز���
			if(RmtSta_tim2&0X10)					//���һ�θߵ�ƽ���� 
			{
 				if(RmtSta_tim2&0X80)//���յ���������
				{
					
					if(Dval_tim2>300&&Dval_tim2<800)			//560Ϊ��׼ֵ,560us
					{
						RmtRec_tim2<<=1;	//����һλ.
						RmtRec_tim2|=0;	//���յ�0	   
					}else if(Dval_tim2>1400&&Dval_tim2<1800)	//1680Ϊ��׼ֵ,1680us
					{
						RmtRec_tim2<<=1;	//����һλ.
						RmtRec_tim2|=1;	//���յ�1
					}else if(Dval_tim2>2200&&Dval_tim2<2600)	//�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
					{
						RmtCnt_tim2++; 		//������������1��
						RmtSta_tim2&=0XF0;	//��ռ�ʱ��		
					}
 				}else if(Dval_tim2>4200&&Dval_tim2<4700)		//4500Ϊ��׼ֵ4.5ms
				{
					RmtSta_tim2|=1<<7;	//��ǳɹ����յ���������
					RmtCnt_tim2=0;		//�����������������
				}						 
			}
			RmtSta_tim2&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);  //����жϱ�־λ 		 		     	    					   
	}

}

//---------------------------TIM9----------------------------
extern u8  RmtSta_tim9;	  	  
extern u16 Dval_tim9;		//�½���ʱ��������ֵ
extern u32 RmtRec_tim9;	//������յ�������	   		    
extern u8  RmtCnt_tim9;	//�������µĴ���	 

void TIM1_BRK_TIM9_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM9,TIM_IT_Update)==SET) //����ж�
	{
		if(RmtSta_tim9&0x80)//�ϴ������ݱ����յ���
		{	
			RmtSta_tim9&=~0X10;						//ȡ���������Ѿ���������
			if((RmtSta_tim9&0X0F)==0X00)RmtSta_tim9|=1<<6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
			if((RmtSta_tim9&0X0F)<14)RmtSta_tim9++;
			else
			{
				RmtSta_tim9&=~(1<<7);//���������ʶ
				RmtSta_tim9&=0XF0;	//��ռ�����	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM9,TIM_IT_Update);  //����жϱ�־λ 				    
	}

	if(TIM_GetITStatus(TIM9,TIM_IT_CC1)==SET) //������(CC1IE)�ж�
	{	  
		if(RDATA_TIM9)//�����ز���
		{
			TIM_OC1PolarityConfig(TIM9,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			TIM_SetCounter(TIM9,0);	   	//��ն�ʱ��ֵ
			RmtSta_tim9|=0X10;					//����������Ѿ�������
		}else //�½��ز���
		{
			Dval_tim9=TIM_GetCapture1(TIM9);//��ȡCCR1Ҳ������CC1IF��־λ
			TIM_OC1PolarityConfig(TIM9,TIM_ICPolarity_Rising); //CC1P=0	����Ϊ�����ز���
			if(RmtSta_tim9&0X10)					//���һ�θߵ�ƽ���� 
			{
 				if(RmtSta_tim9&0X80)//���յ���������
				{
					
					if(Dval_tim9>300&&Dval_tim9<800)			//560Ϊ��׼ֵ,560us
					{
						RmtRec_tim9<<=1;	//����һλ.
						RmtRec_tim9|=0;	//���յ�0	   
					}else if(Dval_tim9>1400&&Dval_tim9<1800)	//1680Ϊ��׼ֵ,1680us
					{
						RmtRec_tim9<<=1;	//����һλ.
						RmtRec_tim9|=1;	//���յ�1
					}else if(Dval_tim9>2200&&Dval_tim9<2600)	//�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
					{
						RmtCnt_tim9++; 		//������������1��
						RmtSta_tim9&=0XF0;	//��ռ�ʱ��		
					}
 				}else if(Dval_tim9>4200&&Dval_tim9<4700)		//4500Ϊ��׼ֵ4.5ms
				{
					RmtSta_tim9|=1<<7;	//��ǳɹ����յ���������
					RmtCnt_tim9=0;		//�����������������
				}						 
			}
			RmtSta_tim9&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM9,TIM_IT_CC1);  //����жϱ�־λ 		 		     	    					   
	}
}

//----------------------TIM10--------------------------

extern u8  RmtSta_tim10;	  	  
extern u16 Dval_tim10;		//�½���ʱ��������ֵ
extern u32 RmtRec_tim10;	//������յ�������	   		    
extern u8  RmtCnt_tim10;	//�������µĴ���	 

void TIM1_UP_TIM10_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET) //����ж�
	{
		if(RmtSta_tim10&0x80)//�ϴ������ݱ����յ���
		{	
			RmtSta_tim10&=~0X10;						//ȡ���������Ѿ���������
			if((RmtSta_tim10&0X0F)==0X00)RmtSta_tim10|=1<<6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
			if((RmtSta_tim10&0X0F)<14)RmtSta_tim10++;
			else
			{
				RmtSta_tim10&=~(1<<7);//���������ʶ
				RmtSta_tim10&=0XF0;	//��ռ�����	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM10,TIM_IT_Update);  //����жϱ�־λ 				    
	}

	if(TIM_GetITStatus(TIM10,TIM_IT_CC1)==SET) //������(CC1IE)�ж�
	{	  
		if(RDATA_TIM10)//�����ز���
		{
			TIM_OC1PolarityConfig(TIM10,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			TIM_SetCounter(TIM10,0);	   	//��ն�ʱ��ֵ
			RmtSta_tim10|=0X10;					//����������Ѿ�������
		}else //�½��ز���
		{
			Dval_tim10=TIM_GetCapture1(TIM10);//��ȡCCR1Ҳ������CC1IF��־λ
			TIM_OC1PolarityConfig(TIM10,TIM_ICPolarity_Rising); //CC1P=0	����Ϊ�����ز���
			if(RmtSta_tim10&0X10)					//���һ�θߵ�ƽ���� 
			{
 				if(RmtSta_tim10&0X80)//���յ���������
				{
					
					if(Dval_tim10>300&&Dval_tim10<800)			//560Ϊ��׼ֵ,560us
					{
						RmtRec_tim10<<=1;	//����һλ.
						RmtRec_tim10|=0;	//���յ�0	   
					}else if(Dval_tim10>1400&&Dval_tim10<1800)	//1680Ϊ��׼ֵ,1680us
					{
						RmtRec_tim10<<=1;	//����һλ.
						RmtRec_tim10|=1;	//���յ�1
					}else if(Dval_tim10>2200&&Dval_tim10<2600)	//�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
					{
						RmtCnt_tim10++; 		//������������1��
						RmtSta_tim10&=0XF0;	//��ռ�ʱ��		
					}
 				}else if(Dval_tim10>4200&&Dval_tim10<4700)		//4500Ϊ��׼ֵ4.5ms
				{
					RmtSta_tim10|=1<<7;	//��ǳɹ����յ���������
					RmtCnt_tim10=0;		//�����������������
				}						 
			}
			RmtSta_tim10&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM10,TIM_IT_CC1);  //����жϱ�־λ 		 		     	    					   
	}
}

//----------------------TIM11--------------------------

extern u8  RmtSta_tim11;	  	  
extern u16 Dval_tim11;		//�½���ʱ��������ֵ
extern u32 RmtRec_tim11;	//������յ�������	   		    
extern u8  RmtCnt_tim11;	//�������µĴ���	 

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM11,TIM_IT_Update)==SET) //����ж�
	{
		if(RmtSta_tim11&0x80)//�ϴ������ݱ����յ���
		{	
			RmtSta_tim11&=~0X10;						//ȡ���������Ѿ���������
			if((RmtSta_tim11&0X0F)==0X00)RmtSta_tim11|=1<<6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
			if((RmtSta_tim11&0X0F)<14)RmtSta_tim11++;
			else
			{
				RmtSta_tim11&=~(1<<7);//���������ʶ
				RmtSta_tim11&=0XF0;	//��ռ�����	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM11,TIM_IT_Update);  //����жϱ�־λ 				    
	}

	if(TIM_GetITStatus(TIM11,TIM_IT_CC1)==SET) //������(CC1IE)�ж�
	{	  
		if(RDATA_TIM11)//�����ز���
		{
			TIM_OC1PolarityConfig(TIM11,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			TIM_SetCounter(TIM11,0);	   	//��ն�ʱ��ֵ
			RmtSta_tim11|=0X10;					//����������Ѿ�������
		}else //�½��ز���
		{
			Dval_tim11=TIM_GetCapture1(TIM11);//��ȡCCR1Ҳ������CC1IF��־λ
			TIM_OC1PolarityConfig(TIM11,TIM_ICPolarity_Rising); //CC1P=0	����Ϊ�����ز���
			if(RmtSta_tim11&0X10)					//���һ�θߵ�ƽ���� 
			{
 				if(RmtSta_tim11&0X80)//���յ���������
				{
					
					if(Dval_tim11>300&&Dval_tim11<800)			//560Ϊ��׼ֵ,560us
					{
						RmtRec_tim11<<=1;	//����һλ.
						RmtRec_tim11|=0;	//���յ�0	   
					}else if(Dval_tim11>1400&&Dval_tim11<1800)	//1680Ϊ��׼ֵ,1680us
					{
						RmtRec_tim11<<=1;	//����һλ.
						RmtRec_tim11|=1;	//���յ�1
					}else if(Dval_tim11>2200&&Dval_tim11<2600)	//�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
					{
						RmtCnt_tim11++; 		//������������1��
						RmtSta_tim11&=0XF0;	//��ռ�ʱ��		
					}
 				}else if(Dval_tim11>4200&&Dval_tim11<4700)		//4500Ϊ��׼ֵ4.5ms
				{
					RmtSta_tim11|=1<<7;	//��ǳɹ����յ���������
					RmtCnt_tim11=0;		//�����������������
				}						 
			}
			RmtSta_tim11&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM11,TIM_IT_CC1);  //����жϱ�־λ 		 		     	    					   
	}
}

//----------------------TIM14--------------------------

extern u8  RmtSta_tim14;	  	  
extern u16 Dval_tim14;		//�½���ʱ��������ֵ
extern u32 RmtRec_tim14;	//������յ�������	   		    
extern u8  RmtCnt_tim14;	//�������µĴ���	 

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //����ж�
	{
		if(RmtSta_tim14&0x80)//�ϴ������ݱ����յ���
		{	
			RmtSta_tim14&=~0X10;						//ȡ���������Ѿ���������
			if((RmtSta_tim14&0X0F)==0X00)RmtSta_tim14|=1<<6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
			if((RmtSta_tim14&0X0F)<14)RmtSta_tim14++;
			else
			{
				RmtSta_tim14&=~(1<<7);//���������ʶ
				RmtSta_tim14&=0XF0;	//��ռ�����	
			}						 	   	
		}	
		TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //����жϱ�־λ 				    
	}

	if(TIM_GetITStatus(TIM14,TIM_IT_CC1)==SET) //������(CC1IE)�ж�
	{	  
		if(RDATA_TIM14)//�����ز���
		{
			TIM_OC1PolarityConfig(TIM14,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			TIM_SetCounter(TIM14,0);	   	//��ն�ʱ��ֵ
			RmtSta_tim14|=0X10;					//����������Ѿ�������
		}else //�½��ز���
		{
			Dval_tim14=TIM_GetCapture1(TIM14);//��ȡCCR1Ҳ������CC1IF��־λ
			TIM_OC1PolarityConfig(TIM14,TIM_ICPolarity_Rising); //CC1P=0	����Ϊ�����ز���
			if(RmtSta_tim14&0X10)					//���һ�θߵ�ƽ���� 
			{
 				if(RmtSta_tim14&0X80)//���յ���������
				{
					
					if(Dval_tim14>300&&Dval_tim14<800)			//560Ϊ��׼ֵ,560us
					{
						RmtRec_tim14<<=1;	//����һλ.
						RmtRec_tim14|=0;	//���յ�0	   
					}else if(Dval_tim14>1400&&Dval_tim14<1800)	//1680Ϊ��׼ֵ,1680us
					{
						RmtRec_tim14<<=1;	//����һλ.
						RmtRec_tim14|=1;	//���յ�1
					}else if(Dval_tim14>2200&&Dval_tim14<2600)	//�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
					{
						RmtCnt_tim14++; 		//������������1��
						RmtSta_tim14&=0XF0;	//��ռ�ʱ��		
					}
 				}else if(Dval_tim14>4200&&Dval_tim14<4700)		//4500Ϊ��׼ֵ4.5ms
				{
					RmtSta_tim14|=1<<7;	//��ǳɹ����յ���������
					RmtCnt_tim14=0;		//�����������������
				}						 
			}
			RmtSta_tim14&=~(1<<4);
		}	
		TIM_ClearITPendingBit(TIM14,TIM_IT_CC1);  //����жϱ�־λ 		 		     	    					   
	}
}



