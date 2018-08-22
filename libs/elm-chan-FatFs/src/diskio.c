/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

extern DSTATUS SD_disk_initialize ();
extern DSTATUS SD_disk_status ();
extern DRESULT SD_disk_read (BYTE *buff, DWORD sector, UINT count);
extern DRESULT SD_disk_write (const BYTE *buff, DWORD sector, UINT count);
extern DRESULT SD_disk_ioctl (BYTE ctrl, void *buff);
extern DSTATUS USBH_disk_initialize();
extern DRESULT USBH_disk_ioctl(BYTE ctrl, void *buff);
extern DRESULT USBH_disk_read(BYTE *buff, DWORD sector, BYTE count);
extern DSTATUS USBH_disk_status();
extern DRESULT USBH_disk_write(const BYTE *buff, DWORD sector, BYTE count);

/* Definitions of physical drive number for each drive */
#define DEV_SD		0	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		1	/* Example: Map USB MSD to physical drive 2 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_SD :
		return SD_disk_status();
	case DEV_USB :
		return USBH_disk_status();
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_SD :
		return SD_disk_initialize();
	case DEV_USB :
		return USBH_disk_initialize();
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	switch (pdrv) {
	case DEV_SD :
        return SD_disk_read(buff, sector, count);
	case DEV_USB :
        return USBH_disk_read(buff, sector, count);
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	switch (pdrv) {
	case DEV_SD :
		return SD_disk_write(buff, sector, count);
	case DEV_USB :
        return USBH_disk_write(buff, sector, count);
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	switch (pdrv) {
	case DEV_SD :
		return SD_disk_ioctl(cmd, buff);
	case DEV_USB :
		return USBH_disk_ioctl(cmd, buff);
	}

	return RES_PARERR;
}

DWORD get_fattime (void)
{
   // TODO
   return 0;
}
