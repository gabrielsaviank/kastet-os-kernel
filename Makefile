BUILD = build

CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra -g -O0

LD = ld
LDFLAGS = -m elf_i386

all: $(BUILD)/os-image.bin

$(BUILD)/boot.bin: boot/boot.asm
	mkdir -p $(BUILD)
	nasm -f bin boot/boot.asm -o $(BUILD)/boot.bin

$(BUILD)/kernel.o: kernel/kernel_main.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c kernel/kernel_main.c -o $(BUILD)/kernel.o

$(BUILD)/console.o: kernel/helpers/console.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c kernel/helpers/console.c -o $(BUILD)/console.o

$(BUILD)/interrupts.o: kernel/interrupts.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c kernel/interrupts.c -o $(BUILD)/interrupts.o

$(BUILD)/isr_stubs.o: kernel/interrupts.asm
	mkdir -p $(BUILD)
	nasm -f elf32 kernel/interrupts.asm -o $(BUILD)/isr_stubs.o

$(BUILD)/kernel.bin: $(BUILD)/kernel.o $(BUILD)/console.o $(BUILD)/interrupts.o $(BUILD)/isr_stubs.o kernel/linker.ld
	$(LD) $(LDFLAGS) -T kernel/linker.ld $(BUILD)/kernel.o $(BUILD)/console.o $(BUILD)/interrupts.o $(BUILD)/isr_stubs.o -o $(BUILD)/kernel.elf
	objcopy -O binary $(BUILD)/kernel.elf $(BUILD)/kernel.bin

$(BUILD)/os-image.bin: $(BUILD)/boot.bin $(BUILD)/kernel.bin
	cat $(BUILD)/boot.bin $(BUILD)/kernel.bin > $(BUILD)/os-image.bin

run: all
	qemu-system-i386 -fda $(BUILD)/os-image.bin

run-debug: all
	qemu-system-i386 -fda $(BUILD)/os-image.bin -S -gdb tcp::1234

clean:
	rm -rf $(BUILD)