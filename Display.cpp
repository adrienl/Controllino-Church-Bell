#include "Display.hpp"

Display::Display(unsigned char lines, unsigned char rows) {
  _lines = lines;
  _rows = rows;
  _view = DV_HOME;
}

void Display::init(){
  _lcd.begin(_rows, _lines);
  _lcd.clear();
  _lcd.home();
}

void Display::setView(tDisplayView view){
  _view = view;
}

tDisplayView Display::getView(){
  return _view;
}

void Display::clearAll(){
  for (unsigned char i = 0; i < _lines; i++){
    clearAt(0, i, _rows);
  }
}

void Display::clearAt(unsigned char x, unsigned char y, unsigned char len){
  char str[len + 1];
  _lcd.setCursor(x, y);
  int i = 0;
  while (i < len){
    str[i] = ' ';
    i++;
  }
  str[i] = 0;
  _lcd.print(str);
}

void Display::setCursor(uint8_t col, uint8_t row){
  _lcd.setCursor(col, row);
}

void Display::printCharAt(unsigned char x, unsigned char y, char c) {
  char ch[2] = {c, 0};
  _lcd.setCursor(x, y);
  _lcd.print(ch);
}

void Display::printStringAt(unsigned char x, unsigned char y, char * str) {
  _lcd.setCursor(x, y);
  _lcd.print(str);
}

void Display::toggleCursor(bool on){
  if (on){
    _lcd.cursor();
  }else{
    _lcd.noCursor();
  }
}

void Display::toggleBlink(bool on){
    if (on){
    _lcd.blink();
  }else{
    _lcd.noBlink();
  }
}