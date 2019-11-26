#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ite/ite_usbex.h"
#include "bootloader.h"
#include "config.h"
#include "ite/itp.h"
#include "ite/itu.h"
#include "itu_private.h"

static bool blLcdOn = false;
static bool blLcdConsoleOn = false;
static bool stop_led = false;

// #define USB_PKG_RECORD

#ifdef USB_PKG_RECORD
FILE *UARTPKG_FD = NULL;
#endif

#if defined (CFG_ENABLE_UART_CLI)
static bool boot = false;
static bool bootstate = false;
char tftppara[128] = "tftp://192.168.1.20/doorbell_indoor2.bin";

#if defined (CFG_UART0_ENABLE) && defined(CFG_DBG_UART0)
#define UART_PORT ":uart0"
#elif defined(CFG_UART1_ENABLE) && defined(CFG_DBG_UART1)
#define UART_PORT ":uart1"
#elif defined(CFG_UART2_ENABLE) && defined(CFG_DBG_UART2)
#define UART_PORT ":uart2"
#elif defined(CFG_UART3_ENABLE) && defined(CFG_DBG_UART3)
#define UART_PORT ":uart3"
#endif

#endif //end of #if defined (CFG_ENABLE_UART_CLI)

#if defined(CFG_UPGRADE_FROM_UART)

#if defined(CFG_UPGRADE_UART0)
#define UPGRADE_UART_PORT     ITP_DEVICE_UART0
#define UPGRADE_UART_BAUDRATE CFG_UART0_BAUDRATE
#define UPGRADE_UART_DEVICE   itpDeviceUart0
#elif defined(CFG_UPGRADE_UART1)
#define UPGRADE_UART_PORT     ITP_DEVICE_UART1
#define UPGRADE_UART_BAUDRATE CFG_UART1_BAUDRATE
#define UPGRADE_UART_DEVICE   itpDeviceUart1
#else
#define UPGRADE_UART_PORT     ITP_DEVICE_UART0
#define UPGRADE_UART_BAUDRATE CFG_UART0_BAUDRATE
#define UPGRADE_UART_DEVICE   itpDeviceUart0
#endif

#define UPGRADE_PATTERN       0x1A
#define ACK20                 0x14
#define ACK50                 0x32
#define ACK100                0x64
#define ACK150                0x96
#define ACK200                0xC8
#define ACK210                0xD2
#define ACK211                0xD3
#define ACK220                0xDC
#define ACK221                0xDD

//the total check times is CHECK_NUM or CHECK_NUM+1
#define CHECK_NUM             4
#define RETRY_SIZE            5
#define RETRY_CHECKSUM        1
#define RETRY_DATA            1

#define CHECK_TIME            10

typedef enum UPGRADE_UART_STATE_TAG
{
	WAIT_SIZE,
	WAIT_SIZE_ACK,
	READY,
	WAIT_CHECKSUM,
	WAIT_CHECK_ACK,
	FINISH,
	FAIL,
} UPGRADE_UART_STATE;

static ITCArrayStream UartStream;
static UPGRADE_UART_STATE gState = WAIT_SIZE;
static UPGRADE_UART_STATE gTimeState[CHECK_TIME];
static int gSec = 0;
static unsigned int gTotalSize = 0;
static unsigned int gSecSize[CHECK_TIME];
#endif

#ifdef CFG_MSC_ENABLE
static bool usbInited = false;
static bool usbDetected = false;
#endif

#ifdef CFG_UPGRADE_USB_DEVICE

static bool DetectUsbDeviceMode( void )
{
	bool ret = false;
	LOG_INFO "Detect USB device mode...\r\n" LOG_END

	// init usb
#if defined(CFG_USB0_ENABLE) || defined(CFG_USB1_ENABLE)
	itpRegisterDevice( ITP_DEVICE_USB, &itpDeviceUsb );
	if ( ioctl( ITP_DEVICE_USB, ITP_IOCTL_INIT, NULL ) != -1 )
		usbInited = true;
#endif

	// init usb device mode as mass storage device
#ifdef CFG_USBD_FILE_STORAGE
	if ( usbInited )
	{
		int timeout = CFG_UPGRADE_USB_DETECT_TIMEOUT;

		itpRegisterDevice( ITP_DEVICE_USBDFSG, &itpDeviceUsbdFsg );
		ioctl( ITP_DEVICE_USBDFSG, ITP_IOCTL_INIT, NULL );

		while ( !ret )
		{
			if ( ioctl( ITP_DEVICE_USBDFSG, ITP_IOCTL_IS_CONNECTED, NULL ) )
			{
				ret = true;
				break;
			}

			timeout -= 10;
			if ( timeout <= 0 )
			{
				LOG_INFO "USB device mode not connected.\n" LOG_END
				break;
			}
			usleep( 10000 );
		}
	}
#endif  // CFG_USBD_FILE_STORAGE
	return ret;
}

#endif // CFG_UPGRADE_USB_DEVICE

static void InitFileSystem( void )
{
	// init card device
#if  !defined(_WIN32) && (defined(CFG_SD0_ENABLE) || defined(CFG_SD1_ENABLE) || defined(CFG_CF_ENABLE) || defined(CFG_MS_ENABLE) || defined(CFG_XD_ENABLE) || defined(CFG_MSC_ENABLE) || defined(CFG_RAMDISK_ENABLE) )
	itpRegisterDevice( ITP_DEVICE_CARD, &itpDeviceCard );
	ioctl( ITP_DEVICE_CARD, ITP_IOCTL_INIT, NULL );
#endif

	// init usb
#ifdef CFG_MSC_ENABLE
	if ( !usbInited )
	{
		itpRegisterDevice( ITP_DEVICE_USB, &itpDeviceUsb );
		if ( ioctl( ITP_DEVICE_USB, ITP_IOCTL_INIT, NULL ) != -1 )
			usbInited = true;
	}
#endif

	// init fat
#ifdef CFG_FS_FAT
	itpRegisterDevice( ITP_DEVICE_FAT, &itpFSDeviceFat.dev );
	ioctl( ITP_DEVICE_FAT, ITP_IOCTL_INIT, NULL );
	ioctl( ITP_DEVICE_FAT, ITP_IOCTL_ENABLE, NULL );
#endif

	// init drive table
#if defined(CFG_FS_FAT) || defined(CFG_FS_NTFS)
	itpRegisterDevice( ITP_DEVICE_DRIVE, &itpDeviceDrive );
	ioctl( ITP_DEVICE_DRIVE, ITP_IOCTL_INIT, NULL );

#ifdef CFG_MSC_ENABLE
	ioctl( ITP_DEVICE_DRIVE, ITP_IOCTL_ENABLE, NULL );
	ioctl( ITP_DEVICE_DRIVE, ITP_IOCTL_INIT_TASK, NULL );
#endif// CFG_MSC_ENABLE
#endif// defined(CFG_FS_FAT) || defined(CFG_FS_NTFS)

	// mount disks on booting
#ifdef CFG_UPGRADE_FILE_FOR_NAND_PROGRAMMER
	if ( !ioctl( ITP_DEVICE_NAND, ITP_IOCTL_NAND_CHECK_REMAP, NULL ) )
	{
		ioctl( ITP_DEVICE_DRIVE, ITP_IOCTL_MOUNT, NULL );
	}
#endif

#ifdef CFG_MSC_ENABLE
	// wait msc is inserted
	if ( usbInited )
	{
		ITPDriveStatus* driveStatusTable;
		ITPDriveStatus* driveStatus = NULL;
		int i, timeout = CFG_UPGRADE_USB_DETECT_TIMEOUT;
		bool found = false;
	#if (defined(CFG_SD0_ENABLE) && !defined(CFG_SD0_STATIC) ) || (defined(CFG_SD1_ENABLE) && !defined(CFG_SD1_STATIC) )
		static const int diskTable[] = { ITP_DISK_SD0, ITP_DISK_SD1, -1 };
    #else
		static const int diskTable[] = { -1 };
    #endif      // (defined(CFG_SD0_ENABLE) && !defined(CFG_SD0_STATIC)) || (defined(CFG_SD1_ENABLE) && !defined(CFG_SD1_STATIC))

		// mount usb disks on booting
		ioctl( ITP_DEVICE_DRIVE, ITP_IOCTL_MOUNT, (void*)diskTable );

		while ( !found )
		{
			for ( i = 0; i < 1; i++ )
			{
				if ( ioctl( ITP_DEVICE_USB, ITP_IOCTL_IS_CONNECTED, (void*)(USB0 + i) ) )
				{
					found = true;
					usbDetected = true;
					break;
				}
			}

			if ( found )
			{
				break;
			}
			else
			{
				timeout -= 1;
				if ( timeout <= 0 )
				{
					LOG_INFO "USB device not found.\n" LOG_END
					return;
				}
				usleep( 1000 );
			}
		}

		found = false;
		timeout = CFG_UPGRADE_USB_TIMEOUT;

		ioctl( ITP_DEVICE_DRIVE, ITP_IOCTL_GET_TABLE, &driveStatusTable );
		
		// LOG_INFO "USB found\n" LOG_END
		while ( !found && usbDetected)
		{
			for ( i = 0; i < ITP_MAX_DRIVE; i++ )
			{
				driveStatus = &driveStatusTable[i];
				if ( driveStatus->disk >= ITP_DISK_MSC00 && driveStatus->disk <= ITP_DISK_MSC17 && driveStatus->avail )
				{
					LOG_DBG "drive[%d]:usb disk=%d\n", i, driveStatus->disk LOG_END
					found = true;
					// usleep( 100 * 1000 );
				}
			}
			if ( !found )
			{
				timeout -= 100;
				if ( timeout <= 0 )
				{
					LOG_INFO "USB disk not found.\n" LOG_END
					break;
				}
				usleep( 100000 );
			}
		}
	}
#if (defined(CFG_SD0_ENABLE) && !defined(CFG_SD0_STATIC) ) || (defined(CFG_SD1_ENABLE) && !defined(CFG_SD1_STATIC) )
	else
	{
		static const int diskTable[] = { ITP_DISK_SD0, ITP_DISK_SD1, -1 };
		// mount sd disks on booting
		ioctl( ITP_DEVICE_DRIVE, ITP_IOCTL_MOUNT, (void*)diskTable );
	}
#endif  // (defined(CFG_SD0_ENABLE) && !defined(CFG_SD0_STATIC)) || (defined(CFG_SD1_ENABLE) && !defined(CFG_SD1_STATIC))
#else
	// mount disks on booting
	ioctl( ITP_DEVICE_DRIVE, ITP_IOCTL_MOUNT, NULL );

#endif  // CFG_MSC_ENABLE
}

#if !defined(CFG_LCD_ENABLE) && (CFG_CHIP_FAMILY == 970)
static void* UgLedTask( void* arg )
{
	int gpio_pin = 15;
	ithGpioSetOut( 16 );
	ithGpioSetMode( 16, ITH_GPIO_MODE0 );
	ithGpioSetOut( 15 );
	ithGpioSetMode( 15, ITH_GPIO_MODE0 );
	stop_led = false;

	for (;; )
	{
		if ( stop_led == true )
		{
			ithGpioSet( 15 );
			ithGpioSet( 16 );
			while ( 1 )
				usleep( 500000 );
		}
		ithGpioClear( gpio_pin );
		if ( gpio_pin == 16 )
			gpio_pin = 15;
		else
			gpio_pin = 16;
		ithGpioSet( gpio_pin );
		usleep( 500000 );
	}
}

#endif

#if (CFG_CHIP_FAMILY == 970)
static void DetectKey( void )
{
	int ret;
	int phase = 0;
	int time_counter = 0;
	int key_counter = 0;
	bool key_pressed;
	bool key_released;
	ITPKeypadEvent ev;

	while ( 1 )
	{
		key_pressed = key_released = false;
		ioctl( ITP_DEVICE_KEYPAD, ITP_IOCTL_PROBE, NULL );
		if ( read( ITP_DEVICE_KEYPAD, &ev, sizeof(ITPKeypadEvent) ) == sizeof(ITPKeypadEvent) )
		{
			if ( ev.code == 0 )
			{
				if ( ev.flags & ITP_KEYPAD_DOWN )
					key_pressed = true;
				else if ( ev.flags & ITP_KEYPAD_UP )
					key_released = true;
			}
		}

		if ( phase == 0 )
		{
			if ( key_pressed )
			{
				printf( "key detected\n" );
				phase = 1;
			}
			else
				break;
		}
		else if ( phase == 1 )
		{
			if ( key_released )
				break;
			if ( time_counter > 100 )
			{
				phase = 2;
				ithGpioSetOut( 16 );
				ithGpioSetMode( 16, ITH_GPIO_MODE0 );
				ithGpioSetOut( 15 );
				ithGpioSetMode( 15, ITH_GPIO_MODE0 );
			}
		}
		else if ( phase == 2 )
		{
			if ( key_pressed )
			{
				ithGpioSet( 15 );
				key_counter++;
			}
			if ( key_released )
				ithGpioClear( 15 );

			if ( time_counter > 200 )
			{
				ithGpioSet( 16 );
				ithGpioClear( 15 );
				phase = 3;
			}

			// blink per 6*50000 us
			if ( (time_counter / 6) % 2 )
				ithGpioSet( 16 );
			else
				ithGpioClear( 16 );
		}
		else if ( phase == 3 )
		{
			printf( "key_counter: %d\n", key_counter );
			if ( key_counter == 1 )
			{
				// do reset
				InitFileSystem();
				ioctl( ITP_DEVICE_DRIVE, ITP_IOCTL_MOUNT, NULL );
				ret = ugResetFactory();
		#if defined(CFG_NOR_ENABLE) && CFG_NOR_CACHE_SIZE > 0
				ioctl( ITP_DEVICE_NOR, ITP_IOCTL_FLUSH, NULL );
		#endif

				#ifdef CFG_NAND_ENABLE
				ioctl( ITP_DEVICE_NAND, ITP_IOCTL_FLUSH, ITP_NAND_FTL_MODE );
				#endif

				exit( ret );
			}
			if ( key_counter == 2 )
			{
				// dump addressbook.xml
				InitFileSystem();
				ioctl( ITP_DEVICE_DRIVE, ITP_IOCTL_MOUNT, NULL );
				CopyUclFile();
			}
			ithGpioClear( 16 );
			break;
		}

		usleep( 50000 );
		time_counter++;
	}
}

#endif

#if defined (CFG_ENABLE_UART_CLI)
static int parseCommand( char* str, int strlength )
{

	int ret;

	if ( strncmp( str, "boot", 4 ) == 0 )
	{
		printf( "going to boot procedure\n" );
		boot = true;
	}

	if ( strncmp( str, "update", 6 ) == 0 )
	{
		ITCStream* upgradeFile;
		uint8_t* imagebuf;
		uint32_t imagesize;

		upgradeFile = OpenRecoveryPackage();
		if ( upgradeFile )
		{
			BootBin( upgradeFile );
			return 1;
		}
	}

	if ( strncmp( str, "upgrade", 7 ) == 0 )
	{
		ITCStream* upgradeFile;

		upgradeFile = OpenRecoveryPackage();
		if ( upgradeFile )
		{
			if ( ugCheckCrc( upgradeFile, NULL ) )
				LOG_ERR "Upgrade failed.\n" LOG_END
				else
					ret = ugUpgradePackage( upgradeFile );

	    #if defined(CFG_NOR_ENABLE) && CFG_NOR_CACHE_SIZE > 0
			LOG_INFO "Flushing NOR cache...\n" LOG_END
			        ioctl( ITP_DEVICE_NOR, ITP_IOCTL_FLUSH, NULL );
	    #endif

			#ifdef CFG_NAND_ENABLE
			ioctl( ITP_DEVICE_NAND, ITP_IOCTL_FLUSH, ITP_NAND_FTL_MODE );
			#endif

			if ( ret )
				LOG_INFO "Upgrade failed.\n" LOG_END
				else
					LOG_INFO "Upgrade finished.\n" LOG_END

					        exit( ret );
		}
		else
		{
	#ifdef CFG_UPGRADE_RECOVERY_LED
			ioctl( fd, ITP_IOCTL_OFF, NULL );
	#endif
		}
		while ( 1 )
			;
	}

	if ( strncmp( str, "setenv", 6 ) == 0 )
	{
		char para[128] = { 0 };
		int i = 0;

		strncpy( para, str + 7, strlength - 7 );
		memset( tftppara, 0, 128 );
		strcpy( tftppara, para );
		printf( "\ntftppara=%s\n", tftppara );
	}

	return 0;
}

static void CommandReciver( void )
{
	char rcvbuf[128], cmdbuf[128];
	static int wp = 0;
	int fd, len, i, strlength;

#if defined (CFG_UART0_ENABLE) && defined(CFG_DBG_UART0)
	itpRegisterDevice( ITP_DEVICE_UART0, &itpDeviceUart0 );
	ioctl( ITP_DEVICE_UART0, ITP_IOCTL_RESET, (void*)CFG_UART0_BAUDRATE );
#elif defined (CFG_UART1_ENABLE) && defined(CFG_DBG_UART1)
	itpRegisterDevice( ITP_DEVICE_UART1, &itpDeviceUart1 );
	ioctl( ITP_DEVICE_UART1, ITP_IOCTL_RESET, (void*)CFG_UART1_BAUDRATE );
#elif defined (CFG_UART2_ENABLE) && defined(CFG_DBG_UART2)
	itpRegisterDevice( ITP_DEVICE_UART2, &itpDeviceUart2 );
	ioctl( ITP_DEVICE_UART2, ITP_IOCTL_RESET, (void*)CFG_UART2_BAUDRATE );
#elif defined (CFG_UART3_ENABLE) && defined(CFG_DBG_UART3)
	itpRegisterDevice( ITP_DEVICE_UART3, &itpDeviceUart3 );
	ioctl( ITP_DEVICE_UART3, ITP_IOCTL_RESET, (void*)CFG_UART3_BAUDRATE );
#endif

	fd = open( UART_PORT, O_RDONLY );

	if ( fd < 0 )
		return;

	for (;; )
	{

		memset( rcvbuf, 0, 128 );
		len = read( fd, rcvbuf, 128 );

		if ( len )
		{
			for ( i = 0; i < len; i++ )
			{
				cmdbuf[wp] = rcvbuf[i];
				wp++;
				if ( rcvbuf[i] == '\0' )
				{
					strlength = strlen( cmdbuf );
					parseCommand( cmdbuf, strlength );
					memset( cmdbuf, 0, 128 );
					wp = 0;
				}
			}
		}
		if ( boot )
			break;

	}
	printf( "Exit CommandReciver\n" );
}

#endif
static void DoUpgrade( void )
{
	ITCStream* upgradeFile;

	LOG_INFO "Do Upgrade...\r\n" LOG_END

	upgradeFile = OpenUpgradePackage();
	if ( upgradeFile )
	{
		int ret = 0;

    #if !defined(CFG_LCD_ENABLE) && (CFG_CHIP_FAMILY == 970)
		//---light on red/green led task
		pthread_t task;
		pthread_create( &task, NULL, UgLedTask, NULL );
		//------
    #endif
		// output messages to LCD console
    #if defined(CFG_LCD_ENABLE) && defined(CFG_BL_LCD_CONSOLE)
		if ( !blLcdOn )
		{
	#if !defined(CFG_BL_SHOW_LOGO)
			extern uint32_t __lcd_base_a;
			extern uint32_t __lcd_base_b;
			extern uint32_t __lcd_base_c;

			itpRegisterDevice( ITP_DEVICE_SCREEN, &itpDeviceScreen );
			ioctl( ITP_DEVICE_SCREEN, ITP_IOCTL_RESET, NULL );
			ithLcdSetBaseAddrA( (uint32_t) &__lcd_base_a );
			ithLcdSetBaseAddrB( (uint32_t) &__lcd_base_b );

	#ifdef CFG_BACKLIGHT_ENABLE
			itpRegisterDevice( ITP_DEVICE_BACKLIGHT, &itpDeviceBacklight );
			ioctl( ITP_DEVICE_BACKLIGHT, ITP_IOCTL_INIT, NULL );
	#endif// CFG_BACKLIGHT_ENABLE

	#endif// !defined(CFG_BL_SHOW_LOGO)

			ioctl( ITP_DEVICE_SCREEN, ITP_IOCTL_POST_RESET, NULL );
			ioctl( ITP_DEVICE_BACKLIGHT, ITP_IOCTL_RESET, NULL );
			blLcdOn = true;
		}
		if ( !blLcdConsoleOn )
		{
			itpRegisterDevice( ITP_DEVICE_STD, &itpDeviceLcdConsole );
			itpRegisterDevice( ITP_DEVICE_LCDCONSOLE, &itpDeviceLcdConsole );
			ioctl( ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_INIT, NULL );
			ioctl( ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_CLEAR, NULL );
			blLcdConsoleOn = true;
		}
    #endif      // defined(CFG_LCD_ENABLE) && defined(BL_LCD_CONSOLE)

		if ( ugCheckCrc( upgradeFile, NULL ) )
			LOG_ERR "Upgrade failed.\n" LOG_END
			else
				ret = ugUpgradePackage( upgradeFile );

    #ifdef CFG_UPGRADE_DELETE_PKGFILE_AFTER_FINISH
		DeleteUpgradePackage();
    #endif

	#if defined(CFG_NOR_ENABLE) && CFG_NOR_CACHE_SIZE > 0
		LOG_INFO "Flushing NOR cache...\n" LOG_END
		        ioctl( ITP_DEVICE_NOR, ITP_IOCTL_FLUSH, NULL );
	#endif

		#ifdef CFG_NAND_ENABLE
		ioctl( ITP_DEVICE_NAND, ITP_IOCTL_FLUSH, ITP_NAND_FTL_MODE );
		#endif

		if ( ret )
			LOG_INFO "Upgrade failed.\n" LOG_END
			else
			{
				stop_led = true;
				LOG_INFO "Upgrade finished.\n" LOG_END
			}
    #if defined(CFG_UPGRADE_DELAY_AFTER_FINISH) && CFG_UPGRADE_DELAY_AFTER_FINISH > 0
		sleep( CFG_UPGRADE_DELAY_AFTER_FINISH );
    #endif

		exit( ret );
		while ( 1 );
	}
}

#ifdef CFG_BOOT_TESTBIN_ENABLE
static void DoBootTestBin( void )
{
	ITCStream* testBinFile;

	LOG_INFO "Do Test Bin Booting...\r\n" LOG_END

	testBinFile = OpenTestBin();
	if ( testBinFile )
	{
		BootTestBin( testBinFile );
	}
}

#endif

#ifdef CFG_BACKLIGHT_ENABLE
static void* backlightResetTask( void* arg )
{
	// usleep( 100000 );
	ioctl( ITP_DEVICE_BACKLIGHT, ITP_IOCTL_RESET, NULL );
}

#endif

unsigned short niu_CRC16( unsigned char *start_address, unsigned int len, unsigned int opt )
{
	#define CRC16_TEST 3
	printf( "crc16 test %d\t\n", CRC16_TEST );

    #if (CRC16_TEST == 1)
	unsigned short crc = 0xFFFF;
	unsigned short CRC_KEY = 0xE32A;
	unsigned int i, j;
	// printf("crc16 test 1\t\n");
	// crc = (!opt) ? 0xFFFF : crc;
	for ( i = 0; i < len; i++ )
	{
		// printf("crc16 index i=0x%X..\t\n", i);
		crc ^= (unsigned short)start_address[i];
		for ( j = 0; j < 8; j++ )
		{
			crc = (crc & 1) != 0 ? (unsigned short)( (crc >> 1) ^ CRC_KEY) : (unsigned short)(crc >> 1);
		}
	}
	return crc;

    #elif (CRC16_TEST == 2)

	unsigned int i, j;
	unsigned short crc = 0x0000;

	for ( i = 0; i < len; i++ )
	{
		crc ^= ( (unsigned short)start_address[i] << 8);
		for ( j = 0; j < 8; j++ )
		{
			if ( (crc & 0x8000) > 0 )
				crc = (unsigned short)( (crc << 1) ^ 0x8408);
			else
				crc <<= 1;
		}
	}
	return crc;

	#elif (CRC16_TEST == 3)

	unsigned short crc = 0xFFFF;
	unsigned short CRC_KEY = 0xE32A;
	unsigned int i, j;

	for ( i = 0; i < len; i++ )
	{
		crc ^= start_address[i];
		for ( j = 0; j < 8; j++ )
		{
			if ( (crc & 1) > 0 )
			{
				crc = (crc >> 1) ^ CRC_KEY;
			}
			else
			{
				crc = (crc >> 1);
			}			
		}
	}
	return crc;

    #endif
}

unsigned char niu_CRC8( unsigned char *ptr, unsigned int len )
{
	unsigned int crc;
	unsigned int i, j;
	crc = 0;
	for ( i = 0; i < len; i++ )
	{
		crc ^= *ptr++;
		for ( j = 0; j < 8; j++ )
		{
			if ( crc & 0x80 )
			{
				crc = (crc << 1) ^ 0x07;
			}
			else
			{
				crc <<= 1;
			}
		}
	}
	return (unsigned char)crc;
}

unsigned char niu_CS8( unsigned char *ptr, unsigned int len )
{
	unsigned int checksum = 0;
	while ( len-- )
	{
		checksum += *ptr++;
	}
	return (unsigned char)checksum;
}

unsigned short niu_CS16( unsigned char *ptr, unsigned int len, unsigned int opt )
{
	unsigned int checksum = 0;
	while ( len-- )
	{
		checksum += *ptr++;
	}
	return (unsigned short)checksum;
}

#if !defined(_WIN32)
#if defined(CFG_UPGRADE_FROM_UART)
static void UartTimeout_handler( timer_t timerid, int arg )
{
	int i;
	int same_count = 0;

	gTimeState[gSec] = gState;
	gSecSize[gSec] = gTotalSize;
	gSec++;

	if ( gSec >= CHECK_TIME )    //10 sec
	{
		for ( i = 0; i < CHECK_TIME - 1; i++ )
		{
			if ( gTimeState[i] == gTimeState[i + 1] )
			{
				same_count++;
			}
		}

		if ( same_count == CHECK_TIME - 1 )
		{
			if ( gState == READY )
			{
				same_count = 0;

				for ( i = 0; i < CHECK_TIME - 1; i++ )
				{
					if ( gSecSize[i] == gSecSize[i + 1] )
					{
						same_count++;
					}
				}

				if ( same_count == CHECK_TIME - 1 )
				{
					printf( "Timeout, gTotalSize = %d\n", gTotalSize );
#if 0
					for ( i = 0; i < CHECK_TIME; i++ )
					{
						printf( "gSecSize[%d]=%d\n", i, gSecSize[i] );
					}
#endif
					gState = FAIL;
				}
			}
			else if ( gState != READY )
			{
				printf( "Timeout, state=%d\n", gState );
				gState = FAIL;
			}
		}
		else
		{
			//reset
			for ( i = 0; i < CHECK_TIME; i++ )
			{
				gTimeState[i] = WAIT_SIZE;
			}
		}

		gSec = 0;
	}
}

static void UgPercentage_handler( timer_t timerid, int arg )
{
	unsigned char ProgressBar[1] = { 0 };

	//Notice: int(4 bytes) convert to unsigned char(1 byte)
	ProgressBar[0] = ugGetProrgessPercentage();

	write( UPGRADE_UART_PORT, ProgressBar, 1 );
}

static int UpgradeUartPackage( void )
{
	int ret = 0, i = 0;
	ITCStream* fwStream = NULL;
	unsigned char tmp[1024] = { 0 };
	unsigned char *pkgData = NULL;
	unsigned char *checksumData = NULL;
	unsigned char SizeAck[4] = { 0 }, CSAck[4] = { 0 }, ACK[1] = { 0 };
	unsigned int tmpTotalSize = 0;
	unsigned int checkSum = 0;
	unsigned int PkgSize = 0, readLen = 0;
	int retrySize = 1, retryChecksum = 1, retryData = 1;
	int checkSize = 0, remainSize = 0;
	int sliceNum = 1, sliceSize = 0;
	timer_t timer, timer2;
	struct itimerspec value, value2;

	//Init UartTimeout_handler
	for ( i = 0; i < 10; i++ )
	{
		gTimeState[i] = WAIT_SIZE;
	}
	timer_create( CLOCK_REALTIME, NULL, &timer );
	timer_connect( timer, (VOIDFUNCPTR)UartTimeout_handler, 0 );
	value.it_value.tv_sec = value.it_interval.tv_sec = 1;
	value.it_value.tv_nsec = value.it_interval.tv_nsec = 0;
	timer_settime( timer, 0, &value, NULL );

	while ( 1 )
	{
		switch ( gState )
		{
			case WAIT_SIZE:
				readLen = read( UPGRADE_UART_PORT, tmp + tmpTotalSize, 1024 );
				tmpTotalSize += readLen;
				if ( tmpTotalSize >= 3 )
				{
					//Get Pkg_size
					PkgSize = tmp[0] | tmp[1] << 8 | tmp[2] << 16;
					printf( "PkgSize=0x%X\n", PkgSize );

					//Send Ack=0x64 + PkgSize
					SizeAck[0] = ACK50;
					SizeAck[1] = tmp[0];
					SizeAck[2] = tmp[1];
					SizeAck[3] = tmp[2];
					write( UPGRADE_UART_PORT, SizeAck, 4 );

					//Change to WAIT_SIZE_ACK state
					gState = WAIT_SIZE_ACK;

					//reset data
					tmpTotalSize = 0;
					memset( tmp, 0, 1024 );
				}
				break;
			case WAIT_SIZE_ACK:
				readLen = read( UPGRADE_UART_PORT, tmp + tmpTotalSize, 1024 );
				tmpTotalSize += readLen;
				if ( tmpTotalSize >= 1 )
				{
					if ( tmp[0] == ACK50 + 0x1 )
					{
						//allocate memory for pkgData
						pkgData = (unsigned char *)malloc( PkgSize );
						if ( pkgData == NULL )
						{
							printf( "pkgData cannot be allocated\n" );
							ret = -1;
							exit( ret );
						}
						memset( pkgData, 0, PkgSize );

						checkSize = PkgSize / CHECK_NUM;
						remainSize = PkgSize % CHECK_NUM;
						printf( "checkSize = %d remainSize = %d\n", checkSize, remainSize );

						//send Ack100
						ACK[0] = ACK100;
						write( UPGRADE_UART_PORT, ACK, 1 );

						gState = READY;
					}
					else
					{
						printf( "WAIT_SIZE_ACK FAIL:Retry %d\n", retrySize );
						//ACK50+0x28
						if ( retrySize >= RETRY_SIZE )
						{
							gState = FAIL;
						}
						else
						{
							gState = WAIT_SIZE;
							retrySize++;
						}
					}

					//reset data
					tmpTotalSize = 0;
					memset( tmp, 0, 1024 );
				}
				break;
			case READY:
				readLen = read( UPGRADE_UART_PORT, pkgData + gTotalSize, 1024 );
				gTotalSize += readLen;
				sliceSize += readLen;
				if ( sliceSize >= checkSize && sliceNum <= CHECK_NUM )
				{
					//Send Ack150
					ACK[0] = ACK150;
					write( UPGRADE_UART_PORT, ACK, 1 );

					gState = WAIT_CHECKSUM;
				}
				else if ( sliceSize >= remainSize && sliceNum > CHECK_NUM )
				{
					//the last slice, send Ack150
					ACK[0] = ACK150;
					write( UPGRADE_UART_PORT, ACK, 1 );

					gState = WAIT_CHECKSUM;
				}
				break;
			case WAIT_CHECKSUM:
				readLen = read( UPGRADE_UART_PORT, tmp + tmpTotalSize, 1024 );
				tmpTotalSize += readLen;
				if ( tmpTotalSize >= 4 )
				{
					//printf("tmpTotalSize=%d TotalSize=%d sliceSize=%d\n", tmpTotalSize, TotalSize, sliceSize);
					//Calculate CheckSum(only new data need to do)
					if ( retryChecksum == 1 || retryData > 1 )
					{
						checksumData = pkgData + (gTotalSize - sliceSize);
						for ( i = 0; i < sliceSize; i++ )
						{
							checkSum += checksumData[i];
						}

						CSAck[0] = checkSum & 0xFF;
						CSAck[1] = (checkSum >> 8) & 0xFF;
						CSAck[2] = (checkSum >> 16) & 0xFF;
						CSAck[3] = (checkSum >> 24) & 0xFF;
					}

					printf( "Checksum(PC): %x %x %x %x\n", tmp[0], tmp[1], tmp[2], tmp[3] );
					printf( "Checksum(ITE): %x %x %x %x\n", CSAck[0], CSAck[1], CSAck[2], CSAck[3] );

					if ( (tmp[0] == CSAck[0]) && (tmp[1] == CSAck[1]) && (tmp[2] == CSAck[2]) && (tmp[3] == CSAck[3]) )
					{
						//OK
						ACK[0] = ACK200;
					}
					else
					{
						//FAIL
						ACK[0] = ACK200 + 0x28;
					}
					//Send Ack+OK/FAIL
					write( UPGRADE_UART_PORT, ACK, 1 );
					gState = WAIT_CHECK_ACK;

					//reset data
					tmpTotalSize = 0;
					memset( tmp, 0, 1024 );
				}
				break;
			case WAIT_CHECK_ACK:
				readLen = read( UPGRADE_UART_PORT, tmp + tmpTotalSize, 1024 );
				tmpTotalSize += readLen;
				if ( tmpTotalSize >= 1 )
				{
					if ( tmp[0] == ACK200 + 1 )
					{
						if ( remainSize == 0 && sliceNum == CHECK_NUM )
						{
							gState = FINISH;
						}
						else if ( remainSize != 0 && sliceNum > CHECK_NUM )
						{
							gState = FINISH;
						}
						else
						{
							gState = READY;

							//reset data
							tmpTotalSize = 0;
							memset( tmp, 0, 1024 );
							sliceSize = 0;
							sliceNum++;
							retryChecksum = 1;
							retryData = 1;
							checkSum = 0;
						}
					}
					else
					{
						//ACK200+0x1+0x28
						if ( retryChecksum <= RETRY_CHECKSUM )
						{
							printf( "WAIT_CHECK_ACK FAIL, Retry Checksum\n" );
							//retry checksum
							gState = WAIT_CHECKSUM;
							retryChecksum++;

							//reset data
							tmpTotalSize = 0;
							memset( tmp, 0, 1024 );
						}
						else if ( retryData <= RETRY_DATA )
						{
							printf( "WAIT_CHECK_ACK FAIL, Retry Data\n" );
							//retry data
							gState = READY;
							retryData++;

							//reset data
							tmpTotalSize = 0;
							memset( tmp, 0, 1024 );
							gTotalSize -= sliceSize;
							memset( pkgData + gTotalSize, 0, sliceSize );
							sliceSize = 0;
							checkSum = 0;
						}
						else
						{
							//retry checksum and date done, but it still fail.
							gState = FAIL;
						}
					}
				}
				break;
			case FINISH:
				printf( "FINISH state\n" );
				break;
			case FAIL:
				break;
			default:
				break;
		}

		if ( gState == FAIL || gState == FINISH )
		{
			printf( "State=%d\n", gState );
			timer_delete( timer );
			break;
		}
	}

	if ( gState == FAIL )
	{
		//Download Fail
		ACK[0] = ACK211;
		write( UPGRADE_UART_PORT, ACK, 1 );

		LOG_INFO "FAIL state, Uart Connection Error\n" LOG_END
		if ( pkgData )
			free( pkgData );

		exit( -1 );
	}
	else
	{
		//Download Finish
		ACK[0] = ACK210;
		write( UPGRADE_UART_PORT, ACK, 1 );

		LOG_INFO "Finish state\n" LOG_END
	}

#if 0
	for ( i = 0; i < 512; i++ )
	{
		printf( "%02X ", pkgData[i] );
		if ( ( (i + 1) % 16) == 0 )
		{
			printf( "\n" );
		}
	}
	printf( "\n" );
#endif

	//Init UgPercentage_handler Timer
	timer_create( CLOCK_REALTIME, NULL, &timer2 );
	timer_connect( timer2, (VOIDFUNCPTR)UgPercentage_handler, 0 );
	value2.it_value.tv_sec = value2.it_interval.tv_sec = 1;
	value2.it_value.tv_nsec = value2.it_interval.tv_nsec = 0;
	timer_settime( timer2, 0, &value2, NULL );

	//Upgrade FW start
	printf( "Start to upgrade FW\n" );
	itcArrayStreamOpen( &UartStream, pkgData, PkgSize );
	fwStream = &UartStream.stream;
	if ( !fwStream )
	{
		LOG_INFO "fwStream is NULL\n" LOG_END
		        free( pkgData );
		ret = -1;
		goto UPGRADE_ERROR;
	}

#if defined(CFG_LCD_ENABLE) && defined(CFG_BL_LCD_CONSOLE)
	if ( !blLcdOn )
	{
    #if !defined(CFG_BL_SHOW_LOGO)
		extern uint32_t __lcd_base_a;
		extern uint32_t __lcd_base_b;
		extern uint32_t __lcd_base_c;

		itpRegisterDevice( ITP_DEVICE_SCREEN, &itpDeviceScreen );
		ioctl( ITP_DEVICE_SCREEN, ITP_IOCTL_RESET, NULL );
		ithLcdSetBaseAddrA( (uint32_t)&__lcd_base_a );
		ithLcdSetBaseAddrB( (uint32_t)&__lcd_base_b );

#ifdef CFG_BACKLIGHT_ENABLE
		itpRegisterDevice( ITP_DEVICE_BACKLIGHT, &itpDeviceBacklight );
		ioctl( ITP_DEVICE_BACKLIGHT, ITP_IOCTL_INIT, NULL );
#endif// CFG_BACKLIGHT_ENABLE

#endif// !defined(CFG_BL_SHOW_LOGO)

		ioctl( ITP_DEVICE_SCREEN, ITP_IOCTL_POST_RESET, NULL );
		ioctl( ITP_DEVICE_BACKLIGHT, ITP_IOCTL_RESET, NULL );
		blLcdOn = true;
	}
	if ( !blLcdConsoleOn )
	{
		itpRegisterDevice( ITP_DEVICE_STD, &itpDeviceLcdConsole );
		itpRegisterDevice( ITP_DEVICE_LCDCONSOLE, &itpDeviceLcdConsole );
		ioctl( ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_INIT, NULL );
		ioctl( ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_CLEAR, NULL );
		blLcdConsoleOn = true;
	}
#endif  // defined(CFG_LCD_ENABLE) && defined(BL_LCD_CONSOLE)

	ret = ugCheckCrc( fwStream, NULL );
	if ( ret )
	{
		LOG_INFO "check crc fail\n" LOG_END
		goto UPGRADE_ERROR;
	}

	ret = ugUpgradePackage( fwStream );
	if ( ret )
	{
		LOG_INFO "Upgrade failed.\n" LOG_END
		goto UPGRADE_ERROR;
	}

#if defined(CFG_NOR_ENABLE) && CFG_NOR_CACHE_SIZE > 0
	LOG_INFO "Flushing NOR cache...\n" LOG_END
	        ioctl( ITP_DEVICE_NOR, ITP_IOCTL_FLUSH, NULL );
#endif

#ifdef CFG_NAND_ENABLE
	ioctl( ITP_DEVICE_NAND, ITP_IOCTL_FLUSH, ITP_NAND_FTL_MODE );
#endif

	LOG_INFO "upgrade success!\n" LOG_END

#if defined(CFG_UPGRADE_DELAY_AFTER_FINISH) && CFG_UPGRADE_DELAY_AFTER_FINISH > 0
	sleep( CFG_UPGRADE_DELAY_AFTER_FINISH );
#endif

	//Delete UgPercentage_handler Timer
	timer_delete( timer2 );

	//Upgrade OK
	ACK[0] = ACK220;
	write( UPGRADE_UART_PORT, ACK, 1 );

	exit( 0 );
	while ( 1 )
		;

UPGRADE_ERROR:
	//Delete UgPercentage_handler Timer
	timer_delete( timer2 );

	//Upgrade Fail
	ACK[0] = ACK221;
	write( UPGRADE_UART_PORT, ACK, 1 );

	exit( ret );
	while ( 1 )
		;

	return 0;
}

static void DetectUartPattern( void )
{
	unsigned char tmp[1024] = { 0 };
	unsigned char Ack[1] = { 0 };
	int readLen = 0, i = 0;
	unsigned int detect_count = 2000000;
	bool upgradeFlag = false;

	printf( "DetectUartPattern start\n" );

	itpRegisterDevice( UPGRADE_UART_PORT, &UPGRADE_UART_DEVICE );
	ioctl( UPGRADE_UART_PORT, ITP_IOCTL_RESET, (void*)UPGRADE_UART_BAUDRATE );

	while ( detect_count )
	{
		readLen = read( UPGRADE_UART_PORT, tmp, 1024 );
		if ( readLen > 0 && upgradeFlag == false )
		{
			for ( i = 0; i < readLen; i++ )
			{
				if ( tmp[i] == UPGRADE_PATTERN )
					upgradeFlag = true;
			}

			if ( upgradeFlag == true )
			{
				Ack[0] = ACK20;
				write( UPGRADE_UART_PORT, Ack, 1 );
				//sleep(1);
			}
		}

		if ( upgradeFlag == true && readLen == 0 )
		{
			//For empty in RX FIFO
			break;
		}
		else
		{
			detect_count--;
		}
	}

	if ( upgradeFlag == true )
	{
		LOG_INFO "Upgrade FW by Uart\n" LOG_END
#if (UPGRADE_UART_PORT == ITP_DEVICE_UART0)
		ithUartSetRxTriggerLevel( ITH_UART0_BASE, ITH_UART_TRGL0 );
#elif (UPGRADE_UART_PORT == ITP_DEVICE_UART1)
		ithUartSetRxTriggerLevel( ITH_UART1_BASE, ITH_UART_TRGL0 );
#endif
		UpgradeUartPackage();
	}

	printf( "DetectUartPattern end, detect_count=%d\n", detect_count );

}

static int UpgradeUartPackage_customized( unsigned int pkglength )
{
	int ret = 0, i = 0, j = 0;
	ITCStream* fwStream = NULL;
	unsigned char crc8_cmd = 0;
	static unsigned char tmp[2048] = { 0 };
	unsigned char *pkgData = NULL;
	unsigned char fota_header[] = { '$', 'C', 'M', 'C' };
	unsigned char Ack_frame[10] = { 0 };
	unsigned int tmpTotalSize = 0;
	unsigned int PkgSize = 0, readLen = 0;
	unsigned int Pkg_Maxnum = 0, Pkg_Index = 0, PkgSize_remain = 0, Pkg_checksize = 0;
	unsigned char errortype = 0, totaltype = 0;
	unsigned int Pkg_crc16 = 0, Pkg_crc16_cal = 0, Pkg_Index_w = 0;

	// timer_t timer, timer2;
	// struct itimerspec value, value2;

	Ack_frame[0] = '$';
	Ack_frame[1] = 'C';
	Ack_frame[2] = 'M';
	Ack_frame[3] = 'C';
	Ack_frame[4] = 0x04;
	Ack_frame[5] = 0x00;
	Ack_frame[7] = 0x00;
	Ack_frame[9] = '&';

	// Ack_frame[0] = 0x24;
	// Ack_frame[1] = 0x43;
	// Ack_frame[2] = 0x4D;
	// Ack_frame[3] = 0x43;
	// Ack_frame[4] = 0x04;
	// Ack_frame[5] = 0x00;
	// Ack_frame[6] = 0x87;
	// Ack_frame[7] = 0x80;
	// Ack_frame[8] = 0xD7;
	// Ack_frame[9] = 0x26;

	//Init UartTimeout_handler
	// for (i = 0; i < 10; i++)
	// {
	//     gTimeState[i] = WAIT_SIZE;
	// }
	// memset(tmp, 0, 2048);
	// timer_create(CLOCK_REALTIME, NULL, &timer);
	// timer_connect(timer, (VOIDFUNCPTR)UartTimeout_handler, 0);
	// value.it_value.tv_sec = value.it_interval.tv_sec = 1;
	// value.it_value.tv_nsec = value.it_interval.tv_nsec = 0;
	// timer_settime(timer, 0, &value, NULL);

	while ( 1 )
	{
		switch ( gState )
		{
			case WAIT_SIZE:
				PkgSize = pkglength;
				// printf("******PkgSize=0x%X..\t\n", PkgSize);
				gState = WAIT_SIZE_ACK;

				break;
			case WAIT_SIZE_ACK:
				//allocate memory for pkgData
				pkgData = (unsigned char *)malloc( PkgSize );
				if ( pkgData == NULL )
				{
					printf( "pkgData cannot be allocated\n" );
					ret = -1;
					exit( ret );
				}
				memset( pkgData, 0, PkgSize );
				Pkg_Maxnum = PkgSize / 1024;
				// Pkg_Maxnum = (PkgSize >> 10);
				PkgSize_remain = PkgSize % 1024;
				// PkgSize_remain = PkgSize & ((unsigned int)0x000003ff);
				if ( PkgSize_remain ) //1024
				{
					Pkg_Maxnum++;
				}
				else
				{
					PkgSize_remain = 1024;
				}
				Pkg_Index = 1;
				Pkg_Index_w = 1;
				gTotalSize = 0;
				if ( Pkg_Index >= Pkg_Maxnum )
				{
					Pkg_checksize = PkgSize_remain + 13;
				}
				else
				{
					Pkg_checksize = 1024 + 13;
				}
				printf( "upgradeState:READY\t\n" );
				printf( "TotalPkgMaxnum:0x%02X\t\n", Pkg_Maxnum );
				printf( "Pkg,Size\t\n" );
				printf( "0x%02X,0x%02X\t\n", Pkg_Index, Pkg_checksize );
				gState = READY;
				break;

			case READY:
				readLen = read( UPGRADE_UART_PORT, tmp + tmpTotalSize, 2048 );
				tmpTotalSize += readLen;
				// printf("Pkg_Index:0x%02X..\t\n",Pkg_Index);
				printf( "pkgrds:%d\t\n", tmpTotalSize );
				// printf("Pkg_checksize:0x%02X..\t\n",Pkg_checksize);
				if ( tmpTotalSize >= Pkg_checksize )
				{
					// printf("Pkg_Maxnum:0x%02X..\t\n",Pkg_Maxnum);
					// printf("Pkg_Index:0x%02X..\t\n",Pkg_Index);
					// printf("tmpTotalSize:%d..\t\n",tmpTotalSize);
					// for (i = 0; i < tmpTotalSize;i++)
					// {
					//     printf("tmp[%d]=0x%02x\t\n", i,tmp[i]);
					// }
					for ( i = 0; i < tmpTotalSize; i++ )
					{
						if ( memcmp( &tmp[i], fota_header, 4 ) == 0 )
						{
							// if ((tmp[i + Pkg_checksize - 2] == niu_CRC8(&tmp[i], Pkg_checksize-2)) && （tmp[i + Pkg_checksize - 1] == '&'))
							if ( tmp[i + 6] == 0x06 && (tmp[i + Pkg_checksize - 1] == '&') )
							{
								crc8_cmd = niu_CRC8( &tmp[i], Pkg_checksize - 2 );
								// printf("CRC8[%d]=0x%02x..\t\n", Pkg_checksize - 2, crc8_cmd);
								if ( (tmp[i + Pkg_checksize - 2] == crc8_cmd) )
								{
									Pkg_Index = (unsigned int)tmp[i + 10] << 24;
									Pkg_Index += (unsigned int)tmp[i + 9] << 16;
									Pkg_Index += (unsigned int)tmp[i + 8] << 8;
									Pkg_Index += (unsigned int)tmp[i + 7] << 0;
									totaltype = 1;
									// printf("******tmp[7]=0x%02X,tmp[8]=0x%02X,tmp[9]=0x%02X,tmp[10]=0x%02X..\t\n", tmp[7],tmp[8],tmp[9],tmp[10]);
									if ( Pkg_Index_w != Pkg_Index )
									{
										// printf("******error:PkgIndex_w=0x%02X,PkgIndex=0x%02X..\t\n", Pkg_Index_w,Pkg_Index);
										printf( "log:0x%02X,0x%02X\t\n", Pkg_Index_w, Pkg_Index );
										errortype = 2;
										Ack_frame[7] = 0x10;
									}
									else
									{
										// usleep(5000);
										Ack_frame[6] = tmp[i + 6] | 0x80;
										Ack_frame[8] = niu_CRC8( &Ack_frame[0], 8 );
										write( UPGRADE_UART_PORT, Ack_frame, 10 );
										// for (j = 0; j < 10;j++)
										// {
										//     printf("Ack_frame[%d]=0x%02X..\t\n", j,Ack_frame[j]);
										// }
										memcpy( (pkgData + gTotalSize), &tmp[i + 11], Pkg_checksize - 13 );
										gTotalSize += (Pkg_checksize - 13);

										// printf("Pkg_Index=0x%02X..\t\n", Pkg_Index);
										// printf("Pkg_Index_size=0x%02X..\t\n", Pkg_checksize - 13);
										// printf("gTotalSize=0x%02X,PkgSize=0x%02X..\t\n", gTotalSize, PkgSize);
										// printf("Pkg_Index=0x%02X,Pkg_Maxnum=0x%02X..\t\n", Pkg_Index, Pkg_Maxnum);

										if ( Pkg_Index == Pkg_Maxnum || gTotalSize == PkgSize )
										{
											printf( "Ready to Check Pkg crc16\t\n" );
											gState = WAIT_CHECKSUM;
											break;
										}
										Pkg_Index++;
										Pkg_Index_w++;
									}
									if ( Pkg_Index_w >= Pkg_Maxnum )
									{
										Pkg_checksize = PkgSize_remain + 13;
									}
									else
									{
										Pkg_checksize = 1024 + 13;
									}
									// printf("Next Pkg_Index:0x%02X,", Pkg_Index_w);
									// printf("tmpTotalSize:%d..\t\n", tmpTotalSize);
									// printf("Pkg_checklength:0x%02X..\t\n", Pkg_checksize);
									printf( "0x%02X,0x%02X\t\n", Pkg_Index, Pkg_checksize );

								}
								else
								{
									printf( "crcerr:%d\t\n", Pkg_Index_w );
									errortype = 1;
									Ack_frame[7] = 0x20;
								}

							}
							else
							{
								//    if (tmp[i + 6] == 0x05 && (tmp[i + 12] == '&'))
								//    {

								//    }
								if ( (i + Pkg_checksize) <= tmpTotalSize )
								{
									totaltype = 1;
								}
							}

							if ( errortype == 1 || errortype == 2 )
							{
								Ack_frame[6] = tmp[i + 6] | 0xC0;
								Ack_frame[8] = niu_CRC8( &Ack_frame[0], 8 );
								write( UPGRADE_UART_PORT, Ack_frame, 10 );
								printf( "size=%d,head=%d,", tmpTotalSize, i );
								printf( "err=%d\t\n", errortype );
								// for (j = 0; j < 10; j++)
								// {
								//     printf("sd[%d]=0x%02X..\t\n", j, Ack_frame[j]);
								// }
								errortype = 0;
							}
						}
					}
					if ( totaltype )
					{
						tmpTotalSize = 0;
						totaltype = 0;
					}
					// tmpTotalSize = 0;
					// memset(tmp, 0, 2048);
				}
				// printf("exit get_Pkg ..\t\n");
				break;

			case WAIT_CHECKSUM:
				readLen = read( UPGRADE_UART_PORT, tmp + tmpTotalSize, 2048 );
				tmpTotalSize += readLen;
				// printf("tmpTotalSize:%d..\t\n", tmpTotalSize);
				// printf("check crc16\t\n");
				if ( tmpTotalSize >= 11 )
				{
					// printf("tmpTotalSize:%d..\n",tmpTotalSize);
					// for (i = 0; i < tmpTotalSize;i++)
					// {
					//     printf("tmp[%d]=0x%02X\t\n", i,tmp[i]);
					// }
					printf( "crc:%d", tmpTotalSize );
					for ( i = 0; i < tmpTotalSize; i++ )
					{
						// printf("tmp[%d]=0x%02x\t\n", i, tmp[i]);
						printf( " 0x%02x", tmp[i] );
					}
					printf( "\t\n" );
					for ( i = 0; i < tmpTotalSize; i++ )
					{
						if ( memcmp( &tmp[i], fota_header, 4 ) == 0 )
						{
							if ( tmp[i + 6] == 0x07 )
							{
								// printf("find available frame %d..\t\n",i);
								if ( tmp[i + 9] == niu_CRC8( &tmp[i], 9 ) && tmp[i + 10] == '&' )
								{
									// printf("find available command 0x07..\t\n");
									Pkg_crc16 = (unsigned int)tmp[i + 8] << 8;
									Pkg_crc16 += (unsigned int)tmp[i + 7];
									// printf( "PkgSize=0x%X,", PkgSize );
									// printf("calculate Pkg crc16,starting..\t\n");
									// Pkg_crc16_cal = niu_CRC16( pkgData, PkgSize, 0 );
									Pkg_crc16_cal = niu_CS16( pkgData, PkgSize, 0 );
									// printf("calculate Pkg crc16,finish..\t\n");									
									printf( "PkgSize=0x%X,", PkgSize );
									// printf( "Pkgcrc16=0x%04X,", Pkg_crc16 );
									// printf( "Pkgcrc16_cal=0x%04X\t\n", Pkg_crc16_cal );
									printf( "Pkgcs16=0x%04X,", Pkg_crc16 );
									printf( "Pkgcs16_cal=0x%04X\t\n", Pkg_crc16_cal );

				#ifdef USB_PKG_RECORD
									UARTPKG_FD = fopen( "A:/Test.PKG", "a" );
									if ( UARTPKG_FD == NULL )
									{
										printf( "UARTPKG_FD fail..\t\n" );
										while ( 1 )
											sleep( 1 );
									}
									else
									{
										fwrite( pkgData, PkgSize, 1, UARTPKG_FD );
										fclose( UARTPKG_FD );
										printf( "UARTPKG_FD success..\t\n" );
									}
				#endif

									if ( Pkg_crc16 == Pkg_crc16_cal )
									{
										Ack_frame[6] = tmp[i + 6] | 0x80;
										Ack_frame[8] = niu_CRC8( &Ack_frame[0], 8 );
										write( UPGRADE_UART_PORT, Ack_frame, 10 );
										// printf( "Check Pkg crc16 success\t\n" );
										printf( "Check Pkg cs16 success\t\n" );
										gState = FINISH;
										break;
									}
									else
									{
										// printf( "Check Pkg crc16 fail\t\n" );
										printf( "Check Pkg cs16 fail\t\n" );
										errortype = 3;
										Ack_frame[7] = 0x10;
									}
									// else
									// {
									//     gState = FAIL;
									// }
								}
								else
								{
									errortype = 2;
									Ack_frame[7] = 0x40;
								}
							}
							else
							{
								errortype = 1;
								Ack_frame[7] = 0x20;
							}
							// if (errortype == 1 || errortype == 2 || errortype == 3)
							if ( errortype > 0 )
							{
								Ack_frame[6] = tmp[i + 6] | 0xC0;
								Ack_frame[8] = niu_CRC8( &Ack_frame[0], 8 );
								write( UPGRADE_UART_PORT, Ack_frame, 10 );
								// for (j = 0; j < 10; j++)
								// {
								//     printf("Ack_frame[%d]=0x%02X..\t\n", j, Ack_frame[j]);
								// }
								printf( "size=%d,head=%d,", tmpTotalSize, i );
								printf( "err=%d\t\n", errortype );
								errortype = 0;
							}
						}
						else
						{
							printf( "Not find frame:%d\t\n", i );
						}

					}
					tmpTotalSize = 0;
				}
				// memset(tmp, 0, 2048);
				break;
			// case WAIT_CHECK_ACK:
			//      break;
			case FINISH:
				break;
			case FAIL:
				break;
			default:
				break;
		}

		if ( gState == FAIL || gState == FINISH )
		{
			// printf("State=%d\n", gState);
			// timer_delete(timer);
			break;
		}
		usleep( 200 );
	}

	if ( gState == FAIL )
	{
		//Download Fail
		LOG_INFO "FAIL state, Uart Connection Error\n" LOG_END
		if ( pkgData )
			free( pkgData );

		exit( -1 );
	}
	else
	{
		//Download Finish
		LOG_INFO "Download Finish state\n" LOG_END
	}

#if 0
	for ( i = 0; i < 512; i++ )
	{
		printf( "%02X ", pkgData[i] );
		if ( ( (i + 1) % 16) == 0 )
		{
			printf( "\n" );
		}
	}
	printf( "\n" );
#endif

	//Init UgPercentage_handler Timer
	// timer_create(CLOCK_REALTIME, NULL, &timer2);
	// timer_connect(timer2, (VOIDFUNCPTR)UgPercentage_handler, 0);
	// value2.it_value.tv_sec = value2.it_interval.tv_sec = 1;
	// value2.it_value.tv_nsec = value2.it_interval.tv_nsec = 0;
	// timer_settime(timer2, 0, &value2, NULL);

	// while(1)
	// {
	//     printf("sleep ..\t\n");
	//     sleep(1);
	// }

	// int n_read = 0;
	// printf("open pkg file..\t\n");
	// UARTPKG_FD = fopen("A:/ITEPKG04.PKG", "r");
	// if (UARTPKG_FD == NULL)
	// {
	//     printf("UARTPKG_FD fail..\t\n");
	// }
	// else
	// {
	//     pkgData = (unsigned char *)malloc(1024 * 1024 * 2);
	//     n_read = fread(pkgData, 1, 1024 * 1024 * 2, UARTPKG_FD);
	//     PkgSize = n_read;
	//     fclose(UARTPKG_FD);
	//     printf("UARTPKG_FD success..\t\n");
	// }

	//Upgrade FW start
	LOG_INFO "Upgrade FW start\n" LOG_END
	        itcArrayStreamOpen( &UartStream, pkgData, PkgSize );
	fwStream = &UartStream.stream;
	if ( !fwStream )
	{
		LOG_INFO "fwStream is NULL\n" LOG_END
		        free( pkgData );
		ret = -1;
		goto UPGRADE_ERROR;
	}

#if defined(CFG_LCD_ENABLE) && defined(CFG_BL_LCD_CONSOLE)
	if ( !blLcdOn )
	{
    #if !defined(CFG_BL_SHOW_LOGO)
		extern uint32_t __lcd_base_a;
		extern uint32_t __lcd_base_b;
		extern uint32_t __lcd_base_c;

		itpRegisterDevice( ITP_DEVICE_SCREEN, &itpDeviceScreen );
		ioctl( ITP_DEVICE_SCREEN, ITP_IOCTL_RESET, NULL );
		ithLcdSetBaseAddrA( (uint32_t)&__lcd_base_a );
		ithLcdSetBaseAddrB( (uint32_t)&__lcd_base_b );

    #ifdef CFG_BACKLIGHT_ENABLE
		itpRegisterDevice( ITP_DEVICE_BACKLIGHT, &itpDeviceBacklight );
		ioctl( ITP_DEVICE_BACKLIGHT, ITP_IOCTL_INIT, NULL );
    #endif// CFG_BACKLIGHT_ENABLE

    #endif// !defined(CFG_BL_SHOW_LOGO)

		ioctl( ITP_DEVICE_SCREEN, ITP_IOCTL_POST_RESET, NULL );
		ioctl( ITP_DEVICE_BACKLIGHT, ITP_IOCTL_RESET, NULL );
		blLcdOn = true;
	}
	// blLcdConsoleOn = true;
    if (!blLcdConsoleOn)
    {        
        itpRegisterDevice(ITP_DEVICE_STD, &itpDeviceLcdConsole);         
        itpRegisterDevice(ITP_DEVICE_LCDCONSOLE, &itpDeviceLcdConsole);        
        ioctl(ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_INIT, NULL);        
        ioctl(ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_CLEAR, NULL);        
        blLcdConsoleOn = true;
    }
#endif // defined(CFG_LCD_ENABLE) && defined(BL_LCD_CONSOLE)

    
	ret = ugCheckCrc(fwStream, NULL);    
	if (ret)
	{
		LOG_INFO "check crc fail\n" LOG_END
		goto UPGRADE_ERROR;
	}

	ret = ugUpgradePackage(fwStream);
	if (ret)
	{
		LOG_INFO "Upgrade failed.\n" LOG_END
		goto UPGRADE_ERROR;
	}
	
#if defined(CFG_NOR_ENABLE) && CFG_NOR_CACHE_SIZE > 0
    LOG_INFO "Flushing NOR cache...\n" LOG_END
    ioctl(ITP_DEVICE_NOR, ITP_IOCTL_FLUSH, NULL);
#endif

#ifdef CFG_NAND_ENABLE
	ioctl(ITP_DEVICE_NAND, ITP_IOCTL_FLUSH, ITP_NAND_FTL_MODE);
#endif

	LOG_INFO "Upgrade success!\n" LOG_END
	
#if defined(CFG_UPGRADE_DELAY_AFTER_FINISH) && CFG_UPGRADE_DELAY_AFTER_FINISH > 0
	// sleep(CFG_UPGRADE_DELAY_AFTER_FINISH);
	// sleep(3);
	usleep(10*1000);
#endif

	//Delete UgPercentage_handler Timer
	// timer_delete(timer2);

	//Upgrade OK    
    Ack_frame[6] = 0x08;
    Ack_frame[7] = 0xFF;
    Ack_frame[8] = niu_CRC8(&Ack_frame[0], 8);    
    write(UPGRADE_UART_PORT, Ack_frame, 10);
   
    exit(0);
	while(1);

UPGRADE_ERROR:
	// Delete UgPercentage_handler Timer
	// timer_delete(timer2);
	
	// Upgrade Fail	
    Ack_frame[6] = 0x08;
    Ack_frame[7] = 0x00;
    Ack_frame[8] = niu_CRC8(&Ack_frame[0], 8);    
    write(UPGRADE_UART_PORT, Ack_frame, 10);
	
	exit(ret);
	while(1);

	return 0;
}


static void DetectUartPattern_customized(void)
{
    #define VALUE_WAIT_MS   50   
    #define VALUE_PMS       1000
    #define VALUE_DELAY_US  1000    

	unsigned char tmp[256] = {0};
    unsigned char fota_header[] = {'$','C','M','C',0x07,0x00,0x05};
	
    unsigned char errortype = 0,crc8_cmd = 0;
    unsigned char Ack_frame[10] = {0};
    unsigned char Ack_frame_test[10] = {0};
    int readLen = 0, i = 0;
    unsigned int detect_count = VALUE_WAIT_MS*VALUE_PMS; //60*60*1000;//500;
    unsigned int pkg_size = 0, tmpTotalSize = 0, printf_count = 0, detectcfg = 0;
    bool upgradeFlag = false;

    printf("DetectUart start.\t\n");
    printf("DetectUartPattern time:%dms\t\n", detect_count / VALUE_PMS);
    // itpRegisterDevice(UPGRADE_UART_PORT, &UPGRADE_UART_DEVICE);
    // ioctl(UPGRADE_UART_PORT, ITP_IOCTL_RESET, (void*)UPGRADE_UART_BAUDRATE);
    // ioctl(UPGRADE_UART_PORT, ITP_IOCTL_RESET, (void*)2000000);
    itpRegisterDevice(ITP_DEVICE_UART1, &itpDeviceUart1);   
    // ioctl(ITP_DEVICE_UART1, ITP_IOCTL_UART_SET_BOOT, (void *)false); 
    ioctl(ITP_DEVICE_UART1, ITP_IOCTL_RESET, (void*)2000000);

    Ack_frame[0] = '$';
    Ack_frame[1] = 'C';
    Ack_frame[2] = 'M';
    Ack_frame[3] = 'C';
    Ack_frame[4] = 0x04;
    Ack_frame[5] = 0x00;
    // Ack_frame[7] = 0x00;
    Ack_frame[9] = '&'; 
    detect_count /= VALUE_DELAY_US;

    ithGpioEnable(14);
    ithGpioSet(14);
    // ithGpioClear(14);
    ithGpioSetOut(14);
    #ifdef CFG_UART_DMA
        #warning CFG_UART_DMA defined
        // printf("CFG_UART_DMA defined.\t\n");
    #elif defined(CFG_UART_INTR)
        #warning CFG_UART_INTR defined
        // printf("CFG_UART_INTR defined.\t\n");
    #else
        #warning CFG_UART_NULL defined
        // printf("CFG_UART_NULL defined.\t\n");
    #endif
    while (detect_count)
    {
		readLen = read(UPGRADE_UART_PORT, tmp+tmpTotalSize, 256);
        // readLen = read(ITP_DEVICE_UART1, tmp+tmpTotalSize, 256);
        
        tmpTotalSize += readLen;
        
        // printf("upgradeFlag:0x%02X..\t\n", upgradeFlag);        
        if (tmpTotalSize > 12 && upgradeFlag == false)
        {
            printf("rds:%d\t\n", tmpTotalSize);
            // printf("%d:", tmpTotalSize);            
            // for (i = 0; i < tmpTotalSize; i++)
            // {
            //     // printf("tmp[%d]=0x%02x\t\n", i, tmp[i]);
            //     printf(" 0x%02x", tmp[i]);
            // }
            // printf("\t\n");
            for(i = 0; i < tmpTotalSize; i++) 
            {
                if (memcmp(&tmp[i], &fota_header[0], 4) == 0 && tmp[i + 12] == '&')
                {
                    if (tmp[i + 6] == 0x05 && tmp[i + 12] == '&')
                    {
                        // printf("$CMC=%d..\t\n",i);
                        crc8_cmd = niu_CRC8(&tmp[i], 11);
                        if (tmp[i + 11] == crc8_cmd)
                        {
                            upgradeFlag = true;
                            errortype = 0;           
                            break;
                        }
                        else
                        {
                            errortype = 2; 
                            Ack_frame[7] = 0x80;  
                        } 
                    }                   
                    if (errortype > 0 && upgradeFlag == false)
                    {                         
                        Ack_frame[6] = tmp[i + 6] | 0xC0;                    
                        Ack_frame[8] = niu_CRC8(&Ack_frame[0], 8);
                        write(UPGRADE_UART_PORT, Ack_frame, sizeof(Ack_frame)); 
                        printf("err:%d\t\n",errortype);      
                        errortype = 0;                                           
                    }
                }
            }
            if (upgradeFlag == true)
            {
                // Ack_frame[6] = 0x05 | 0x80;
                Ack_frame[6] = tmp[i + 6] | 0x80;
                Ack_frame[7] = 0x00;
                Ack_frame[8] = niu_CRC8(&Ack_frame[0], 8);
                write(UPGRADE_UART_PORT, Ack_frame, sizeof(Ack_frame));
                pkg_size = (unsigned int)tmp[i + 10] << 24;
                pkg_size += (unsigned int)tmp[i + 9] << 16;
                pkg_size += (unsigned int)tmp[i + 8] << 8;
                pkg_size += (unsigned int)tmp[i + 7] << 0;
                // printf("upgradeFlag:0x%02X..\t\n", upgradeFlag);
                printf("pkgsize:0x%02X..\t\n", pkg_size);
                // printf("upgrade 0..\t\n");
                break;
            }
            // tmpTotalSize = 0;
        }
        if (upgradeFlag == true && readLen == 0)
        {
            //For empty in RX FIFO
            // printf("upgrade ......\t\n");
            break;
        }
        else 
        {
			detect_count--;
            tmpTotalSize = 0;
			// memset(tmp, 0, 256); 
            usleep(VALUE_DELAY_US);            
        }       
    }  
    ithGpioClear(14);     
    // upgradeFlag = true;
    if (upgradeFlag == true)
    {
        printf("Detecttime:%dms\t\n", VALUE_WAIT_MS-detect_count);   
        LOG_INFO "Upgrade FW by Uart\t\n" LOG_END
#if (UPGRADE_UART_PORT == ITP_DEVICE_UART0)
		// ithUartSetRxTriggerLevel(ITH_UART0_BASE, ITH_UART_TRGL0);
#elif (UPGRADE_UART_PORT == ITP_DEVICE_UART1)
		// ithUartSetRxTriggerLevel(ITH_UART1_BASE, ITH_UART_TRGL0);
#endif

		UpgradeUartPackage_customized(pkg_size);
	}
	
	printf("Detecttime:%dms\t\n", VALUE_WAIT_MS-detect_count);
}
#endif
#endif

void* BootloaderMain(void* arg)
{
    int ret = 0;

#if defined(CFG_UPGRADE_PRESSKEY) || defined(CFG_UPGRADE_RESET_FACTORY) || defined(CFG_UPGRADE_RECOVERY)
    ITPKeypadEvent ev;
#endif

#ifdef CFG_WATCHDOG_ENABLE
    ioctl(ITP_DEVICE_WATCHDOG, ITP_IOCTL_DISABLE, NULL);
#endif

    ithMemDbgDisable(ITH_MEMDBG0);
    ithMemDbgDisable(ITH_MEMDBG1);

#ifdef CFG_BL_SHOW_LOGO
    ioctl(ITP_DEVICE_SCREEN, ITP_IOCTL_POST_RESET, NULL);
    ShowLogo();
#ifdef CFG_BACKLIGHT_ENABLE
    {
        pthread_t       task;
        pthread_create(&task, NULL, backlightResetTask, NULL);
    }
#endif // CFG_BACKLIGHT_ENABLE
    blLcdOn = true;
#endif // CFG_BL_SHOW_LOGO

#ifdef CFG_UPGRADE_USB_DEVICE
	#if 0
    if (DetectUsbDeviceMode())
    {
        ITCStream* upgradeFile = OpenUsbDevicePackage();
        if (upgradeFile)
        {
            if (ugCheckCrc(upgradeFile, NULL))
                LOG_ERR "Upgrade failed.\n" LOG_END
            else
                ret = ugUpgradePackage(upgradeFile);

            #if defined(CFG_NOR_ENABLE) && CFG_NOR_CACHE_SIZE > 0
                LOG_INFO "Flushing NOR cache...\n" LOG_END
                ioctl(ITP_DEVICE_NOR, ITP_IOCTL_FLUSH, NULL);
            #endif

			#ifdef CFG_NAND_ENABLE
				ioctl(ITP_DEVICE_NAND, ITP_IOCTL_FLUSH, ITP_NAND_FTL_MODE);
			#endif
			
            if (ret)
                LOG_INFO "Upgrade failed.\n" LOG_END
            else
                LOG_INFO "Upgrade finished.\n" LOG_END

            exit(ret);
        }
	}
	#endif
#endif // CFG_UPGRADE_USB_DEVICE

#if defined(CFG_UPGRADE_PRESSKEY) || defined(CFG_UPGRADE_RESET_FACTORY) || defined(CFG_UPGRADE_RECOVERY)
    ioctl(ITP_DEVICE_KEYPAD, ITP_IOCTL_PROBE, NULL);
    if (read(ITP_DEVICE_KEYPAD, &ev, sizeof (ITPKeypadEvent)) == sizeof (ITPKeypadEvent))
    {
        int key = ev.code, delay = 0;

#endif // defined(CFG_UPGRADE_PRESSKEY) || defined(CFG_UPGRADE_RESET_FACTORY) || defined(CFG_UPGRADE_RECOVERY)

    #ifdef CFG_UPGRADE_RECOVERY
        if (key == CFG_UPGRADE_RECOVERY_PRESSKEY_NUM)
        {
            struct timeval time = ev.time;

            // detect key pressed time
            for (;;)
            {
                if (ev.flags & ITP_KEYPAD_UP)
                    break;

                ioctl(ITP_DEVICE_KEYPAD, ITP_IOCTL_PROBE, NULL);
                if (read(ITP_DEVICE_KEYPAD, &ev, sizeof (ITPKeypadEvent)) == 0)
                    continue;

                delay += itpTimevalDiff(&time, &ev.time);
                time = ev.time;

                LOG_DBG "recovery key: time=%ld.%ld,code=%d,down=%d,up=%d,repeat=%d,delay=%d\r\n",
                    ev.time.tv_sec,
                    ev.time.tv_usec / 1000,
                    ev.code,
                    (ev.flags & ITP_KEYPAD_DOWN) ? 1 : 0,
                    (ev.flags & ITP_KEYPAD_UP) ? 1 : 0,
                    (ev.flags & ITP_KEYPAD_REPEAT) ? 1 : 0,
                    delay
                LOG_END

                if (delay >= CFG_UPGRADE_RECOVERY_PRESSKEY_DELAY)
                    break;
            };

            if (delay >= CFG_UPGRADE_RECOVERY_PRESSKEY_DELAY)
            {
                ITCStream* upgradeFile;
            #ifdef CFG_UPGRADE_RECOVERY_LED
                int fd = open(":led:" CFG_UPGRADE_RECOVERY_LED_NUM, O_RDONLY);
                ioctl(fd, ITP_IOCTL_FLICKER, (void*)500);
            #endif

                // output messages to LCD console
            #if defined(CFG_LCD_ENABLE) && defined(CFG_BL_LCD_CONSOLE)
                if (!blLcdConsoleOn)
                {
                    itpRegisterDevice(ITP_DEVICE_STD, &itpDeviceLcdConsole);
                    itpRegisterDevice(ITP_DEVICE_LCDCONSOLE, &itpDeviceLcdConsole);
                    ioctl(ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_INIT, NULL);
                    ioctl(ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_CLEAR, NULL);
                    blLcdConsoleOn = true;
                }
                if (!blLcdOn)
                {
                    ioctl(ITP_DEVICE_SCREEN, ITP_IOCTL_POST_RESET, NULL);
                    ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_RESET, NULL);
                    blLcdOn = true;
                }
            #endif // defined(CFG_LCD_ENABLE) && defined(BL_LCD_CONSOLE)

                LOG_INFO "Do Recovery...\r\n" LOG_END

                InitFileSystem();

                upgradeFile = OpenRecoveryPackage();
                if (upgradeFile)
                {
                    if (ugCheckCrc(upgradeFile, NULL))
                        LOG_ERR "Recovery failed.\n" LOG_END
                    else
                        ret = ugUpgradePackage(upgradeFile);

                    #if defined(CFG_NOR_ENABLE) && CFG_NOR_CACHE_SIZE > 0
                        LOG_INFO "Flushing NOR cache...\n" LOG_END
                        ioctl(ITP_DEVICE_NOR, ITP_IOCTL_FLUSH, NULL);
                    #endif

				    #ifdef CFG_NAND_ENABLE
				        ioctl(ITP_DEVICE_NAND, ITP_IOCTL_FLUSH, ITP_NAND_FTL_MODE);
				    #endif
				    
                    if (ret)
                        LOG_INFO "Recovery failed.\n" LOG_END
                    else
                        LOG_INFO "Recovery finished.\n" LOG_END

                    exit(ret);
                }
                else
                {
                #ifdef CFG_UPGRADE_RECOVERY_LED
                    ioctl(fd, ITP_IOCTL_OFF, NULL);
                #endif
                }
                while (1);
            }
        }
    #endif // CFG_UPGRADE_RECOVERY

    #ifdef CFG_UPGRADE_RESET_FACTORY
        if (key == CFG_UPGRADE_RESET_FACTORY_PRESSKEY_NUM)
        {
            struct timeval time = ev.time;

            // detect key pressed time
            for (;;)
            {
                if (ev.flags & ITP_KEYPAD_UP)
                    break;

                ioctl(ITP_DEVICE_KEYPAD, ITP_IOCTL_PROBE, NULL);
                if (read(ITP_DEVICE_KEYPAD, &ev, sizeof (ITPKeypadEvent)) == 0)
                    continue;

                delay += itpTimevalDiff(&time, &ev.time);
                time = ev.time;

                LOG_DBG "reset key: time=%ld.%ld,code=%d,down=%d,up=%d,repeat=%d,delay=%d\r\n",
                    ev.time.tv_sec,
                    ev.time.tv_usec / 1000,
                    ev.code,
                    (ev.flags & ITP_KEYPAD_DOWN) ? 1 : 0,
                    (ev.flags & ITP_KEYPAD_UP) ? 1 : 0,
                    (ev.flags & ITP_KEYPAD_REPEAT) ? 1 : 0,
                    delay
                LOG_END

                if (delay >= CFG_UPGRADE_RESET_FACTORY_PRESSKEY_DELAY)
                    break;
            };

            if (delay >= CFG_UPGRADE_RESET_FACTORY_PRESSKEY_DELAY)
            {
                // output messages to LCD console
            #if defined(CFG_LCD_ENABLE) && defined(CFG_BL_LCD_CONSOLE)
                if (!blLcdConsoleOn)
                {
                    itpRegisterDevice(ITP_DEVICE_STD, &itpDeviceLcdConsole);
                    itpRegisterDevice(ITP_DEVICE_LCDCONSOLE, &itpDeviceLcdConsole);
                    ioctl(ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_INIT, NULL);
                    ioctl(ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_CLEAR, NULL);
                    blLcdConsoleOn = true;
                }
                if (!blLcdOn)
                {
                    ioctl(ITP_DEVICE_SCREEN, ITP_IOCTL_POST_RESET, NULL);
                    ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_RESET, NULL);
                    blLcdOn = true;
                }
            #endif // defined(CFG_LCD_ENABLE) && defined(BL_LCD_CONSOLE)

                LOG_INFO "Do Reset...\r\n" LOG_END

                InitFileSystem();
                ioctl(ITP_DEVICE_DRIVE, ITP_IOCTL_MOUNT, NULL);

                ret = ugResetFactory();

            #if defined(CFG_NOR_ENABLE) && CFG_NOR_CACHE_SIZE > 0
                LOG_INFO "Flushing NOR cache...\n" LOG_END
                ioctl(ITP_DEVICE_NOR, ITP_IOCTL_FLUSH, NULL);
            #endif

			#ifdef CFG_NAND_ENABLE
				ioctl(ITP_DEVICE_NAND, ITP_IOCTL_FLUSH, ITP_NAND_FTL_MODE);
			#endif
			
                if (ret)
                    LOG_INFO "Reset failed.\n" LOG_END
                else
                    LOG_INFO "Reset finished.\n" LOG_END

                exit(ret);
                while (1);
            }
        }
    #endif // CFG_UPGRADE_RESET_FACTORY

    #ifdef CFG_UPGRADE_PRESSKEY
        if (key == CFG_UPGRADE_PRESSKEY_NUM)
        {
            struct timeval time = ev.time;

            // detect key pressed time
            for (;;)
            {
                if (ev.flags & ITP_KEYPAD_UP)
                    break;

                ioctl(ITP_DEVICE_KEYPAD, ITP_IOCTL_PROBE, NULL);
                if (read(ITP_DEVICE_KEYPAD, &ev, sizeof (ITPKeypadEvent)) == 0)
                    continue;

                delay += itpTimevalDiff(&time, &ev.time);
                time = ev.time;

                LOG_DBG "upgrade key: time=%ld.%ld,code=%d,down=%d,up=%d,repeat=%d,delay=%d\r\n",
                    ev.time.tv_sec,
                    ev.time.tv_usec / 1000,
                    ev.code,
                    (ev.flags & ITP_KEYPAD_DOWN) ? 1 : 0,
                    (ev.flags & ITP_KEYPAD_UP) ? 1 : 0,
                    (ev.flags & ITP_KEYPAD_REPEAT) ? 1 : 0,
                    delay
                LOG_END

                if (delay >= CFG_UPGRADE_PRESSKEY_DELAY)
                    break;
            };

            if (delay >= CFG_UPGRADE_PRESSKEY_DELAY)
            {
                InitFileSystem();
                DoUpgrade();
            }
        }
    #endif // CFG_UPGRADE_PRESSKEY
#if defined(CFG_UPGRADE_PRESSKEY) || defined(CFG_UPGRADE_RESET_FACTORY) || defined(CFG_UPGRADE_RECOVERY)
    }
#endif

#if (CFG_CHIP_FAMILY == 970)
	#if 0
	printf("DetectKey start.\t\n");
	DetectKey();
	#endif
#endif

#if !defined(CFG_UPGRADE_PRESSKEY) && defined(CFG_UPGRADE_OPEN_FILE)
	InitFileSystem();
	if(usbDetected)	DoUpgrade();
#if defined (CFG_ENABLE_UART_CLI)
    CommandReciver();
#endif
#endif

#if defined(CFG_UPGRADE_FROM_UART)
	//Upgrade FW by Uart
	// DetectUartPattern();
    DetectUartPattern_customized();
#endif

#ifdef CFG_UPGRADE_BACKUP_PACKAGE
    if (ugUpgradeCheck())
    {
        // output messages to LCD console
    #if defined(CFG_LCD_ENABLE) && defined(CFG_BL_LCD_CONSOLE)
        if (!blLcdConsoleOn)
        {
            itpRegisterDevice(ITP_DEVICE_STD, &itpDeviceLcdConsole);
            itpRegisterDevice(ITP_DEVICE_LCDCONSOLE, &itpDeviceLcdConsole);
            ioctl(ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_INIT, NULL);
            ioctl(ITP_DEVICE_LCDCONSOLE, ITP_IOCTL_CLEAR, NULL);
            blLcdConsoleOn = true;
        }
        if (!blLcdOn)
        {
            ioctl(ITP_DEVICE_SCREEN, ITP_IOCTL_POST_RESET, NULL);
            ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_RESET, NULL);
            blLcdOn = true;
        }
    #endif // defined(CFG_LCD_ENABLE) && defined(BL_LCD_CONSOLE)

        LOG_INFO "Last upgrade failed, try to restore from internal package...\r\n" LOG_END

    #ifndef CFG_UPGRADE_USB_DEVICE

        // init fat
    #ifdef CFG_FS_FAT
        itpRegisterDevice(ITP_DEVICE_FAT, &itpFSDeviceFat.dev);
        ioctl(ITP_DEVICE_FAT, ITP_IOCTL_INIT, NULL);
        ioctl(ITP_DEVICE_FAT, ITP_IOCTL_ENABLE, NULL);
    #endif // CFG_FS_FAT

        // mount disks on booting
        ioctl(ITP_DEVICE_DRIVE, ITP_IOCTL_MOUNT, NULL);

    #endif // !CFG_UPGRADE_USB_DEVICE

        RestorePackage();
    }
#endif // CFG_UPGRADE_BACKUP_PACKAGE

#ifdef CFG_BOOT_TESTBIN_ENABLE
    #if !defined(CFG_UPGRADE_OPEN_FILE)
    InitFileSystem();
    #endif
    DoBootTestBin();
#endif
    LOG_INFO "Do Booting...\r\n" LOG_END

    BootImage();

    return NULL;
}
/*
#if (CFG_CHIP_PKG_IT9079)
void
ithCodecPrintfWrite(
    char* string,
    int length)
{
    // this is a dummy function for linking with library itp_boot. (itp_swuart_codec.c)
}

#endif
*/
