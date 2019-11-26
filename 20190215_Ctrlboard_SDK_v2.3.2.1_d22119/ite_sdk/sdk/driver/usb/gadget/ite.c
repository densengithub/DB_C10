/*
* ite.c -- ITE Composite Gadget Driver
*/
#include <stdio.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/err.h>

#include "f_mass_storage.h"


#define DRIVER_DESC		"ITE Composite Gadget"

static struct usb_composite_overwrite coverwrite = {
    .idVendor = cpu_to_le16(CFG_USB_VENDOR_ID),
	.idProduct = cpu_to_le16(CFG_USB_PRODUCT_ID),
	.bcdDevice = cpu_to_le16(0x0200),
#if defined(CFG_USBD_MASS_STORAGE)
	.serial_number = CFG_USBD_STR_SERIAL_NUMBER,
#endif
	.manufacturer = CFG_USBD_STR_MANUFACTURER,
	.product = CFG_USBD_STR_PRODUCT,
};

/***************************** Device Descriptor ****************************/
#define STRING_DESCRIPTION_IDX		USB_GADGET_FIRST_AVAIL_IDX


#define ITE_VENDOR_ID			0x3014
#define ITE_PRODUCT_ID		    0x0970

static struct usb_device_descriptor device_desc = {
    .bLength = USB_DT_DEVICE_SIZE,
    .bDescriptorType = USB_DT_DEVICE,
    /* .bcdUSB = DYNAMIC */
    .bDeviceClass = USB_CLASS_PER_INTERFACE,
    .idVendor = cpu_to_le16(ITE_VENDOR_ID),
    .idProduct = cpu_to_le16(ITE_PRODUCT_ID),
    /* .iManufacturer = DYNAMIC */
    /* .iProduct = DYNAMIC */
    .bNumConfigurations = 1,
};

static struct usb_string strings_dev[] = {
    [USB_GADGET_MANUFACTURER_IDX].s = "",
    [USB_GADGET_PRODUCT_IDX].s = "",
#if 1//defined(CFG_USBD_MASS_STORAGE)
    [USB_GADGET_SERIAL_IDX].s = "",
#endif
    [STRING_DESCRIPTION_IDX].s = "ITE Configuration",
    {} /* end of list */
};

static struct usb_gadget_strings stringtab_dev = {
    .language = 0x0409,       /* en-us */
    .strings = strings_dev,
};

static struct usb_gadget_strings *dev_strings[] = {
    &stringtab_dev,
    NULL,
};

/****************************** Configurations ******************************/



/*-------------------------------------------------------------------------*/
static struct usb_function *f_msg_cfg;


static struct usb_configuration ite_config_driver = {
	.label		= "ITE Gadget",
	.bConfigurationValue = 1,
	/* .iConfiguration = DYNAMIC */
	.bmAttributes	= USB_CONFIG_ATT_ONE,
	.MaxPower	= 500,
};


static struct usb_function_instance *fi_msg;
static struct usb_function_instance *fi_acm;
static struct usb_function_instance *fi_idb;
static struct usb_function_instance *fi_uvc;

static struct usb_function *f_msg;
static struct usb_function *f_acm;
static struct usb_function *f_idb;
static struct usb_function *f_uvc;

/*-------------------------------------------------------------------------*/
static int ite_do_config(struct usb_configuration *c)
{
	int ret = 0;

#if defined(CFG_USBD_MASS_STORAGE)
	f_msg = usb_get_function(fi_msg);
	if (IS_ERR(f_msg)) {
		ret = PTR_ERR(f_msg);
		goto err;
	}

	ret = usb_add_function(c, f_msg);
	if (ret)
		goto err_func_msg;
#endif

#if defined(CFG_USBD_IDB)
	f_idb = usb_get_function(fi_idb);
	if (IS_ERR(f_idb)) {
		ret = PTR_ERR(f_idb);
		goto err_func_idb;
	}

	ret = usb_add_function(c, f_idb);
	if (ret)
		goto err_func_idb1;
#endif
	
#if defined(CFG_USBD_UVC)
	f_uvc = usb_get_function(fi_uvc);
	if (IS_ERR(f_uvc)) {
		ret = PTR_ERR(f_uvc);
		goto err_func_uvc;
	}

	ret = usb_add_function(c, f_uvc);
	if (ret)
		goto err_func_uvc1;
#endif

#if defined(CFG_USBD_ACM)
    f_acm = usb_get_function(fi_acm);
    if (IS_ERR(f_acm)) {
        ret = PTR_ERR(f_acm);
        goto err_func_acm;
    }

    ret = usb_add_function(c, f_acm);
    if (ret)
        goto err_func_acm1;
#endif

	return 0;

err_func_acm1:
    #if defined(CFG_USBD_ACM)	
    usb_put_function(f_acm);
    #endif
err_func_acm:
    #if defined(CFG_USBD_UVC)	
    usb_remove_function(c, f_uvc);
    #endif
err_func_uvc1:
    #if defined(CFG_USBD_UVC)	
    usb_put_function(f_uvc);
    #endif
err_func_uvc:
    #if defined(CFG_USBD_IDB)	
    usb_remove_function(c, f_idb);
    #endif
err_func_idb1:
    #if defined(CFG_USBD_IDB)
    usb_put_function(f_idb);
    #endif
err_func_idb:
    #if defined(CFG_USBD_MASS_STORAGE)
    usb_remove_function(c, f_msg);
    #endif
err_func_msg:
    #if defined(CFG_USBD_MASS_STORAGE)
    usb_put_function(f_msg);
    #endif
err:
    return ret;
}

static int ite_bind(struct usb_composite_dev *cdev)
{
	struct fsg_opts *fsg_opts;
	int status;
	printf("ite_bind() \n");

	status = usb_string_ids_tab(cdev, strings_dev);
	if (status < 0)
		goto err;
	device_desc.iManufacturer = strings_dev[USB_GADGET_MANUFACTURER_IDX].id;
	device_desc.iProduct = strings_dev[USB_GADGET_PRODUCT_IDX].id;
	ite_config_driver.iConfiguration = strings_dev[STRING_DESCRIPTION_IDX].id;

#if defined(CFG_USBD_MASS_STORAGE)
	device_desc.iSerialNumber = strings_dev[USB_GADGET_SERIAL_IDX].id;
    fi_msg = usb_get_function_instance("mass_storage");
    if (IS_ERR(fi_msg)) {
        status = PTR_ERR(fi_msg);
        goto err;
    }
#endif

#if defined(CFG_USBD_IDB)
    fi_idb = usb_get_function_instance("idb");
    if (IS_ERR(fi_idb)) {
        status = PTR_ERR(fi_idb);
        goto err_fsg_inst;
    }
#endif

#if defined(CFG_USBD_UVC)
    fi_uvc = usb_get_function_instance("uvc");
    if (IS_ERR(fi_uvc)) {
        status = PTR_ERR(fi_uvc);
        goto err_idb_inst;
    }
#endif

#if defined(CFG_USBD_ACM)
    fi_acm = usb_get_function_instance("acm");
    if (IS_ERR(fi_acm)) {
        status = PTR_ERR(fi_acm);
        goto err_uvc_inst;
    }
#endif


	/* finally register the configuration */
	status = usb_add_config(cdev, &ite_config_driver,
			ite_do_config);
	if (status < 0)
        goto err_acm_inst;

	usb_composite_overwrite_options(cdev, &coverwrite);

	return 0;


err_acm_inst:
    #if defined(CFG_USBD_ACM)
    usb_put_function_instance(fi_acm);
    #endif
err_uvc_inst:
    #if defined(CFG_USBD_UVC)
    usb_put_function_instance(fi_uvc);
    #endif
err_idb_inst:
    #if defined(CFG_USBD_IDB)
    usb_put_function_instance(fi_idb);
    #endif
err_fsg_inst:
	#if defined(CFG_USBD_MASS_STORAGE)
    usb_put_function_instance(fi_msg);
	#endif
err:
    return status;
}

static int ite_unbind(struct usb_composite_dev *cdev)
{
#if defined(CFG_USBD_MASS_STORAGE)
    usb_put_function(f_msg);
    usb_put_function_instance(fi_msg);
#endif

#if defined(CFG_USBD_IDB)
    usb_put_function(f_idb);
    usb_put_function_instance(fi_idb);
#endif

#if defined(CFG_USBD_UVC)
    usb_put_function(f_uvc);
    usb_put_function_instance(fi_uvc);
#endif

#if defined(CFG_USBD_ACM)
    usb_put_function(f_acm);
    usb_put_function_instance(fi_acm);
#endif
    printf("ite_unbind() \n");

    return 0;
}

static struct usb_composite_driver ite_driver = {
    .name = "ite_gadget",
    .dev = &device_desc,
    .strings = dev_strings,
    .max_speed = USB_SPEED_HIGH,
    .bind = ite_bind,
    .unbind = ite_unbind,
#if defined(CFG_USBD_MASS_STORAGE)
    .needs_serial = 1, 
#endif
};

int iteUsbRegisterGadget(void)
{
    return usb_composite_probe(&ite_driver);
}

int iteUsbUnRegisterGadget(void)
{
    usb_composite_unregister(&ite_driver);
	return 0;
}
