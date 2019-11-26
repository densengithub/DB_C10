INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/sdk/driver/usb/inc
    )

ADD_LIBRARY(usb STATIC
    mmp_usbex.c
    usb_port.c
    usb/hcd.c
    usb/usb.c
    usb/urb.c
    usb/message.c
    usb/config.c
    usb/quirks.c
    usb/hub.c
    usb/driver.c
    usb/hcd/ehci_hcd.c
    )

if (DEFINED CFG_USB_DEVICE_NEW)

    INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/sdk/driver/usb/gadget/function/
        ${PROJECT_SOURCE_DIR}/sdk/driver/usb/gadget/include/
    )

    add_library(usb STATIC
        gadget/composite.c
        gadget/config.c
        gadget/epautoconf.c
        gadget/functions.c
        gadget/u_f.c
        gadget/usbstring.c
        gadget/common.c
        gadget/ite.c
        gadget/udc/core.c
        )

    if (DEFINED CFG_USBD_MASS_STORAGE)
        add_library(usb STATIC
            gadget/function/storage_common.c
            gadget/function/f_mass_storage.c
        )
    endif()
        
    if (DEFINED CFG_USBD_ACM)
        add_library(usb STATIC
            gadget/function/f_acm.c
            gadget/function/u_serial.c
            gadget/function/tty.c
        )
    endif()

else ()

    if (DEFINED CFG_USBD_FILE_STORAGE)
        add_library(usb STATIC
            gadget_leagcy/fsg/file_storage.c
        )
    endif()

    if (DEFINED CFG_USBD_IDB)
    add_definitions(
        -DCONFIG_USBD_IDB=1
    )
    add_library(usb STATIC
        gadget_leagcy/idb/idbInjection.c
        gadget_leagcy/idb/idbProperty.cpp
    )
    else ()
    add_definitions(
    -DCONFIG_USBD_IDB=0
    )

endif()


endif()
