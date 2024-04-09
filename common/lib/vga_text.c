#include "lib/vga_text.h"

char* const VGA_TEXT_MODE_BUFFER = (char*)0xB8000;

// Function print a string to the screen in VGA text mode
void print_to_screen(const char* str) {
    static uint16_t pos = 0;
    while (*str)  {
        if (*str == '\n') {
            pos += SCREEN_WIDTH - (pos % SCREEN_WIDTH);
        } else {
            VGA_TEXT_MODE_BUFFER[pos * 2] = *str;
            VGA_TEXT_MODE_BUFFER[pos * 2 + 1] = 0x07; // Light grey on black
        }
        ++pos;
        ++str;
        //Scroll the screen if necessary
        if (pos >= SCREEN_WIDTH * SCREEN_HEIGHT) {
            for (int i = 0; i < (SCREEN_HEIGHT - 1) * SCREEN_WIDTH * 2; i++) {
                VGA_TEXT_MODE_BUFFER[i] = VGA_TEXT_MODE_BUFFER[i + SCREEN_WIDTH * 2];
            }
            for (int i = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH * 2; i < SCREEN_HEIGHT * SCREEN_WIDTH * 2; i++) {
                VGA_TEXT_MODE_BUFFER[i] = 0;
            }
            pos -= SCREEN_WIDTH;
        }
        //++str;
    }
}

void print_hex(uint32_t value) {
    char hex_string[9]; // 8 characters + null terminator
    char* hex_chars = "0123456789ABCDEF";

    for (int i = 7; i >= 0; i--) {
        hex_string[i] = hex_chars[value & 0xF]; // Get the last hex digit
        value >>= 4; // Shift right by 4 bits to get the next digit
    }
    hex_string[8] = '\0';

    print_to_screen(hex_string);
}

