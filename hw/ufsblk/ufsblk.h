/*
 * QEMU UFS
 *
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Written by Jeuk Kim <jeuk20.kim@samsung.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef HW_UFS_UFS_H
#define HW_UFS_UFS_H

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "migration/vmstate.h"
#include "scsi/constants.h"
#include "hw/core/irq.h"
#include "hw/core/sysbus.h"
#include "qemu/qemu-print.h"


#define TYPE_UFSBLK "ufsblk-sysbus"

// 在vexpress.c中添加宏（空闲地址/中断，避开已有外设）
#define VEXPRESS_UFSBLK_BASE 0x20000000  // 寄存器物理基地址
#define VEXPRESS_UFSBLK_IRQ  52          // 中断号（需对应GIC）

struct ufsblk_hci {
    SysBusDevice parent_obj;

    MemoryRegion iomem;
    qemu_irq irq;

    uint32_t fifo_depth; /* FIFO depth in non-compact mode */
};


#define UFSBLK(obj) OBJECT_CHECK(struct ufsblk_hci, (obj), TYPE_UFSBLK)
#endif