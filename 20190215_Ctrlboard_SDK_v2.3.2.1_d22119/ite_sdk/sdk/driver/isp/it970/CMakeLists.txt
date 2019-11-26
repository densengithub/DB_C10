INCLUDE_DIRECTORIES(
    .
    ..
    ${PROJECT_SOURCE_DIR}/sdk/include/
    ${PROJECT_SOURCE_DIR}/sdk/include/ite/it970/
    ${PROJECT_SOURCE_DIR}/sdk/driver/ith/it970/
    ${PROJECT_SOURCE_DIR}/sdk/include/isp
)

ADD_LIBRARY(isp STATIC
    isp.c
    isp_hw.c
    isp_hw_op.c
    isp_queue.c
    isp_util.c
    mmp_isp.c
)
