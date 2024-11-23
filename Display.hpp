#pragma once

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

class Display {

private:
    hd44780_I2Cexp _lcd;
    unsigned char _lines;
    unsigned char _rows;
public:
    int x;
    Display(unsigned char lines, unsigned char rows);
    void clearAt(unsigned char t, unsigned char y, unsigned char len);
    void clearAll();
    void printCharAt(unsigned char t, unsigned char y, char ch);
    void printStringAt(unsigned char x, unsigned char y, char * str);
    void init();
    static Display build2X16() {
      return Display(2, 16);
    }
};
