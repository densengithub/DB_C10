#if (CFG_CHIP_FAMILY == 970 || CFG_CHIP_FAMILY == 960)
    #include "it970/mmp_isp.h"
#elif (CFG_CHIP_FAMILY == 9850)
    #include "it9850/mmp_isp.h"
#else
    #error "not defined"
#endif