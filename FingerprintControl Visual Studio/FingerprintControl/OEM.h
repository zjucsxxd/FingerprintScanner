#ifndef OEM
#define OEM

#include "CommBase.h"
#include "sb_protocol_oem.h"

/// Definition of the commands of the device ///
/// Provided by ADH-Tech ///

enum
{
	CMD_NONE = 0x00,
	CMD_OPEN = 0x01,
	CMD_CLOSE = 0x02,
	CMD_USB_INTERNAL_CHECK = 0x03,
	CMD_CHANGE_BAUDRATE = 0x04,

	CMD_CMOS_LED = 0x12,

	CMD_ENROLL_COUNT = 0x20,
	CMD_CHECK_ENROLLED = 0x21,
	CMD_ENROLL_START = 0x22,
	CMD_ENROLL1 = 0x23,
	CMD_ENROLL2 = 0x24,
	CMD_ENROLL3 = 0x25,
	CMD_IS_PRESS_FINGER = 0x26,

	CMD_DELETE = 0x40,
	CMD_DELETE_ALL = 0x41,

	CMD_VERIFY = 0x50,
	CMD_IDENTIFY = 0x51,
	CMD_VERIFY_TEMPLATE = 0x52,
	CMD_IDENTIFY_TEMPLATE = 0x53,

	CMD_CAPTURE = 0x60,

	CMD_GET_IMAGE = 0x62,
	CMD_GET_RAWIMAGE = 0x63,

	CMD_GET_TEMPLATE = 0x70,
	CMD_ADD_TEMPLATE = 0x71,
	CMD_GET_DATABASE_START = 0x72,
	CMD_GET_DATABASE_END = 0x73,

	CMD_FW_UPDATE = 0x80,
	CMD_ISO_UPDATE = 0x81,

	ACK_OK = 0x30,
	NACK_INFO = 0x31,
};

enum
{
	NACK_NONE = 0x1000,
	NACK_TIMEOUT,
	NACK_INVALID_BAUDRATE,
	NACK_INVALID_POS,
	NACK_IS_NOT_USED,
	NACK_IS_ALREADY_USED,
	NACK_COMM_ERR,
	NACK_VERIFY_FAILED,
	NACK_IDENTIFY_FAILED,
	NACK_DB_IS_FULL,
	NACK_DB_IS_EMPTY,
	NACK_TURN_ERR,
	NACK_BAD_FINGER,
	NACK_ENROLL_FAILED,
	NACK_IS_NOT_SUPPORTED,
	NACK_DEV_ERR,
	NACK_CAPTURE_CANCELED,
	NACK_INVALID_PARAM,
	NACK_FINGER_IS_NOT_PRESSED,
};

enum
{
	OEM_NONE = -2000,
	OEM_COMM_ERR,
};

typedef struct _devinfo
{
	DWORD FirmwareVersion;
	DWORD IsoAreaMaxSize;
	BYTE DeviceSerialNumber[16];
} devinfo;

#define FP_TEMPLATE_SIZE	506
#define IMG8BIT_SIZE	240*216

extern BYTE	gbyImg8bit[IMG8BIT_SIZE];

extern WORD gwDevID;
extern WORD gwLastAck;
extern int  gwLastAckParam;
extern int  gnPassedTime;

extern devinfo gDevInfo;

int oem_CommandRun(WORD wCmd, int nCmdParam);
int oem_open(void); // Initialization
int oem_close(void); // Termination
int oem_usb_internal_check(void); // Check if the connected USB device is valid
int oem_cmos_led(BOOL bOn); // Control CMOS LED
int oem_capture(BOOL bBest); // Capture a fingerprint image (256x256) from the sensor
int oem_get_image(void); // Download the captured fingerprint image (256x256)


/// NOT USED HERE ///

/*
extern BYTE gbyImgRaw[240 * 216];
extern BYTE	gbyTemplate[FP_TEMPLATE_SIZE];

int oem_change_baudrate(int nBaudrate); // Change UART baud rate
int oem_enroll_count(void); // Get enrolled fingerprint count
int oem_check_enrolled(int nPos); // Check whether the specified ID is already enrolled
int oem_enroll_start(int nPos); // Start an enrollment
int oem_enroll_nth(int nPos, int nTurn); // Make nth template for enrollment and if the 3rd, merge three templates into one template, save merged template to the database
int oem_is_press_finger(void); // Check if a finger is placed on the sensor
int oem_delete(int nPos); // Delete the fingerprint with the specified ID
int oem_delete_all(void); // Delete all fingerprints from the database
int oem_verify(int nPos); // 1:1 Verification of the capture fingerprint image with the specified ID
int oem_identify(void); // 1:N Identification of the capture fingerprint image with the database
int oem_verify_template(int nPos); // 1:1 Verification of a fingerprint template with the specified ID
int oem_identify_template(void); // 1:N Identification of a fingerprint template with the database
int oem_get_rawimage(void); // Capture and Download raw fingerprint image (320x240)
int oem_get_template(int nPos); // Download the template of the specified ID
int oem_add_template(int nPos); // Make template for transmission and upload the template of the specified ID
int oem_get_database_start(void); // Start database download, obsolete
int oem_get_database_end(void); // End database download, obsolete
int oem_fw_upgrade(BYTE* pBuf, int nLen, CProgressCtrl *pProgress = NULL); // Firmware upgrade
int oem_iso_upgrade(BYTE* pBuf, int nLen, CProgressCtrl *pProgress = NULL); // ISO upgrade, not supported
*/

#endif