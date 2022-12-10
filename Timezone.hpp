#pragma once

class TimeZone {
  private:
    char _title {};
    char _regionalShortName {};
    char _regionalShortSummerName {};
    int _offset;
    int _summerOffset;
    bool _hasSummerOffset;
  public:
    TimeZone(char title[16], int offset);
    TimeZone(char title[16], char regionalShortName[4], int offset);
    TimeZone(char title[16], char regionalShortName[4], char regionalShortSummerName[5], int offset, int summerOffset);
    int getUTCOffsetInMinutes();
    int getSummerUTCOffsetInMinutes();
    void getTitle(char * buff, unsigned char len);
    void getRegionalShortName(char * buff, unsigned char len);
    void getRegionalShortSummerName(char * buff, unsigned char len);
    bool changeOffsetInSummer();
    //Static
    
    static TimeZone buildEuropeParisTimezone();
    static TimeZone buildUTC0Timezone();
};
