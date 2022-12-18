#pragma once

class TimeZone {
  private:
    char _title[16];
    char _regionalShortName[4];
    char _regionalShortDSTName[5];
    int _offset;
    int _dstOffset;
    bool _hasDSTOffset;
    bool _isDSTOffset;
  public:
    TimeZone(char * title, int offset);
    TimeZone(char * title, char * regionalShortName, int offset);
    TimeZone(char * title, char * regionalShortName, char * regionalShortSummerName, int offset, int summerOffset);
    int getOffsetInMinutes();
    int getDSTOffsetInMinutes();
    void getTitle(char * buff, unsigned char len);
    void getRegionalShortName(char * buff, unsigned char len);
    void getRegionalShortDSTName(char * buff, unsigned char len);
    bool hasDSTOffset();
    //Static
    
    static TimeZone buildEuropeParisTimezone();
    static TimeZone buildUTC0Timezone();
};
