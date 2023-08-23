build:
	mkdir -p bin
	gcc -m32 kernel/32/*.c -nostdlib -ffreestanding -mno-red-zone -fno-exceptions -nodefaultlibs -fno-builtin -fno-pic -O2 -T link32.ld -o paging32.img
	gcc -m64 kernel/64/*.c -nostdlib -ffreestanding -mno-red-zone -fno-exceptions -nodefaultlibs -fno-builtin -fno-pic -O2 -T link64.ld -o kernel.img
	nasm -f bin boot/boot32.s -o boot.img
	nasm -f bin boot/os_trampoline32.s -o boot-x86_64.img
	cat boot.img > bin/boot.img
	cat boot-x86_64.img >> bin/boot.img
	rm *.img
run:
	qemu-system-x86_64 -hda bin/boot.img -d int -D debug-log -no-reboot