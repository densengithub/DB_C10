/*
 * Copyright (c) 2011 ITE Tech.Inc.All Rights Reserved.
 */
/** @file
 * Castor hardware abstraction layer definitions.
 *
 * @author Jim Tan
 * @version 1.0
 */

#if (CFG_CHIP_FAMILY == 970 || CFG_CHIP_FAMILY == 960)
    #include "it970/ith_defs.h"
#elif (CFG_CHIP_FAMILY == 9910)
    #include "it9910/ith_defs.h"
#elif (CFG_CHIP_FAMILY == 9850)
    #include "it9850/ith_defs.h"
#elif (CFG_CHIP_FAMILY == 9070)
    #include "it9070/ith_defs.h"
#else
    #error "not defined"
#endif