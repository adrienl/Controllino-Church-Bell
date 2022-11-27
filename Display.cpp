#include "Display.hpp"

Display::Display(unsigned char lines, unsigned char rows) {
  _lines = lines;
  _rows = rows;
}

void Display::init(){
  _lcd.begin(_rows, _lines);
  _lcd.clear();
  _lcd.home();
}

void Display::printCharAt(unsigned char x, unsigned char y, unsigned char c) {
  char ch[2] = {c, 0};
  _lcd.setCursor(x, y);
  _lcd.print(ch);
}

void Display::printStringAt(unsigned char x, unsigned char y, char * str) {
  _lcd.setCursor(x, y);
  _lcd.print(str);
}
