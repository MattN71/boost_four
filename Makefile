program_name := boost_four
program_SRCS := $(wildcard src/*.cpp)
program_OBJS := $(wildcard build/*.o)

TOOLCHAIN := arm-none-eabi

CFLAGS := -mfloat-abi=soft -mcpu=cortex-m0 -mthumb -O3 -Wall -Wextra -std=c++17 --specs=nosys.specs -nostdlib -fno-unwind-tables -fno-exceptions -fno-non-call-exceptions -fno-rtti  -fno-use-cxa-atexit -ffreestanding -g3
LDFLAGS := -lgcc

.PHONY: all
all: compile
	$(TOOLCHAIN)-g++ $(CFLAGS) -o build/$(program_name).elf -T linker.ld build/*.o $(LDFLAGS)

.PHONY: compile
compile: clean
	$(TOOLCHAIN)-g++ $(CFLAGS) -c $(program_SRCS)
	@mv *.o build/

.PHONY: clean
clean:
	@rm -f $(program_OBJS)
	@rm -f $(program_name).elf

.PHONY: sections
sections: all
	$(TOOLCHAIN)-objdump -h build/$(program_name).elf

.PHONY: asm
asm: all
	$(TOOLCHAIN)-objdump -tD build/$(program_name).elf

.PHONY: flash
flash: all
	openocd -f cfg/jlink.cfg -f cfg/linux_stm32f0x.cfg -c "program build/$(program_name).elf verify reset exit"
#	openocd -f cfg/jlink.cfg -f cfg/stm32f0x.cfg -c "program build/$(program_name).elf reset exit"

.PHONY: debug
debug:
	openocd -f cfg/jlink.cfg -f cfg/linux_stm32f0x.cfg

.PHONY: print
print:
	$(info	srcs are $(program_SRCS))
	$(info	objs are $(program_OBJS))

.PHONY: size
size: all
	$(TOOLCHAIN)-size -A -t -d build/$(program_name).elf


# Serial port
# sudo picocom --baud 2000000 --flow n --parity n --databits 8 --stopbits 1 /dev/ttyUSB0
