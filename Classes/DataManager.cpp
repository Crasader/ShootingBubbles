#include "DataManager.h"

DataManager::DataManager(){

}

DataManager::~DataManager(){

}

bool DataManager::init(){
    std::string path = FileUtils::getInstance()->getWritablePath() + DATABASE_FILE_NAME;

    if ((sqlite3_open(path.c_str(), &database) != SQLITE_OK)) {
        sqlite3_close(database);
        return 0;
    }

    sqlite3_key(database, passEncrypt, (int)strlen(passEncrypt));

    //Create score table
    char create_score_sql[] = "CREATE TABLE score ( "
        "id INTEGER PRIMARY KEY, "
        "score INTEGER NOT NULL"
    ");";
    sqlite3_exec(database, create_score_sql, 0, 0, NULL);

    //Create setting table
    char create_setting_sql[] = "CREATE TABLE setting ( "
        "id INTEGER PRIMARY KEY, "
        "bg_music INTEGER NOT NULL, "
        "eff_music INTEGER NOT NULL, "
        "char_skin INTEGER NOT NULL, "
        "map INTEGER NOT NULL, "
        "time_playing INTEGER NOT NULL, "
        "tut_show INTERGER NOT NULL"
    ");";

    sqlite3_exec(database, create_setting_sql, 0, 0, NULL);

    //Add default setting
    char add_basic_setting_sql[] = "INSERT INTO setting (id, bg_music, eff_music, char_skin, map, time_playing, tut_show) VALUES (1, 1, 1, 0, 0, 0, 1);";
    sqlite3_exec(database, add_basic_setting_sql, 0, 0, NULL);
    return 1;
}

void DataManager::appendScore(int score){
    //Get number of row, to set the id
    int numOfRow = 0;
    char sqlGetNumOfRow[] = "SELECT count(*) FROM score;";
    if (sqlite3_prepare_v2(database, sqlGetNumOfRow, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            numOfRow = sqlite3_column_int(stmt, 0);
            sqlite3_reset(stmt);
        }
    }
    std::string sqlAppendScore = "INSERT INTO score (id, score) VALUES ("
                            + std::to_string(numOfRow + 1) + " ,"
                            + std::to_string(score) + ");";
    sqlite3_exec(database, sqlAppendScore.c_str(), 0, 0, NULL);
}

void DataManager::turnOnBGMusic(bool value){
    std::string valueStr = (value) ? "1" : "0";
    std::string sql = "UPDATE setting SET bg_music = " + valueStr + " WHERE id = 1;";
    sqlite3_exec(database, sql.c_str(), 0, 0, NULL);
}

void DataManager::turnOnAudio(bool value){
    std::string valueStr = (value) ? "1" : "0";
    std::string sql = "UPDATE setting SET eff_music = " + valueStr + " WHERE id = 1;";
    sqlite3_exec(database, sql.c_str(), 0, 0, NULL);
}

void DataManager::changeCharacterColor(int color){
    std::string valueStr = std::to_string(color);
    std::string sql = "UPDATE setting SET char_skin = " + valueStr + " WHERE id = 1;";
    sqlite3_exec(database, sql.c_str(), 0, 0, NULL);
}

bool DataManager::isBGMusicIsTurningOn(){
    int isTurningOn = 0;
    char sql[] = "SELECT bg_music FROM setting WHERE id = 1;";
    if (sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            isTurningOn = sqlite3_column_int(stmt, 0);
            sqlite3_reset(stmt);
        }
    }
    return isTurningOn;
}

bool DataManager::isAudioIsTurningOn(){
    int isTurningOn = 0;
    char sql[] = "SELECT eff_music FROM setting WHERE id = 1;";
    if (sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            isTurningOn = sqlite3_column_int(stmt, 0);
            sqlite3_reset(stmt);
        }
    }
    return isTurningOn;
}

int DataManager::getCharacterColor(){
    int color = 0;
    char sql[] = "SELECT char_skin FROM setting WHERE id = 1;";
    if (sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            color = sqlite3_column_int(stmt, 0);
            sqlite3_reset(stmt);
        }
    }
    return color;
}

int DataManager::getHighestRecord(){
    int score = 0;
    char sql[] = "SELECT * FROM score ORDER BY score DESC;";
    if (sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            score = sqlite3_column_int(stmt, 1);
        }
        sqlite3_reset(stmt);
    }
    return score;
}

std::vector <std::string> DataManager::getTop5Record(){
    std::vector <std::string> topRecord;
    std::string score = "", column = "";
    int count = 1;
    char sql[] = "SELECT * FROM score ORDER BY score DESC;";
    if (sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW && count <= 5) {
            score = std::to_string(sqlite3_column_int(stmt, 1)); // score
            column = "         " + score;
            topRecord.push_back(column);
            ++count;
        }
        sqlite3_reset(stmt);
    }
    return topRecord;
}

int DataManager::countPlayingTimes() {
    int count = 0;
    char sql[] = "SELECT time_playing FROM setting WHERE id = 1;";
    if (sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
            sqlite3_reset(stmt);
        }
    }
    return count;
}

void DataManager::setCountPlayingTimes(int count){
    std::string valueStr = std::to_string(count);
    std::string sql = "UPDATE setting SET time_playing = " + valueStr + " WHERE id = 1;";
    sqlite3_exec(database, sql.c_str(), 0, 0, NULL);
}

bool DataManager::getTutorialShowPlaying(){
    int isTutoralPlayed = 0;
    char sql[] = "SELECT tut_show FROM setting WHERE id = 1;";
    if (sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            isTutoralPlayed = sqlite3_column_int(stmt, 0);
            sqlite3_reset(stmt);
        }
    }
    return isTutoralPlayed;
}

void DataManager::setTutorialShowPlaying(bool value){
    std::string valueStr = (value) ? "1" : "0";
    std::string sql = "UPDATE setting SET tut_show = " + valueStr + " WHERE id = 1;";
    sqlite3_exec(database, sql.c_str(), 0, 0, NULL);
}

int DataManager::getSumOfRecords(){
    int result = 0;
    char sql[] = "SELECT SUM(score) FROM score;";
    if (sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            result = sqlite3_column_int(stmt, 0);
            sqlite3_reset(stmt);
        }
    }
    return result;
}

void DataManager::changeMap(int map){

    //Add map if still not exists
    std::string addMapSQL = "ALTER TABLE setting ADD map INTEGER;";
    sqlite3_exec(database, addMapSQL.c_str(), 0, 0, NULL);

    std::string valueStr = std::to_string(map);
    std::string sql = "UPDATE setting SET map = " + valueStr + " WHERE id = 1;";
    sqlite3_exec(database, sql.c_str(), 0, 0, NULL);
}

int DataManager::getMap(){
    int map = 0;
    char sql[] = "SELECT map FROM setting WHERE id = 1;";
    if (sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            map = sqlite3_column_int(stmt, 0);
            sqlite3_reset(stmt);
        }
    }
    return map;
}

void DataManager::close(){
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(database);
}
