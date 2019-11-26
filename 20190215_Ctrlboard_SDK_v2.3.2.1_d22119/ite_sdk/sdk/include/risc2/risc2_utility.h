/** @file
 * RISC2 device library.
 *
 * @author Steven Hsiao
 * @version 1.0
 * @date 2018
 * @copyright ITE Tech. Inc. All Rights Reserved.
 */
/** @defgroup RISC2 Utility API declaration
 *  @{
 */
#ifndef RISC2_UTILITY_API_H
#define RISC2_UTILITY_API_H

#ifdef __cplusplus
extern "C" {
#endif

//GPIO
void setGpioDir(unsigned long gpioPin, unsigned long bIn);

void setGpioMode(unsigned long gpioPin, unsigned long mode);

unsigned long getGpioValue(unsigned long gpioPin, unsigned long bIn);

void setGpioValue(unsigned long gpioPin, unsigned long bHigh);

//TickTimer
unsigned long getCurTimer(int timer);

unsigned long getTimer(int timer);

void resetTimer(int timer);

void stopTimer(int timer);

void startTimer(int timer);

unsigned long getDuration(int timer, unsigned long previousTick);

unsigned long getDurationInUs(int timer, unsigned long previousTick, unsigned long tickPerUs);

#ifdef __cplusplus
}
#endif

#endif // RISC2_UTILITY_API_H
/** @} */ // end of RISC2_UTILITY_API_H
