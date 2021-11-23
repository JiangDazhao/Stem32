/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "w25qxx.h"
#include "sdio_sdcard.h"

///* Definitions of physical drive number for each drive */
//#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#define SD_CARD 0
#define SPI_FLASH 1

#define SD_BLOCKSIZE 512


//声明外部变量
extern SD_CardInfo SDCardInfo;
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
//  int result;
	
	switch (pdrv) {
	case SD_CARD :
		if(SD_GetState() == SD_CARD_ERROR)
		  stat = STA_NOINIT; //检测不到SD卡
		else
			stat = 0;
    break;
	case SPI_FLASH :
		if(W25QXX_ReadID()!=W25Q128)
			stat = STA_NOINIT; //检测不到W25Q128
		else
			stat = 0;
		break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
//	int result;

	switch (pdrv) {
	case SD_CARD :
		if(SD_OK == SD_Init())
		{
		  stat = 0;
		}
		else
		{
		  stat = STA_NOINIT;
		}
		return stat;
	case SPI_FLASH :
		W25QXX_Init();
	  return disk_status(SPI_FLASH);
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;
	SD_Error SD_Status = SD_OK;
  //	int result;

	switch (pdrv) {
	case SD_CARD :
    SD_Status = SD_ReadDisk(buff , sector , count);
	  if(SD_Status == SD_OK)
	    res = RES_OK;
		else 
			res = RES_ERROR;
		break;
	case SPI_FLASH :
		W25QXX_Read(buff , sector*4096 , count*4096);

		res = RES_OK;
	  break;
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
//	int result;

	switch (pdrv) {
	case SD_CARD :
    SD_WriteDisk((u8*)buff , sector , count);
	  res = RES_OK;

		return res;
	case SPI_FLASH :
		W25QXX_Write((u8*)buff , sector*4096 , count*4096);
	  res = RES_OK;

		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
//	int result;

	switch (pdrv) {
	case SD_CARD :

		switch(cmd)
		{
		    //返回扇区个数
			case GET_SECTOR_COUNT:
				*(DWORD *)buff = SDCardInfo.CardCapacity / SD_BLOCKSIZE;
				break;
			case GET_SECTOR_SIZE:
				*(WORD *)buff = SD_BLOCKSIZE;
				break;
			case GET_BLOCK_SIZE:
				*(DWORD *)buff = SDCardInfo.CardBlockSize;
				break;
		}

		res = RES_OK;
		return res;

	case SPI_FLASH :

		switch(cmd)
		{
		    //返回扇区个数
			case GET_SECTOR_COUNT:
				*(DWORD *)buff = 16777216;
				break;
			case GET_SECTOR_SIZE:
				*(WORD *)buff = 4096;
				break;
			case GET_BLOCK_SIZE:
				*(WORD *)buff = 1;
				break;
		}

		res = RES_OK;
		return res;
	}

	return RES_PARERR;
}

