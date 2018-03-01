// SPDX-License-Identifier: GPL-2.0+
/*
 * MOD-USB-UEXT Bridge Driver
 *
 * Copyright (C) 2018 Olimex Ltd.
 * Author(s): Stefan Mavrodiev <stefan@olimex.com>
 */

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>

struct usb_uext {
	struct usb_device	*udev;
	struct usb_interface	*intf;
};

static int usb_uext_probe(struct usb_interface *interface,
			  const struct usb_device_id *id)
{
	struct usb_uext *dev;

	dev = kzalloc(sizeof(*dev), GPF_KERNEL);
	if (!dev)
		return -ENOMEM;

	dev->udev = usb_get_dev(interface_to_usbdev(interface));
	dev->intf = interface;

	/* Save private data */
	usb_set_intfdata(interface, dev);

	return 0;
}

static void usb_uext_disconnect(struct usb_interface *interface)
{
	struct usb_uext *dev = usb_get_intfdata(interface);

	usb_set_intfdata(interface, NULL);
	usb_put_dev(dev->udev);
	kfree(dev);

	dev_dbg(&interface->dev, "device disconnected\n");
}

static const struct usb_device_id usb_uext_table[] = {
	{ USB_DEVICE(0x15ba, 0x0040) },
	{ },
};
MODULE_DEVICE_TABLE (usb, usb_uext_table);

static struct usb_driver usb_uext_driver = {
	.name           = "usb_uext",
	.probe          = usb_uext_probe,
	.disconnect     = usb_uext_disconnect,
	.id_table       = usb_uext_table,
};

module_usb_driver(usb_uext_driver);

MODULE_AUTHOR("Stefan Mavrodiev <stefan@olimex.com>");
MODULE_DESCRIPTION("MOD-USB-UEXT driver");
MODULE_LICENSE("GPL v2");
