#ifndef COMMBASE
#define COMMBASE

#include "OEM.h"

/// USB and Serial communication protocols ///
/// Provided by ADH-Tech ///

#define USB_DEVICE_MARK 0x55
#define USB_BLOCK_SIZE 65536

inline void ui_polling()
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

enum
{
	COMM_MODE_SERIAL,
	COMM_MODE_USB,
};

extern double comm_percent;
extern int gn_comm_type;

int comm_send(BYTE* pbuf, int nsize, int ntimeout);
int comm_recv(BYTE* pbuf, int nsize, int ntimeout);
BOOL comm_open_serial(int nComNumber, int nComBaudRate);
BOOL comm_open_usb();
void comm_close();

typedef struct _SCSI_PASS_THROUGH_DIRECT
{
	USHORT  Length;
	UCHAR  ScsiStatus;
	UCHAR  PathId;
	UCHAR  TargetId;
	UCHAR  Lun;
	UCHAR  CdbLength;
	UCHAR  SenseInfoLength;
	UCHAR  DataIn;
	ULONG  DataTransferLength;
	ULONG  TimeOutValue;
	PVOID  DataBuffer;
	ULONG  SenseInfoOffset;
	UCHAR  Cdb[16];
}SCSI_PASS_THROUGH_DIRECT, *PSCSI_PASS_THROUGH_DIRECT;

typedef struct _SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER
{
	SCSI_PASS_THROUGH_DIRECT sptd;
	ULONG Filler;
	UCHAR ucSenseBuf[32];
} SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, *PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER;

#define CDB10GENERIC_LENGTH 10
#define SCSI_IOCTL_DATA_OUT 0
#define SCSI_IOCTL_DATA_IN 1
#define SCSI_IOCTL_DATA_UNSPECIFIED 2
#define IOCTL_SCSI_BASE  FILE_DEVICE_CONTROLLER
#define IOCTL_SCSI_PASS_THROUGH_DIRECT  CTL_CODE(IOCTL_SCSI_BASE, 0x0405, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

class CCommSerial
{
public:
	CCommSerial();
	virtual ~CCommSerial();

	BOOL Open(int nPort, DWORD dwBaudrate);
	void Close();
	int SendData(BYTE *pBuf, int nSize, DWORD dwTimeOut);
	int RecvData(BYTE *pBuf, int nSize, DWORD dwTimeOut);

private:
	HANDLE m_hComm;
};

class CCommUSB
{
public:
	CCommUSB();
	virtual ~CCommUSB();

	BOOL Open();
	void Close();
	int SendData(BYTE *pBuf, int nSize, DWORD dwTimeOut);
	int RecvData(BYTE *pBuf, int nSize, DWORD dwTimeOut);

private:
	HANDLE	m_hDevice;

	BOOL	SCSI_Open();
	void	SCSI_Close();
	int		SCSI_Operation(BYTE *pBuf, int nSize, DWORD dwTimeOut, BOOL bRead);
	int		OperationInternal(BYTE *pBuf, int nSize, DWORD dwTimeOut, BOOL bRead);
};

#endif