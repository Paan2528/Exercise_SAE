//
//  main.cpp
//  Hello World auf LCD Screen
//
//  Created by Salih Kayan on 01.07.26.
//

#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <string>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>

const char* I2C_DEVICE = "/dev/i2c-1";
const int LCD_ADDRESS = 0x27; // oft 0x27 oder 0x3F

const unsigned char LCD_BACKLIGHT = 0x08;
const unsigned char ENABLE = 0x04;
const unsigned char REGISTER_SELECT = 0x01;

void sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void write_byte(int fd, unsigned char value) {
    write(fd, &value, 1);
}

void lcd_strobe(int fd, unsigned char data) {
    write_byte(fd, data | ENABLE | LCD_BACKLIGHT);
    sleep_ms(1);
    write_byte(fd, (data & ~ENABLE) | LCD_BACKLIGHT);
    sleep_ms(1);
}

void lcd_write_4_bits(int fd, unsigned char data) {
    write_byte(fd, data | LCD_BACKLIGHT);
    lcd_strobe(fd, data);
}

void lcd_send(int fd, unsigned char value, bool is_character) {
    unsigned char mode = is_character ? REGISTER_SELECT : 0x00;

    unsigned char high = value & 0xF0;
    unsigned char low = (value << 4) & 0xF0;

    lcd_write_4_bits(fd, high | mode);
    lcd_write_4_bits(fd, low | mode);
}

void lcd_command(int fd, unsigned char command) {
    lcd_send(fd, command, false);
}

void lcd_write_char(int fd, char character) {
    lcd_send(fd, character, true);
}

void lcd_init(int fd) {
    sleep_ms(50);

    lcd_write_4_bits(fd, 0x30);
    sleep_ms(5);
    lcd_write_4_bits(fd, 0x30);
    sleep_ms(5);
    lcd_write_4_bits(fd, 0x30);
    sleep_ms(5);
    lcd_write_4_bits(fd, 0x20);

    lcd_command(fd, 0x28); // 4-bit, 2 Zeilen
    lcd_command(fd, 0x0C); // Display an, Cursor aus
    lcd_command(fd, 0x06); // Cursor nach rechts bewegen
    lcd_command(fd, 0x01); // Display leeren
    sleep_ms(2);
}

void lcd_set_cursor(int fd, int row, int col) {
    int row_offsets[] = {0x00, 0x40};
    lcd_command(fd, 0x80 | (row_offsets[row] + col));
}

void lcd_print(int fd, const std::string& text) {
    for (char c : text) {
        lcd_write_char(fd, c);
    }
}

int main() {
    int fd = open(I2C_DEVICE, O_RDWR);

    if (fd < 0) {
        std::cerr << "Fehler: I2C-Bus konnte nicht geoeffnet werden.\n";
        return 1;
    }

    if (ioctl(fd, I2C_SLAVE, LCD_ADDRESS) < 0) {
        std::cerr << "Fehler: LCD-Adresse konnte nicht ausgewaehlt werden.\n";
        close(fd);
        return 1;
    }

    lcd_init(fd);
    lcd_set_cursor(fd, 0, 0);
    lcd_print(fd, "Hello World!");

    close(fd);
    return 0;
}
