/*
 * Copyright (C) 2016 Daniel Schwierzeck, daniel.schwierzeck@gmail.com
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <errno.h>
#include <common.h>
#include <dm/device.h>
#include <asm/gpio.h>
#include <asm/io.h>

struct xway_gpio {
	u32	clc;
};

struct xway_gpio_hw {
	unsigned int bank_cnt;
	unsigned int gpio_per_bank;
};

struct xway_gpio_priv {
	struct xway_gpio *regs;
	const struct xway_gpio_hw *hw;
};

static int xway_gpio_get_value(struct udevice *dev, unsigned offset)
{
	struct xway_gpio_priv *priv = dev_get_priv(dev);

	return 0;
}

static int xway_gpio_set_value(struct udevice *dev, unsigned offset,
				 int value)
{
	struct xway_gpio_priv *priv = dev_get_priv(dev);

	return 0;
}

static int xway_gpio_direction_input(struct udevice *dev, unsigned offset)
{
	struct xway_gpio_priv *priv = dev_get_priv(dev);

	return 0;
}

static int xway_gpio_direction_output(struct udevice *dev,
				       unsigned offset, int value)
{
	struct xway_gpio_priv *priv = dev_get_priv(dev);

	return 0;
}

static int xway_gpio_get_function(struct udevice *dev, unsigned offset)
{
	int ret = GPIOF_UNUSED;

	return ret;
}

static const struct xway_gpio_hw xway_gpio_danube = {
	.bank_cnt = 3, .gpio_per_bank = 16,
};

static const struct xway_gpio_hw xway_gpio_grx330 = {
	.bank_cnt = 4, .gpio_per_bank = 16,
};

static const struct xway_gpio_hw xway_gpio_vrx200 = {
	.bank_cnt = 4, .gpio_per_bank = 16,
};

static const struct udevice_id xway_gpio_of_match[] = {
	{
		.compatible = "intel,danube-gpio",
		.data = (ulong)&xway_gpio_danube,
	}, {
		.compatible = "intel,grx330-gpio",
		.data = (ulong)&xway_gpio_grx330,
	}, {
		.compatible = "intel,vrx200-gpio",
		.data = (ulong)&xway_gpio_vrx200,
	},
	{ /* sentinel */ }
};

static const struct dm_gpio_ops xway_gpio_ops = {
	.direction_input	= xway_gpio_direction_input,
	.direction_output	= xway_gpio_direction_output,
	.get_value		= xway_gpio_get_value,
	.set_value		= xway_gpio_set_value,
	.get_function		= xway_gpio_get_function,
};

static int xway_gpio_probe(struct udevice *dev)
{
	struct gpio_dev_priv *uc_priv = dev_get_uclass_priv(dev);
	struct xway_gpio_priv *priv = dev_get_priv(dev);
	const struct xway_gpio_hw *hw =
		(const struct xway_gpio_hw *)dev_get_driver_data(dev);
	fdt_addr_t base;

	base = dev_get_addr(dev);
	if (base == FDT_ADDR_T_NONE)
		return -EINVAL;

	priv->regs = ioremap(base, 0);
	priv->hw = hw;

	uc_priv->gpio_count = hw->bank_cnt * hw->gpio_per_bank;
	uc_priv->bank_name = "GPIO";

	return 0;
}

U_BOOT_DRIVER(xway_gpio) = {
	.name = "intel-xway-gpio",
	.id = UCLASS_GPIO,
	.of_match = xway_gpio_of_match,
	.probe = xway_gpio_probe,
	.priv_auto_alloc_size = sizeof(struct xway_gpio_priv),
	.ops = &xway_gpio_ops,
};