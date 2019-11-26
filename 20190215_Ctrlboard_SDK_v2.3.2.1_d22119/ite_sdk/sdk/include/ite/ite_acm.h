
#ifndef ITE_ACM_H
#define ITE_ACM_H

#ifdef __cplusplus
extern "C" {
#endif


extern int iteAcmRegister(void);
extern int iteAcmUnRegister(void);

extern int iteAcmTtyIsReadyEx(int index);
extern int iteAcmTtyOpenEx(int index);
extern int iteAcmTtyCloseEx(int index);
extern int iteAcmTtyReadEx(int index, char *buf, int len);
extern int iteAcmTtyGetWriteRoomEx(int index);
extern int iteAcmTtyPutCharEx(int index, unsigned char c);
extern void iteAcmTtyFlushCharsEx(int index);
extern int iteAcmTtyWriteEx(int index, const unsigned char *buf, int len);

extern int iteAcmConsoleWrite(const char *buf, unsigned count);


#define iteAcmTtyIsReady()          iteAcmTtyIsReadyEx(0)

/**
* This routine is called when a particular tty device is opened.
* This routine is mandatory; if this routine is not filled in,
* the attempted open will fail with ENODEV.
*
* @return 
*/
#define iteAcmTtyOpen()                 iteAcmTtyOpenEx(0)

/**
* This routine is called when a particular tty device is closed.
* Note: called even if the corresponding open() failed. 
*
* @return
*/
#define iteAcmTtyClose()                iteAcmTtyCloseEx(0)

/**
* This routine is called to read out the input characters.
* It will block when no character available.
*
* @return the number of characters actually read
*/
#define iteAcmTtyRead(buf, len)         iteAcmTtyReadEx(0, buf, len)

/*
* This routine is called to write a series of
* characters to the tty device.
*
* @return the number of characters actually accepted for writing
*/
#define iteAcmTtyWrite(buf, len)        iteAcmTtyWriteEx(0, buf, len)

/*
* This routine returns the numbers of characters the tty driver
* will accept for queuing to be written. 
*
* @return the numbers of characters the tty driver will accept for queuing to be written.
*/
#define iteAcmTtyGetWriteRoom()         iteAcmTtyGetWriteRoomEx(0)

/*
* This routine is called to write a single
* character to the tty device.  If anyone uses this routine,
* it must call the flush_chars() routine when it is
* done stuffing characters into the driver.  If there is no room
* in the queue, the character is ignored.
*
* @return the number of characters actually accepted for queue
*/
#define iteAcmTtyPutChar(c)             iteAcmTtyPutCharEx(0, c)

/*
* This routine is called after it has written a
* series of characters to the tty device using iteAcmTtyPutChar().  
*/
#define iteAcmTtyFlushChars()           iteAcmTtyFlushCharsEx(0)



#ifdef __cplusplus
}
#endif

#endif // ITE_FSG_H
