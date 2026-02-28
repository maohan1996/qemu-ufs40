编译的命令：
cd qemu/
mkdir build ：这是创建编译产物的存放目录
cd build 

进到创建产物的build目录下再编译
第一步：
../configure --prefix=$PWD/ --target-list="arm-softmmu arm-linux-user" --enable-debug --enable-sdl --enable-kvm --enable-tools --disable-curl

第二步：
make -j16


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

解释：
ufs host已经被挂到了sysbus上，所以：
1、 -drive if=none,id=ufs_disk,file=rootfs_ext4.img,format=raw
	这里把 id 就是为 flash 镜像起了个名字，指定为 ufs_disk ，file 指定自己创建的镜像当作flash

2、 -device ufs-lu,drive=ufs_disk,bus=ufs-bus,lun=0
	这里创建一个 LU，LU 挂载的flash就是前面创建的镜像，名称指定上面镜像的id，bus 的名字就叫 ufs-bus；
	ufs-bus就是前面为lu注册的bus名称，代码如下：
	qbus_init(&u->bus, sizeof(UfsBus), TYPE_UFS_BUS, DEVICE(u), "ufs-bus");



直接启动(测试用)：
./qemu-system-arm -M vexpress-a9 -m 128M -monitor stdio