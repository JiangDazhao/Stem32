#include "stm32f4xx.h"
#include "FreeRTOS.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"  
#include "led.h"
#include "key.h"
#include "w25qxx.h"
#include "sdio_sdcard.h"
#include "ff.h"			  
#include "diskio.h"		
#include "exfuns.h"


int main()
{
	FIL f;
  char write_[] = "1234567890\n";//写入缓冲区
  char read_[4096] = "";
  UINT bw;
  UINT br;
	BYTE sd[4096];
  BYTE work[4096];
	FRESULT res,flash_res;
	
	u8 malloc_init;

	Stm32_Clock_Init(336,8,2,7);//设置时钟,168Mhz 
	delay_init(168);			   //延时初始化  
	uart_init(84,115200);		//初始化串口波特率为115200 
	LED_Init();					    //初始化LED 
 	KEY_Init();					    //按键初始化
	//W25QXX_Init();				  //初始化W25Q128
	


 	while(SD_Init() != SD_OK)//检测不到SD卡
	{
		LEDa_ON;
		printf("can not find SD!!!");
    delay_ms(500); //延时500ms
		printf("please check!!!");
		LEDa_OFF;
    delay_ms(500); //延时500ms
	}
	printf("SD init succeed!!!\n");
	//show_sdcard_info();
	malloc_init = exfuns_init();//申请内存
if(malloc_init==0)
		{
			printf(" malloc_init!!!\n");
		}	
	else
{
printf("malloc init is %d.\n",malloc_init);
}
	
  res = f_mount(fs[0],"0:",1); 					//挂载SD卡 
	if(res==0)
		{
			printf("mount success!!!\n");
		}
	if(res==FR_NO_FILESYSTEM)   //SD卡磁盘,FAT文件系统错误,重新格式化SD卡
	{
		res = f_mkfs("0:",0,sd,sizeof(sd));//格式化SD卡,0,盘符;0,不需要引导区,8个扇区为1个簇
		if(res)
		printf("f_mkfs is %d.\n",res);
		res = f_mount(NULL,"0:",1); 			//取消挂载SD卡
		res = f_mount(fs[0],"0:",1); 			//挂载SD卡
		delay_ms(1000);
		printf("f_mount is %d.\n",res);
	}
	res = f_open(&f,"0:my_file.txt",FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
	if(res==0)
		{
			printf("open success!!!\n");
		}
	else
		{
			printf("can not open!!!\n");
		}
	if(res == 0)
  {
	  res = f_write(&f ,write_,sizeof(write_),&bw);
		if(res==0)
		{
			printf("write success!!!\n");
		}
	  else
		{
			printf("can not write!!!\n");
		}
		if(res == 0)
		{
		  f_lseek(&f,0);
			res = f_read(&f,read_,f_size(&f),&br);
			if(res == 0)
			{
				printf("read success!!!\n");
				printf("content：%s\n",read_);
			}
      else
			{
				printf("can not read!!!\n");
			}
		}
	}
	f_close(&f);
	while(1)
	{
		LEDa_ON;
    delay_ms(500); //延时500ms
		LEDa_OFF;
		LEDb_ON;
    delay_ms(500); //延时500ms
		LEDb_OFF;
	} 
}
