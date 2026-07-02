//
//  main.cpp
//
//
//  Created by Salih Kayan on 30.06.26.
//

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

using namespace std;

#define I2C_BUS "/dev/i2c-1"

#define LCD_BACKLIGHT 0x08
#define ENABLE 0x04
#define LCD_CMD 0
#define LCD_CHR 1

int i2cFile;
int lcdAddress;

void i2cWriteByte(int data) {
    write(i2cFile, &data, 1);
}

void lcdToggleEnable(int data) {
    usleep(500);
    i2cWriteByte(data | ENABLE);
    usleep(500);
    i2cWriteByte(data & ~ENABLE);
    usleep(500);
}

void lcdSendNibble(int data) {
    i2cWriteByte(data | LCD_BACKLIGHT);
    lcdToggleEnable(data | LCD_BACKLIGHT);
}

void lcdSendByte(int value, int mode) {
    int high = mode | (value & 0xF0);
    int low = mode | ((value << 4) & 0xF0);

    lcdSendNibble(high);
    lcdSendNibble(low);
}

void lcdCommand(int cmd) {
    lcdSendByte(cmd, LCD_CMD);
}

void lcdChar(char c) {
    lcdSendByte(c, LCD_CHR);
}

void lcdInit() {
    usleep(50000);

    lcdSendNibble(0x30);
    usleep(5000);

    lcdSendNibble(0x30);
    usleep(5000);

    lcdSendNibble(0x30);
    usleep(5000);

    lcdSendNibble(0x20);

    lcdCommand(0x28); // 4-bit mode, 2 lines
    lcdCommand(0x0C); // display on, cursor off
    lcdCommand(0x06); // cursor moves right
    lcdCommand(0x01); // clear display

    usleep(2000);
}

void lcdClear() {
    lcdCommand(0x01);
    usleep(2000);
}

void lcdSetCursor(int line, int column) {
    int lineAddress[] = {0x80, 0xC0};
    lcdCommand(lineAddress[line] + column);
}

void lcdPrint(string text) {
    for (char c : text) {
        lcdChar(c);
    }
}

vector<int> scanI2CDevices() {
    vector<int> foundDevices;

    for (int address = 0x03; address <= 0x77; address++) {
        if (ioctl(i2cFile, I2C_SLAVE, address) < 0) {
            continue;
        }

        char buffer = 0;
        if (write(i2cFile, &buffer, 1) >= 0) {
            foundDevices.push_back(address);
        }
    }

    return foundDevices;
}

bool isTypicalLCDAddress(int address) {
    int lcdAddresses[] = {0x27, 0x3F};

    for (int possibleAddress : lcdAddresses) {
        if (address == possibleAddress) {
            return true;
        }
    }

    return false;
}

int main() {
    i2cFile = open(I2C_BUS, O_RDWR);

    if (i2cFile < 0) {
        cout << "Fehler: I2C-Bus konnte nicht geöffnet werden." << endl;
        return 1;
    }

    vector<int> devices = scanI2CDevices();

    cout << "Gefundene I2C-Geräte:" << endl;

    for (int address : devices) {
        cout << "Adresse: 0x" << hex << address << endl;
    }

    bool lcdFound = false;

    for (int address : devices) {
        if (isTypicalLCDAddress(address)) {
            lcdAddress = address;
            lcdFound = true;
            break;
        }
    }

    if (!lcdFound) {
        cout << "Kein typisches LCD-Display gefunden." << endl;
        close(i2cFile);
        return 1;
    }

    cout << "LCD gefunden bei Adresse: 0x" << hex << lcdAddress << endl;

    if (ioctl(i2cFile, I2C_SLAVE, lcdAddress) < 0) {
        cout << "Fehler: LCD-Adresse konnte nicht gesetzt werden." << endl;
        close(i2cFile);
        return 1;
    }

    lcdInit();
    lcdClear();

    lcdSetCursor(0, 0);
    lcdPrint("Hello SAE!");

    close(i2cFile);

    return 0;
}
