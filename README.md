编译的命令：
cd qemu/
mkdir build
cd build

../configure --prefix=$PWD/ --target-list="arm-softmmu arm-linux-user" --enable-debug --enable-sdl --enable-kvm --enable-tools --disable-curl


启动并挂载ufs的命令：
ufs:
	./qemu-system-arm \
	-M vexpress-a9 \
	-m 512M \
	-smp 1 \
	-kernel /home/maohan/linux/qemu/kernel/buildout/arch/arm/boot/zImage \
	-dtb /home/maohan/linux/qemu/kernel/buildout/arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb \
	-append "root=/dev/mmcblk0 rw console=ttyAMA0" \
	-nographic \
	-sd /home/maohan/linux/qemu/qemu-boot/rootfs.ext3 \
	-fsdev local,security_model=passthrough,id=fsdev0,path=/home/maohan/linux/qemu/qemu-boot/share \
	-device virtio-9p-device,id=fs0,fsdev=fsdev0,mount_tag=hostshare \
	-drive if=none,id=ufs_disk,file=rootfs_ext4.img,format=raw \
	-device ufs-lu,drive=ufs_disk,bus=ufs-bus,lun=0 \


直接启动：
./qemu-system-arm -M vexpress-a9 -m 512M -monitor stdio