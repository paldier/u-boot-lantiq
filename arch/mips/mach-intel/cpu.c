/*
 * Copyright (C) 2016 Daniel Schwierzeck, daniel.schwierzeck@gmail.com
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <ram.h>
#include <dm.h>

int dram_init(void)
{
	struct ram_info ram;
	struct udevice *dev;
	int err;

	err = uclass_get_device(UCLASS_RAM, 0, &dev);
	if (err) {
		debug("DRAM init failed: %d\n", err);
		return 0;
	}

	err = ram_get_info(dev, &ram);
	if (err) {
		debug("Cannot get DRAM size: %d\n", err);
		return 0;
	}

	debug("SDRAM base=%zx, size=%x\n", ram.base, ram.size);

	return ram.size;
}