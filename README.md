# SvetOS Kernel
Microkernel-based operating system for embedded devices

## Features
- Modular architecture
- GRUB multiboot2 support
- IPC-based communication
- Sandboxed components

## Build Instructions
```bash
sudo apt install build-essential nasm grub-pc-bin xorriso qemu-system-x86
make all
make run

curl -o LICENSE https://opensource.org/licenses/MIT

git add LICENSE
git commit -m "Add MIT license"
git push origin main
