#include "ufsblk.h"
#include "qemu/osdep.h"
#include "qapi/error.h"
#include "migration/vmstate.h"
#include "scsi/constants.h"
#include "hw/core/irq.h"
#include "hw/core/sysbus.h"
#include "qemu/qemu-print.h"



static void ufsblk_bus_class_init(ObjectClass *class, const void *data)
{
    qemu_printf("name ufsblk \n");
    qemu_printf(" enter in ufs_bus_class_init \n");

}

static uint64_t ufsblk_read(void *opaque, hwaddr offset,
                                unsigned size)
{
    printf("in ufsblk_read\n");
    return 0x78;
}

static void fsblk_write(void *opaque, hwaddr offset,
                             uint64_t value, unsigned size)
{
    printf("in fsblk_write\n");
    return;
}

static const MemoryRegionOps ufsblk_ops = {
    .read = ufsblk_read,
    .write = fsblk_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void ufsblk_init(Object *obj)
{
    SysBusDevice *dev = SYS_BUS_DEVICE(obj);
    struct ufsblk_hci *s = UFSBLK(dev);

    printf("enter in ufsblk_init\n");

    /* Connect the device to the sysbus */
    memory_region_init_io(&s->iomem, obj, &ufsblk_ops, s, TYPE_UFSBLK, 0x1000);
    sysbus_init_mmio(dev, &s->iomem);
    sysbus_init_irq(dev, &s->irq);
}

static const TypeInfo ufsblk_type_info = {
    .name = TYPE_UFSBLK,
    .parent = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(struct ufsblk_hci),
    .class_init = ufsblk_bus_class_init,
    .instance_init = ufsblk_init,
};

/* 注册设备到QEMU */
static void ufsblk_register_types(void)
{
    type_register_static(&ufsblk_type_info);
}
type_init(ufsblk_register_types);



