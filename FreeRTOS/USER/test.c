#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

#define START_TASK_PRIO			1
#define START_STK_SIZE			120
void start_task(void * pvParameters);  //任务函数
TaskHandle_t StartTask_Handler;		//任务句柄
 
 
#define TASK1_TASK_PRIO			2
#define TASK1_STK_SIZE			120
void task1_task(void * pvParameters);
TaskHandle_t Task1Task_Handler;		//任务句柄
 
#define TASK2_TASK_PRIO			3
#define TASK2_STK_SIZE			120 
void task2_task(void * pvParameters);
TaskHandle_t Task2Task_Handler;		//任务句柄	
 
int main(void)
{
	
		Stm32_Clock_Init(336,8,2,7);
		delay_init(168);                //初始化延时函数
    LED_Init();                     //初始化LED
    uart_init(84,115200);              //初始化串口
       
	xTaskCreate((TaskFunction_t	) start_task,
				(char*			) "start_task",
				(uint16_t		) START_STK_SIZE,
				(void * 		) NULL,
				(UBaseType_t	) START_TASK_PRIO,
				(TaskHandle_t*	) &StartTask_Handler);
    vTaskStartScheduler();          //开启任务调度
}
 
void start_task(void * pvParameters)
{
	//Task1
	xTaskCreate((TaskFunction_t	) task1_task,
				(char*			) "task1_task",
				(uint16_t		) TASK1_STK_SIZE,
				(void * 		) NULL,
				(UBaseType_t	) TASK1_TASK_PRIO,
				(TaskHandle_t*	) &Task1Task_Handler);
				
	//Task2
	xTaskCreate((TaskFunction_t	) task2_task,
				(char*			) "task2_task",
				(uint16_t		) TASK1_STK_SIZE,
				(void * 		) NULL,
				(UBaseType_t	) TASK2_TASK_PRIO,
				(TaskHandle_t*	) &Task2Task_Handler);
	vTaskDelete(StartTask_Handler);
}
 
void task1_task(void * pvParameters)
{
	char task1_num=0;
	
	while(1)
	{
		task1_num++;
		
		LED0 = ~LED0;
		printf("Task1 Runing %d !\r\n",task1_num);
		
		vTaskDelay(500);
	}
}
 
void task2_task(void * pvParameters)
{
	char task2_num=0;
	while(1)
	{
		task2_num++;
		
		LED1 = ~LED1;
		printf("Task2 Runing %d!\r\n",task2_num);
		vTaskDelay(1000);
	}
}
