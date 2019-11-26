/** @file
 * Template project definition.
 *
 */
/** @defgroup template
 *  @{
 */
#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "ite/itp.h"

/******************************************/
/***UART 3****/
#define     RS485_RE_PIN    ITH_GPIO_PIN16
#define     RS485_DE_PIN    ITH_GPIO_PIN17
#define     RS485_RXD_PIN   ITH_GPIO_PIN22
#define     RS485_TXD_PIN   ITH_GPIO_PIN23

#define     Rs485ReceiverEnable()               ithGpioClear(RS485_RE_PIN)
#define     Rs485ReceiverDisable()              ithGpioSet(RS485_RE_PIN)
#define     Rs485TransceiverEnable()            ithGpioSet(RS485_DE_PIN)
#define     Rs485TransceiverDisable()           ithGpioClear(RS485_DE_PIN)

/***UART 1****/
#define     BT_RXD_PIN      ITH_GPIO_PIN18
#define     BT_TXD_PIN      ITH_GPIO_PIN19
#define     BT_EN_PIN       ITH_GPIO_PIN48
#define     BT_CTS_PIN      ITH_GPIO_PIN49
#define     BT_RTS_PIN      ITH_GPIO_PIN50
#define     BT_POWERCTRL_PIN      ITH_GPIO_PIN56

#define     BluetoothModuleEnable()             ithGpioSet(BT_EN_PIN)
#define     BluetoothModuleDisable()            ithGpioClear(BT_EN_PIN)
#define     BluetoothModulPoweron()             ithGpioSet(BT_POWERCTRL_PIN)
#define     BluetoothModulPoweroff()            ithGpioClear(BT_POWERCTRL_PIN)
#define     BluetoothGetRequestToSend()         ithGpioGet(BT_RTS_PIN)
#define     BluetoothClearToSendEnable()        ithGpioSet(BT_CTS_PIN)
#define     BluetoothClearToSendDisable()       ithGpioClear(BT_CTS_PIN)

/***SPI 0****/
#define     CAN_SPI0CLK_PIN     ITH_GPIO_PIN25
#define     CAN_SPI0CS0_PIN     ITH_GPIO_PIN26
#define     CAN_SPI0DO_PIN      ITH_GPIO_PIN27
#define     CAN_SPI0DI_PIN      ITH_GPIO_PIN28
#define     CAN_STB_PIN         ITH_GPIO_PIN39
#define     CAN_STLIP_PIN       ITH_GPIO_PIN40
#define     CAN_INT_PIN         ITH_GPIO_PIN42
#define     CAN_INT0_PIN        ITH_GPIO_PIN43
#define     CAN_INT1_PIN        ITH_GPIO_PIN44


/***UART 0****/
#define     DBG_TXD_PIN         ITH_GPIO_PIN4

/***TFT****/
#define     TFT_BKL_PIN         ITH_GPIO_PIN21
#define     TFT_POWERCTRL_PIN   ITH_GPIO_PIN46
#define     TFT_RST_PIN         ITH_GPIO_PIN24

#define     TFTPoweron()            ithGpioSet(TFT_POWERCTRL_PIN)
#define     TFTPoweroff()           ithGpioClear(TFT_POWERCTRL_PIN)
#define     TFTResetEnable()        ithGpioClear(TFT_RST_PIN)
#define     TFTResetTDisable()      ithGpioSet(TFT_RST_PIN)

/***IIC 1****/
#define     GYRO_SDA_PIN        ITH_GPIO_PIN30
#define     GYRO_SCL_PIN        ITH_GPIO_PIN31
#define     GYRO_MPUINT_PIN     ITH_GPIO_PIN29
#define     GYROGetIntValue()           ithGpioGet(GYRO_MPUINT_PIN)

#define     ALSPS_SDA_PIN       ITH_GPIO_PIN36
#define     ALSPS_SCL_PIN       ITH_GPIO_PIN37
#define     ALSPS_EPLINT_PIN       ITH_GPIO_PIN38
#define     ALSPSGetIntValue()           ithGpioGet(ALSPS_EPLINT_PIN)

/***NTC XAIN3****/
#define     NTC_RT1_PIN         ITH_GPIO_PIN20

/***GPIO****/
#define     MCU_ACC_PIN         ITH_GPIO_PIN51
#define     ACCGetValue()           ithGpioGet(MCU_ACC_PIN)
#define     MCU_BK1_PIN         ITH_GPIO_PIN52
#define     MCU_BK2_PIN         ITH_GPIO_PIN53
#define     MCU_BK3_PIN         ITH_GPIO_PIN54

/***SD 0****/
#define     SD0_CMD_PIN         ITH_GPIO_PIN5
#define     SD0_D0_PIN          ITH_GPIO_PIN7
#define     SD0_D1_PIN          ITH_GPIO_PIN8
#define     SD0_D2_PIN          ITH_GPIO_PIN9
#define     SD0_D3_PIN          ITH_GPIO_PIN10
#define     SD0_INT_PIN         ITH_GPIO_PIN14
#define     SD0_CLK_PIN         ITH_GPIO_PIN15
#define     SD0GetIntValue()         ithGpioGet(SD0_INT_PIN)

#define ARRAY_COUNT_OF(array)   (sizeof (array) / sizeof (array[0]))




/*****************************************/


#ifdef __cplusplus
extern "C" {
#endif


void BspGpioInit(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_GPIO_H */

/** @} */ // end of template