TOOLCHAIN = arm-none-eabi

CC = $(TOOLCHAIN)-gcc
OBJCOPY = $(TOOLCHAIN)-objcopy
GDB = $(TOOLCHAIN)-gdb

CPU = arm1176jzf-s

CFLAGS = \
-mcpu=$(CPU) \
-march=armv6zk \
-ffreestanding \
-fno-builtin \
-nostdlib \
-nostartfiles \
-Wall \
-Wextra \
-O2 \
-g

LDFLAGS = \
-T linker.ld

# Diretórios
SRC_DIR = src/kernel
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/objects

# Arquivos fonte (recursivo)
CSOURCES = $(shell find $(SRC_DIR) -name "*.c")
ASMSOURCES = $(shell find $(SRC_DIR) -name "*.S")

# Objetos
COBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CSOURCES))
ASMOBJECTS = $(patsubst $(SRC_DIR)/%.S,$(OBJ_DIR)/%.o,$(ASMSOURCES))

OBJECTS = $(COBJECTS) $(ASMOBJECTS)

# Binários finais
ELF = $(BUILD_DIR)/kernel.elf
IMG = $(BUILD_DIR)/kernel.img

all: build

build: $(IMG)

# Linkagem
$(ELF): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) \
	$(CFLAGS) \
	$(LDFLAGS) \
	-o $@ \
	$(OBJECTS) \
	-lgcc

# Geração da imagem
$(IMG): $(ELF)
	$(OBJCOPY) $< -O binary $@

# Compilação C (funciona para subpastas)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilação Assembly (funciona para subpastas)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Executar no QEMU
run: build
	qemu-system-arm \
	-M raspi1ap \
	-m 512 \
	-serial stdio \
	-kernel $(IMG)

# Debug
dbg: build
	$(GDB) $(ELF)

# Limpeza
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all build run clean dbg