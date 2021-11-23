#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"
#include "clk.h"
#include "sys.h"
#include "systick.h"
#include "usart.h"
#include "timer2.h"
#include "timer3.h"
#include "timer13.h"
//#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
//#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */

int main()
{
	int i, j, k;
	// 设置中断
	DPZ_NVIC_Init( 1 );

	// 设置时钟
	DPZ_SysPLL( 1, 8, 336, 2, 7, 0, 5, 4 );

	// 初始化LED
	DPZ_LED_Init();

	// 初始化串口
	DPZ_USART_Init( 1, 115200, 0);
	DPZ_USART_Init( 2, 115200, 0);

	// PWM模式
	DPZ_Timer13_PWM( 100 );

	i = 0;
	k = j = 1;
	while ( 1 )
	{
		if ( !( i & 1 ) )
		{
			DPZ_IO_T( GPIOA, 7 );
			DPZ_USART1_Send( ( unsigned char* )"1LED is running\r\n", 0 );
			DPZ_SysTick_Sleep( 500 );
		}
		else 
		{
			DPZ_IO_T( GPIOA, 6 );
			DPZ_USART1_Send( ( unsigned char* )"2LED is running\r\n", 0 );
			DPZ_SysTick_Sleep( 500);
		}
		if ( ++i == 64 )
		{
			k = -k;
			i = 0;
		}

		j += k;
		DPZ_Timer13_CCR( j );

	}
}
