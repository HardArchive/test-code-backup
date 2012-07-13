#pragma once

//��ȡӲ�����к� 
//��ǩ�� ��������  2007-01-10 12:17
//ͷ�ļ�
// GetHDSerial.h: interface for the CGetHDSerial class.
//////////////////////////////////////////////////////////////////////
//#ifndef _GETHDSERIAL_H
//#define _GETHDSERIAL_H

#include "stdafx.h" 
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
extern int nHARD_MODE;

#define  SENDIDLENGTH     sizeof(SENDCMDOUTPARAMS)+IDENTIFY_BUFFER_SIZE

#define  IDENTIFY_BUFFER_SIZE  512

#define  FILE_DEVICE_SCSI              0x0000001b

#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ((FILE_DEVICE_SCSI << 16) + 0x0501)

#define  IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition

#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.

#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.

#define  IOCTL_GET_DRIVE_INFO   0x0007c088

#define  IOCTL_GET_VERSION          0x00074080

typedef struct _IDSECTOR
{
	USHORT  wGenConfig;
	USHORT  wNumCyls;
	USHORT  wReserved;
	USHORT  wNumHeads;
	USHORT  wBytesPerTrack;
	USHORT  wBytesPerSector;
	USHORT  wSectorsPerTrack;
	USHORT  wVendorUnique[3];
	CHAR    sSerialNumber[20];
	USHORT  wBufferType;
	USHORT  wBufferSize;
	USHORT  wECCSize;
	CHAR    sFirmwareRev[8];
	CHAR    sModelNumber[40];
	USHORT  wMoreVendorUnique;
	USHORT  wDoubleWordIO;
	USHORT  wCapabilities;
	USHORT  wReserved1;
	USHORT  wPIOTiming;
	USHORT  wDMATiming;
	USHORT  wBS;
	USHORT  wNumCurrentCyls;
	USHORT  wNumCurrentHeads;
	USHORT  wNumCurrentSectorsPerTrack;
	ULONG   ulCurrentSectorCapacity;
	USHORT  wMultSectorStuff;
	ULONG   ulTotalAddressableSectors;
	USHORT  wSingleWordDMA;
	USHORT  wMultiWordDMA;
	BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;

//typedef struct _DRIVERSTATUS
//{
//	BYTE  bDriverError;  //  Error code from driver, or 0 if no error.
//	BYTE  bIDEStatus;    //  Contents of IDE Error register.
//	//  Only valid when bDriverError is SMART_IDE_ERROR.
//	BYTE  bReserved[2];  //  Reserved for future expansion.
//	DWORD  dwReserved[2];  //  Reserved for future expansion.
//} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;

//typedef struct _SENDCMDOUTPARAMS
//{
//	DWORD         cBufferSize;   //  Size of bBuffer in bytes
//	DRIVERSTATUS  DriverStatus;  //  Driver status structure.
//	BYTE          bBuffer[1];    //  Buffer of arbitrary length in which to store the data read from the                                                       // drive.
//} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

typedef struct _SRB_IO_CONTROL
{
	ULONG HeaderLength;
	UCHAR Signature[8];
	ULONG Timeout;
	ULONG ControlCode;
	ULONG ReturnCode;
	ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;

//typedef struct _IDEREGS
//{
//	BYTE bFeaturesReg;       // Used for specifying SMART "commands".
//	BYTE bSectorCountReg;    // IDE sector count register
//	BYTE bSectorNumberReg;   // IDE sector number register
//	BYTE bCylLowReg;         // IDE low order cylinder value
//	BYTE bCylHighReg;        // IDE high order cylinder value
//	BYTE bDriveHeadReg;      // IDE drive/head register
//	BYTE bCommandReg;        // Actual IDE command.
//	BYTE bReserved;          // reserved for future use.  Must be zero.
//} IDEREGS, *PIDEREGS, *LPIDEREGS;

//typedef struct _SENDCMDINPARAMS
//{
//	DWORD     cBufferSize;   //  Buffer size in bytes
//	IDEREGS   irDriveRegs;   //  Structure with drive register values.
//	BYTE bDriveNumber;       //  Physical drive number to send 
//	//  command to (0,1,2,3).
//	BYTE bReserved[3];       //  Reserved for future expansion.
//	DWORD     dwReserved[4]; //  For future use.
//	BYTE      bBuffer[1];    //  Input buffer.
//} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;

typedef struct _tagGETVERSIONOUTPARAMS
{
	BYTE bVersion;      // Binary driver version.
	BYTE bRevision;     // Binary driver revision.
	BYTE bReserved;     // Not used.
	BYTE bIDEDeviceMap; // Bit map of IDE devices.
	DWORD fCapabilities; // Bit mask of driver capabilities.
	DWORD dwReserved[4]; // For future use.

} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ��ȡӲ�����кŵ���
class CGetHDSerial  
{

public:

	CGetHDSerial();

	virtual ~CGetHDSerial();

	void  _stdcall Win9xReadHDSerial(WORD * buffer);// Windows9X/MEϵͳ�¶�ȡӲ�����к�

	char* GetHDSerial();

	// Windows 9x/MEϵͳ�£��������ͣ�WORD����Ӳ����Ϣת��Ϊ�ַ����ͣ�char��
	char* WORDToChar (WORD diskdata [256], int firstIndex, int lastIndex);

	// Windows NT/2000/XPϵͳ�£���˫�����ͣ�DWORD����Ӳ����Ϣת��Ϊ�ַ����ͣ�char��
	char* DWORDToChar (DWORD diskdata [256], int firstIndex, int lastIndex);

	//Mod by lichunhui 20061229 for slave drive hard
	BOOL  WinNTReadIDEHDSerial (int drive, DWORD * buffer);// WindowsNT/2000/XPϵͳ�¶�ȡIDEӲ�����к�

	BOOL  WinNTReadSCSIHDSerial(int controller, DWORD * buffer); // Windows NT/2000/XP�¶�ȡSCSIӲ�����к�

	// Windows NT/2000/XP�¶�ȡIDE�豸��Ϣ
	BOOL  WinNTGetIDEHDInfo (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
		PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
		PDWORD lpcbBytesReturned); 


	//����� ��ȡϵͳ���ڴ���
	int GetSysDiskNum();

};