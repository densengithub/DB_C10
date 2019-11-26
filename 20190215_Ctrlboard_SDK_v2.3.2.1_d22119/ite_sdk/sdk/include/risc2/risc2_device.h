/** @file
 * RISC2 device library.
 *
 * @author Steven Hsiao
 * @version 1.0
 * @date 2011-2012
 * @copyright ITE Tech. Inc. All Rights Reserved.
 */
/** @defgroup ALT CPU device library
 *  @{
 */
#ifndef RISC2_DEVICE_H
#define RISC2_DEVICE_H

#include "ite/itp.h"
#include "ite/ite_risc.h"
#include "mmio.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _risc2_header {
    unsigned char  *cmd_exchange_addr;
    int            (*entry_point)();
};

#define RISC2_UNKNOWN_DEVICE        0
#define RISC2_TEST_DEVICE           1
#define RISC2_OPUS_CODEC            2

#define ITP_IOCTL_RISC2_SWITCH_ENG  ITP_IOCTL_CUSTOM_CTL_ID0

#define REQUEST_CMD_REG             USER_DEFINE_REG30
#define RESPONSE_CMD_REG            USER_DEFINE_REG31

#define RISC2_COMMAND_REG_READ      ithReadRegA
#define RISC2_COMMAND_REG_WRITE     ithWriteRegA

#ifdef __cplusplus
}
#endif

#endif // RISC2_DEVICE_H
/** @} */ // end of RISC2_device
