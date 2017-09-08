#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
//#include "lcd.h"
//#include "tim1.h"
#include "tim13.h"
#include "tim2.h"
#include "tim9.h"
#include "tim10.h"
#include "tim11.h"
#include "tim14.h"
//#include "TSL2561.h"
#include "adc.h"
#include "timer3.h"
#include "string.h"

extern u16 adc_buff[9];
extern u16 adc_buff_max[9];
extern u16 flag_1s;
extern u16 flag_500ms;

int main(void)
{ 
//	u16 tsl2561_ch1;
	u8 i;
	u8 tim_switch = 0;
	
	u32 key_temp;
	u32 key[6];
	u8 t=0;	
	u8 *str=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED
 	//LCD_Init();
 	//Remote_Init();				//红外接收初始化		 	
	/*
	timer13_init();
	timer2_init();
	timer9_init();
	timer10_init();
	timer11_init();
	timer14_init();
	*/
	adc_init();
	dma_init();
	timer3_init();
	
	//tsl2561_init();
	
// 	POINT_COLOR=RED;//设置字体为红色 
//	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
//	LCD_ShowString(30,70,200,16,16,"REMOTE TEST");	
//	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
//	LCD_ShowString(30,110,200,16,16,"2014/5/7");
//   	LCD_ShowString(30,130,200,16,16,"KEYVAL:");	
//   	LCD_ShowString(30,150,200,16,16,"KEYCNT:");	
//   	LCD_ShowString(30,170,200,16,16,"SYMBOL:");	    
 														 	  		    							  
	while(1)
	{
/*		
		key=Remote_Scan_tim1();	
		if(key)
		{	 

			//			LCD_ShowNum(86,130,key,3,16);		//显示键值
//			LCD_ShowNum(86,150,RmtCnt,3,16);	//显示按键次数		  
			switch(key)
			{
				case 0:str="ERROR";break;			   
				case 162:str="POWER";break;	    
				case 98:str="UP";break;	    
				case 2:str="PLAY";break;		 
				case 226:str="ALIENTEK";break;		  
				case 194:str="RIGHT";break;	   
				case 34:str="LEFT";break;		  
				case 224:str="VOL-";break;		  
				case 168:str="DOWN";break;		   
				case 144:str="VOL+";break;		    
				case 104:str="1";break;		  
				case 152:str="2";break;	   
				case 176:str="3";break;	    
				case 48:str="4";break;		    
				case 24:str="5";break;		    
				case 122:str="6";break;		  
				case 16:str="7";break;			   					
				case 56:str="8";break;	 
				case 90:str="9";break;
				case 66:str="0";break;
				case 82:str="DELETE";break;		 
			}

			printf("rev code : 0x%X\r\n",key);
//			LCD_Fill(86,170,116+8*8,170+16,WHITE);	//清楚之前的显示
//			LCD_ShowString(86,170,200,16,16,str);	//显示SYMBOL
		}
		else 
			delay_ms(5);	 
*/
		
/*		
		tsl2561_ch1 = tsl2561_get_ch1();
		printf("tsl2561 ch1 value : %d\r\n",tsl2561_ch1);	
*/		
		//------------------------------------
/*		
		key_temp=Remote_Scan_tim13();
		if(key_temp)
			key[0]= key_temp;

		key_temp=Remote_Scan_tim2();
		if(key_temp)
			key[1]= key_temp;

		key_temp=Remote_Scan_tim9();
		if(key_temp)
			key[2]= key_temp;
		
		key_temp=Remote_Scan_tim10();
		if(key_temp)
			key[3]= key_temp;

		key_temp=Remote_Scan_tim11();
		if(key_temp)
			key[4]= key_temp;

		key_temp=Remote_Scan_tim14();
		if(key_temp)
			key[5]= key_temp;
*/

/*
		if(flag_500ms >= 333)
		{
			flag_500ms = 0;
			
			if(tim_switch)
			{
				tim_switch = 0;
				TIM_Cmd(TIM13,ENABLE );
				TIM_Cmd(TIM2,ENABLE );
				TIM_Cmd(TIM9,ENABLE );
				TIM_Cmd(TIM10,DISABLE );
				TIM_Cmd(TIM11,DISABLE );
				TIM_Cmd(TIM14,DISABLE );
			}
			else
			{
				tim_switch = 1;
				TIM_Cmd(TIM13,DISABLE );
				TIM_Cmd(TIM2,DISABLE );
				TIM_Cmd(TIM9,DISABLE );
				TIM_Cmd(TIM10,ENABLE );
				TIM_Cmd(TIM11,ENABLE );
				TIM_Cmd(TIM14,ENABLE );
			}
		}
*/

		if(flag_1s >= 1000)		//1s
		{
			flag_1s = 0;
			LED0=!LED0;
			
			//------------------------------------
			/*
			key[0]=Remote_Scan_tim13();
			key[1]=Remote_Scan_tim2();
			key[2]=Remote_Scan_tim9();
			key[3]=Remote_Scan_tim10();
			key[4]=Remote_Scan_tim11();
			key[5]=Remote_Scan_tim14();

			for( i = 0; i < 0; i++ )
			{
				if(key[i])
					printf("decode ch%d rev code : 0x%X\r\n", i, key[i]);
				else
					printf("decode ch%d rev code : 0x00000000\r\n", i);
				
			}
			memset(key, 0, sizeof(key));
			*/
			//------------------------------------
			
			printf("----------------------\r\n");
			for( i = 0; i < 9; i++ )
				printf("adc ch%d value: %d mV\r\n", i, (u16)(adc_buff_max[i]/4096.0*3300));
			printf("----------------------\r\n");
			
			memset(adc_buff_max, 0, sizeof(adc_buff_max));
			
			//------------------------------------
		}
	}
}

