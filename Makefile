BUILD = build

CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra
LD = ld
LDFLAGS = -m elf_i386

all: $(BUILD)/os-image.bin

$(BUILD)/boot.bin: boot/boot.asm
	mkdir -p $(BUILD)
	nasm -f bin boot/boot.asm -o $(BUILD)/boot.bin

$(BUILD)/kernel.o: kernel/kernel.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c kernel/kernel.c -o $(BUILD)/kernel.o

$(BUILD)/kernel.bin: $(BUILD)/kernel.o kernel/linker.ld
	$(LD) $(LDFLAGS) -T kernel/linker.ld $(BUILD)/kernel.o -o $(BUILD)/kernel.elf
	objcopy -O binary $(BUILD)/kernel.elf $(BUILD)/kernel.bin

$(BUILD)/os-image.bin: $(BUILD)/boot.bin $(BUILD)/kernel.bin
	cat $(BUILD)/boot.bin $(BUILD)/kernel.bin > $(BUILD)/os-image.bin

run: all
	qemu-system-i386 -fda $(BUILD)/os-image.bin

clean:
	rm -rf $(BUILD)
