编译的命令：
cd qemu/
mkdir build
cd build

../configure --prefix=$PWD/ --target-list="arm-softmmu arm-linux-user" --enable-debug --enable-sdl --enable-kvm --enable-tools --disable-curl


启动并挂载ufs的命令：
ufs:
	./qemu-system-arm \
	-M vexpress-a9 \
	-m 128M \
	-smp 1 \
	-kernel /home/maohan/linux/uboot/uboot-vepress/u-boot \
	-append "root=/dev/mmcblk0 rw console=ttyAMA0"  \
	-nographic \
	-sd /home/maohan/linux/kernel/qemu-learn/rootfs.ext2 \
	-drive if=none,id=ufs_disk,file=rootfs_ext4.img,format=raw \
	-device ufs-lu,drive=ufs_disk,bus=ufs-bus,lun=0 \


直接启动：
./qemu-system-arm -M vexpress-a9 -m 128M -monitor stdio