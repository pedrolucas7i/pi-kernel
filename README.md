
# 📟 pi-kernel

Bare-metal Raspberry Pi kernel (no OS).

---

## ✨ Features

* UART serial output
* GPIO
* Framebuffer graphics
* Basic shapes + effects

---

## 📁 Structure

```
build/        → kernel.img
src/kernel/   → source code
Makefile
linker.ld
```

---

## ⚙️ Build and Test
```bash
make run
```

---

## 💾 Run

Copy to SD card boot partition:

```bash
cp build/kernel.img /boot/kernel.img
cp build/kernel.img /boot/kernel7.img
cp build/kernel.img /boot/kernel8.img
```

---

## 🔌 Example (LED)

```c
gpio_set_function(16, GPIO_OUTPUT);

while (1) {
    gpio_set(16);
    delay_ms(500);
    gpio_clear(16);
    delay_ms(500);
}
```

---

## 📺 Example (Graphics)

```c
framebuffer_init(800, 600, 32);
draw_wave_background(0);
```

---

## 🚧 Status

* UART ✔
* GPIO ✔
* Graphics ✔
* BOOT ✔