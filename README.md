编译的命令：
cd qemu/
mkdir build ：这是创建编译产物的存放目录
cd build 

进到创建产物的build目录下再编译
第一步：
arm32 ：
../configure --prefix=$PWD/ --target-list="arm-softmmu arm-linux-user" --enable-debug --enable-sdl --enable-kvm --enable-tools --disable-curl

arm64：
../configure --prefix=$PWD/ --target-list="aarch64-softmmu" --enable-debug --enable-sdl --enable-kvm --enable-tools --disable-curl


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

如果使用的是arm64的virt启动，需要用dts文件的话（因为默认编译是没有给出现成的dts）需要启动时候导出dtb，然后反编译出dts再编译成dtb：
1、导出dtb：
    ./qemu-system-aarch64 \
    -M virt,dumpdtb=virto.dtb \ ====>这一行的dumpdtb是导出dtb的命令
    -cpu cortex-a72 \
    -smp 4 \
    -m 512M \
    -kernel ../linux-6.7/arch/arm64/boot/Image.gz \
    -drive format=raw,file=rootfs_qemu.img \
    -nographic \
    -append "noinitrd root=/dev/vda rw console=ttyAMA0 init=/linuxrc ignore_loglevel" \

    执行之后会直接停止，然后目录中会出现virto.dtb，此时你需要反汇编：
        dtc -I dtb -O dts virto.dtb -o virto.dts
    如果想要在启动时候指定dtb，直接导出的dtb不能直接使用，你还需要把dts再编译成dtb：
        dtc -I dts -O dtb -o virto.dtb virto.dts

    若想支持ufs，需加入如下代码在dts(放到CPU后面就行)：
    ufs: ufs@d000000 {
		compatible = "ufs-qemu";
		reg = <0x00 0xd000000 0x00 0x2000>;
		interrupts = <0 186 1>;
	};


2、使用方式：
	./qemu-system-aarch64 \
	-M virt \
	-cpu cortex-a72 \
	-smp 1 \
	-m 512M \
	-kernel /home/maohan/linux/qemu/kernel/arm64_buildout/arch/arm64/boot/Image.gz \
	-dtb virto.dtb \
	-nographic \
	--append "noinitrd root=/dev/vda rw nokaslr console=ttyAMA0 loglevel=8" \
	-drive if=none,file=rootfs.ext3,id=hd0 \
	-device virtio-blk-device,drive=hd0 \
	-fsdev local,security_model=passthrough,readonly=off,id=fsdev0,path=/home/maohan/linux/qemu/qemu-boot/share \
	-device virtio-9p-device,id=fs0,fsdev=fsdev0,mount_tag=hostshare \
	-drive if=none,id=ufs_disk,file=rootfs_ext4.img,format=raw \
	-device ufs-lu,drive=ufs_disk,bus=ufs-bus,lun=0 \


直接启动(测试用)：arm32:
./qemu-system-arm -M vexpress-a9 -m 128M -monitor stdio
arm64:
./qemu-system-aarch64 -M virt -m 128M -monitor stdio

cp qemu-system-aarch64 ../../qemu/qemu-boot/

