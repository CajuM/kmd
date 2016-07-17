project(kmd)
cmake_minimum_required(VERSION 3.2)

add_subdirectory(src)

add_custom_target(kmd_images ALL
	DEPENDS kmd.img
)

add_custom_command(
	OUTPUT kmd.img.in
        COMMAND qemu-img create -f raw kmd.img.in 5G
        COMMAND sudo modprobe nbd max_part=32
        COMMAND sudo umount mnt || true
        COMMAND sudo qemu-nbd -d /dev/nbd0
        COMMAND sudo qemu-nbd -f raw -c /dev/nbd0 kmd.img.in
        COMMAND sudo parted /dev/nbd0 mklabel msdos
        COMMAND sudo parted /dev/nbd0 mkpart primary ext4 1M 100%
        COMMAND sudo mkfs.ext4 /dev/nbd0p1
        COMMAND mkdir -p mnt
        COMMAND sudo mount /dev/nbd0p1 mnt
        COMMAND sudo mkdir -p mnt/boot/grub2
        COMMAND sudo grub2-install --target=i386-pc --no-floppy --modules='biosdisk part_msdos ext2 configfile normal multiboot' --boot-directory=mnt/boot /dev/nbd0
        COMMAND sudo ${CMAKE_SOURCE_DIR}/res/gengrub.sh ${CMAKE_SOURCE_DIR}/res/grub.cfg nbd0p1 >grub.cfg
        COMMAND sudo cp grub.cfg mnt/boot/grub2/
        COMMAND sudo umount mnt
        COMMAND rm -rf mnt
        COMMAND sudo sync
        COMMAND sudo qemu-nbd -d /dev/nbd0
)

add_custom_command(
	OUTPUT kmd-stripped
	COMMAND strip --strip-unneeded -o kmd-stripped src/kmd
	DEPENDS kmd
)

add_custom_command(
	OUTPUT kmd.img
	COMMAND cp kmd.img.in kmd.img
        COMMAND sudo modprobe nbd
        COMMAND sudo qemu-nbd -d /dev/nbd0
        COMMAND sudo qemu-nbd -f raw -c /dev/nbd0 kmd.img
        COMMAND sudo umount mnt || true
        COMMAND rm -rf mnt
	COMMAND mkdir mnt
        COMMAND sudo mount /dev/nbd0p1 mnt
        COMMAND sudo sh -c 'cat kmd-stripped | gzip > mnt/boot/kmd.gz'
        COMMAND sudo umount mnt
        COMMAND rm -rf mnt
        COMMAND sudo sync
        COMMAND sudo qemu-nbd -d /dev/nbd0
	DEPENDS kmd-stripped kmd.img.in
)
