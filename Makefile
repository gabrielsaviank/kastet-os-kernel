BUILD = build

all: $(BUILD)/boot.bin

$(BUILD)/boot.bin: boot/boot.asm
	mkdir -p $(BUILD)
	nasm -f bin boot/boot.asm -o $(BUILD)/boot.bin

run: all
	qemu-system-x86_64 -drive format=raw,file=$(BUILD)/boot.bin

clean:
	rm -rf $(BUILD)

clion_dummy: kernel/kernel.c
    @echo "CLion: kernel.c is part of the project."
