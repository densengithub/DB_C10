#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ite/ith.h>
#include <ite/itp.h>
#include "hdmirx/mmp_hdmirx.h"

pthread_mutex_t ghdmi_mutex;

//=============================================================================
//                Constant Definition
//=============================================================================

//=============================================================================
//                Macro Definition
//=============================================================================
typedef void
(*DISABLE_HDCP)(
    BOOL flag);

typedef BOOL
(*POWER_DOWN)(
    BOOL enable);

typedef void
(*INITIALIZE)(
    void);

typedef void
(*OUTPUT_PIN_TRISTATE)(
    BOOL flag);

typedef BOOL
(*IS_SIGNAL_STABLE)(
    void);

typedef unsigned int
(*GET_PROPERTY)(
    MMP_HDMIRX_PROPERTY property);

typedef void
(*SET_PROPERTY)(
    MMP_HDMIRX_PROPERTY property, unsigned int value);

typedef BOOL
(*IS_HDCP_ON)(
    void);

typedef void
(*GetDETiming)(
    MMP_HDMIRX_DETIMING *pDETiming);
//=============================================================================
//                Structure Definition
//=============================================================================
typedef struct HDMI_RX_TAG
{
    INITIALIZE          initialize;
    POWER_DOWN          power_down;
    DISABLE_HDCP        disable_hdcp;
    OUTPUT_PIN_TRISTATE output_pin_tristate;
    IS_SIGNAL_STABLE    is_signal_stable;
    GET_PROPERTY        get_property;
    SET_PROPERTY        set_property;
    IS_HDCP_ON          is_hdcp_on;
    GetDETiming         get_timing;
} HDMI_RX;

//=============================================================================
//                Global Data Definition
//=============================================================================
static void   *gtHDMIRXSemaphore = NULL;

const HDMI_RX gtHdmiRx         =
{
    IT680xInitialize,
    IT680xPowerDown,
    IT680xDisableHDCP,
    IT680xOutputPinTriState,
    IT680xIsSignalStable,
    IT680xGetProperty,
    IT680xSetProperty,
    IT680xIsHDCPOn,
    IT680xGetDETiming,    
};

//=============================================================================
//                Private Function Definition
//=============================================================================

//=============================================================================
//                Public Function Definition
//=============================================================================
//=============================================================================
/**
 * Device Initialize
 */
//=============================================================================
void
ithHDMIRXInitialize(
    void)
{
    uint16_t chipPacketType = 0;
	ithPrintf("ithHDMIRXInitialize\n");
    pthread_mutex_init(&ghdmi_mutex, NULL); 

    pthread_mutex_lock(&ghdmi_mutex);
    gtHdmiRx.power_down(FALSE);
    gtHdmiRx.initialize();
    pthread_mutex_unlock(&ghdmi_mutex);
}

//=============================================================================
/**
 * Device Terminate
 */
//=============================================================================
void
ithHDMIRXTerminate(
    void)
{
    pthread_mutex_lock(&ghdmi_mutex);

    gtHdmiRx.power_down(TRUE);

    pthread_mutex_unlock(&ghdmi_mutex);

}

//=============================================================================
/**
 * Device Output Pin Tri-State.
 */
//=============================================================================
void
ithHDMIRXOutputPinTriState(
    BOOL flag)
{
    pthread_mutex_lock(&ghdmi_mutex);

    gtHdmiRx.output_pin_tristate(flag);

    pthread_mutex_unlock(&ghdmi_mutex);
}

//=============================================================================
/**
 * Device Signal State
 */
//=============================================================================
BOOL
ithHDMIRXIsSignalStable(
    void)
{
    BOOL    bStableState;
    static unsigned int stableStateCount;

    pthread_mutex_lock(&ghdmi_mutex);


    if (gtHdmiRx.is_signal_stable())
        stableStateCount++;
    else
        stableStateCount = 0;

    // we report the signal is stable only when we continously detect the signal
    // stable 3 times. 2 times is not safe.
    bStableState = (stableStateCount > 3);
    //bStableState = gtHdmiRx.is_signal_stable();

    pthread_mutex_unlock(&ghdmi_mutex);

    return bStableState;
}

//=============================================================================
/**
 * Device property.
 */
//=============================================================================
unsigned int
ithHDMIRXGetProperty(
    MMP_HDMIRX_PROPERTY property)
{
    unsigned int value;

    pthread_mutex_lock(&ghdmi_mutex);

    value = gtHdmiRx.get_property(property);
    
    pthread_mutex_unlock(&ghdmi_mutex);

    return value;
}

//=============================================================================
/**
 * Device set property.
 */
//=============================================================================
void
ithHDMIRXSetProperty(
    MMP_HDMIRX_PROPERTY property, unsigned int value)
{
    pthread_mutex_lock(&ghdmi_mutex);

    gtHdmiRx.set_property(property, value);
    
    pthread_mutex_unlock(&ghdmi_mutex);
}

//=============================================================================
/**
 * Check HDCP.
 */
//=============================================================================
BOOL
ithHDMIRXIsHDCPOn(
    void)
{
    unsigned int bHDCPOn;

    pthread_mutex_lock(&ghdmi_mutex);

    bHDCPOn = gtHdmiRx.is_hdcp_on();

    pthread_mutex_unlock(&ghdmi_mutex);

    return bHDCPOn;
}

//=============================================================================
/**
 * Get DE Timing.
 */
//=============================================================================
void
ithHDMIRXGetDETiming(
    MMP_HDMIRX_DETIMING *pDETiming)
{
    pthread_mutex_lock(&ghdmi_mutex);

    gtHdmiRx.get_timing(pDETiming);

    pthread_mutex_unlock(&ghdmi_mutex);
}

//=============================================================================
/**
 * Device power down
 */
//=============================================================================
BOOL
ithHDMIRXPowerDown(
    BOOL enable)
{
    pthread_mutex_lock(&ghdmi_mutex);

    gtHdmiRx.power_down(enable);

    pthread_mutex_unlock(&ghdmi_mutex);

    return TRUE;
}

//=============================================================================
/**
 * Device HDCP contrl
 */
//=============================================================================
void
ithHDMIRXDisableHDCP(
    BOOL flag)
{
    pthread_mutex_lock(&ghdmi_mutex);

    gtHdmiRx.disable_hdcp(flag);

    pthread_mutex_unlock(&ghdmi_mutex);
}

BOOL
ithHDMIRXIsDisableHDCP(
    void)
{
    BOOL flag = TRUE;

    pthread_mutex_lock(&ghdmi_mutex);

#ifdef CFG_IT6607
    flag = IT6607IsDisableHDCP();
#else
    //flag = mmpIT6604IsDisableHDCP();
#endif

    pthread_mutex_unlock(&ghdmi_mutex);

    return flag;
}

BOOL
ithHDMIRXForISRIsDisableHDCP(
    void)
{
    BOOL flag = TRUE;

#ifdef CFG_IT6607
    flag = IT6607IsDisableHDCP();
#else
    //flag = mmpIT6604IsDisableHDCP();
#endif

    return flag;
}

BOOL
ithHDMIRXIsHDCPKeyEmpty(
    void)
{
    BOOL flag = TRUE;

    pthread_mutex_lock(&ghdmi_mutex);

#ifdef CFG_IT6607
    flag = IT6607IsHDCPKeyEmpty();
#else
    //flag = mmpIT6604IsHDCPKeyEmpty();
#endif

    pthread_mutex_unlock(&ghdmi_mutex);

    return flag;
}