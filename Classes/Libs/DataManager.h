/******************************************************************
Design of database:
- Score table: top 5 record played
+ ID
+ Score
- Setting table:
+ ID
+ Background music: true/false.
+ Effect music: true/false.
+ Skin of character: blue, green, violet.
+ Time playing
*******************************************************************/

#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "cocos2d.h"
#include "sqlite3.h"
#include <vector>

USING_NS_CC;

class DataManager{

public:
    DataManager();
    ~DataManager();

    bool init();
    void appendScore(int score);
    void turnOnBGMusic(bool value);
    void turnOnAudio(bool value);
    void changeCharacterColor(int color);

    bool isBGMusicIsTurningOn();
    bool isAudioIsTurningOn();
    int getCharacterColor();

    int countPlayingTimes();
    void setCountPlayingTimes(int count);

    bool getTutorialShowPlaying();
    void setTutorialShowPlaying(bool);

    int getHighestRecord();
    std::vector <std::string> getTop5Record();

    int getSumOfRecords();

    void changeMap(int map);
    int getMap();

    void close();

private:
    const std::string DATABASE_FILE_NAME = "data.db";
    const int NUM_SCORE_FEATURE = 2;

    sqlite3* database;
    sqlite3_stmt *stmt = nullptr;

    const char* passEncrypt = "luustudioJKD";
};

#endif
