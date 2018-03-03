/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include <string.h> // memcpy
#include "stm32f4xx.h"
#include "Dev_SDIO.h"

#define BLOCK_SIZE            512 /* Block Size in Bytes */

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
        BYTE drv                                /* Physical drive nmuber (0..) */
)
{
	DSTATUS stat = 0;
	
 	/* Supports only single drive */
  if (drv)
  {
    stat |= STA_NOINIT;
  }
	
	/*-------------------------- SD Init ----------------------------- */
  if (SDIO_Setup() !=SDIO_OK)
  {
    stat |= STA_NOINIT;
  }

	return(stat);
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
        BYTE drv                /* Physical drive nmuber (0..) */
)
{
	DSTATUS stat = 0;
	
	if (SDIO_Detect() != SDIO_PRESENT)
		stat |= STA_NODISK;

	// STA_NOTINIT - Subsystem not initailized
	// STA_PROTECTED - Write protected, MMC/SD switch if available
	
	return(stat);
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
        BYTE drv,               /* Physical drive nmuber (0..) */
        BYTE *buff,             /* Data buffer to store read data */
        DWORD sector,   				/* Sector address (LBA) */
        BYTE count              /* Number of sectors to read (1..255) */
)
{
	SDIO_STATE Status;

	
	if (SDIO_Detect() != SDIO_PRESENT)
		return(RES_NOTRDY);

	if ((DWORD)buff & 3) // DMA Alignment failure, do single up to aligned buffer
	{
		DRESULT res = RES_OK;
		DWORD scratch[BLOCK_SIZE / 4]; // Alignment assured, you'll need a sufficiently big stack

		while(count--)
		{
			res = disk_read(drv, (void *)scratch, sector++, 1);

			if (res != RES_OK)
				break;

			memcpy(buff, scratch, BLOCK_SIZE);

			buff += BLOCK_SIZE;
		}

		return(res);
	}

  Status = SDIO_ReadMultiBlocks(buff, sector, BLOCK_SIZE, count); // 4GB Compliant

	if (Status == SDIO_OK)
	{
		SDTransferState State;

		Status = SDIO_WaitReadOperation(); // Check if the Transfer is finished

		while((State = SDIO_GetStatus()) == SDIO_TRANSFER_BUSY); // BUSY, OK (DONE), ERROR (FAIL)

		if ((State == SDIO_TRANSFER_ERROR) || (Status != SDIO_OK))
			return(RES_ERROR);
		else
			return(RES_OK);
	}
	else
		return(RES_ERROR);
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
        BYTE drv,                       /* Physical drive nmuber (0..) */
        const BYTE *buff,       /* Data to be written */
        DWORD sector,           /* Sector address (LBA) */
        BYTE count                      /* Number of sectors to write (1..255) */
)
{
	SDIO_STATE Status;
	
	if (SDIO_Detect() != SDIO_PRESENT)
		return(RES_NOTRDY);

	if ((DWORD)buff & 3) // DMA Alignment failure, do single up to aligned buffer
	{
		DRESULT res = RES_OK;
		DWORD scratch[BLOCK_SIZE / 4]; // Alignment assured, you'll need a sufficiently big stack

		while(count--)
		{
			memcpy(scratch, buff, BLOCK_SIZE);

			res = disk_write(drv, (void *)scratch, sector++, 1);

			if (res != RES_OK)
				break;

			buff += BLOCK_SIZE;
		}

		return(res);
	}

  Status = SDIO_WriteMultiBlocks((uint8_t *)buff, sector, BLOCK_SIZE, count); // 4GB Compliant

	if (Status == SDIO_OK)
	{
		SDTransferState State;

		Status = SDIO_WaitWriteOperation(); // Check if the Transfer is finished

		while((State = SDIO_GetStatus()) == SDIO_TRANSFER_BUSY); // BUSY, OK (DONE), ERROR (FAIL)

		if ((State == SDIO_TRANSFER_ERROR) || (Status != SDIO_OK))
			return(RES_ERROR);
		else
			return(RES_OK);
	}
	else
		return(RES_ERROR);
}
#endif /* _READONLY */




/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
        BYTE drv,               /* Physical drive nmuber (0..) */
        BYTE ctrl,              /* Control code */
        void *buff              /* Buffer to send/receive control data */
)
{
     DRESULT res = RES_OK;

    if (drv)
    {    
        return RES_PARERR;  //仅支持单磁盘操作，否则返回参数错误
    }
//    2010.02.16  23：35 完成此模块。
    switch(ctrl)
    {
    case CTRL_SYNC:

        while(SDIO_GetResponse(SDIO_RESP1)==0);//(SD_WaitReady
        break;
    case GET_SECTOR_SIZE:
        *(WORD*)buff = 512;
        res = RES_OK;
        break;
    case GET_SECTOR_COUNT:
        if((SDCardInfo.CardType == SDIO_STD_CAPACITY_SDIO_CARD_V1_1) || (SDCardInfo.CardType == SDIO_STD_CAPACITY_SDIO_CARD_V2_0))
            *(DWORD*)buff = SDCardInfo.CardCapacity >> 9;
        else if(SDCardInfo.CardType == SDIO_HIGH_CAPACITY_SDIO_CARD)
            *(DWORD*)buff = (SDCardInfo.SDIO_csd.DeviceSize+1)*1024;
        //else;////SD_GetCapacity();
        res = RES_OK;
        break;
    case GET_BLOCK_SIZE:
        *(WORD*)buff = SDCardInfo.CardBlockSize;
        res = RES_OK;
        break;

    default:
        res = RES_PARERR;
        break;
    }
    return res;//RES_OK;//
}

/*-----------------------------------------------------------------------*/
/* Get current time                                                      */
/*-----------------------------------------------------------------------*/
DWORD get_fattime(void){
        return 0;
}
