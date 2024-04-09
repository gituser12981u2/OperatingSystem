#include "lib/io.h"

inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}