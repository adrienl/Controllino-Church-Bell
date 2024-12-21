#pragma once

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

typedef enum eDisplayView{
  DV_HOME = 0,
  DV_MENULIST = 1,
  DV_MENUITEM
} tDisplayView;

class Display {
private:
    hd44780_I2Cexp _lcd;
    unsigned char _lines;
    unsigned char _rows;
    tDisplayView _view;
public:
    Display(unsigned char lines, unsigned char rows);
    void clearAt(unsigned char t, unsigned char y, unsigned char len);
    void clearAll();
    void printCharAt(unsigned char t, unsigned char y, char ch);
    void printStringAt(unsigned char x, unsigned char y, char * str);
    void setView(tDisplayView view);
    tDisplayView getView();
    void init();
    void toggleCursor(bool on);
    static Display build2X16() {
      return Display(2, 16);
    }
    void setCursor(uint8_t col, uint8_t row);
    void toggleBlink(bool on);
};
