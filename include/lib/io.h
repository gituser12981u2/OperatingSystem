#ifndef _IO_H
#define _IO_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned int size_t;

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);
uint16_t inw(uint16_t port);

#endif // _IO_H