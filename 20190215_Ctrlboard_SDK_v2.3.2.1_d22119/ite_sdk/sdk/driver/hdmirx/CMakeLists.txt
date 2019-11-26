INCLUDE_DIRECTORIES(
    ${SMEDIA_INC_DIRS}
)

if($ENV{CFG_PLATFORM} STREQUAL win32)
    ADD_DEFINITIONS(
        -DHDMITX_EXPORTS
        )
endif($ENV{CFG_PLATFORM} STREQUAL win32)

if(DEFINED CFG_IT6801)
ADD_LIBRARY(hdmirx
        it6802/edid.c
        it6802/IO.c
        it6802/it680x.c
        it6802/it680x_cec
        it6802/Utility.c
        it6802/it680x_interface.c
        mmp_hdmirx.c
    )
endif()

TARGET_LINK_LIBRARIES(hdmirx iic)