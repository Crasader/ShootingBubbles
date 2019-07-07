#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

/**********************************************************************************
Bitmask rule
Set category bitmask for objects is power of 2
Set collision bitmask for objects is sum of category bitmasks that object collided

Character --> 1 --> Bubble, Box, Boss, Boss bullet, Windmill
Bubble --> 2 --> Character, Bullet, Windmill
Character bullet --> 4 --> Bubble, Boss, WindMill
Box --> 8 --> Character, Windmill
Boss --> 16 --> Character, Bullet
Boss bullet --> 32 --> Character
Windmill --> 64 --> Character, Bubble, Box, Character bullet
************************************************************************************/

/**********************************************************************************
How to use sqlite
- Download wxsqlite to like tutorial
- Copy all files to wxsqlite
- Add SET(CMAKE_CXX_FLAGS_DEBUG  "-DSQLITE_HAS_CODEC") to the end of CMakeList.txt
- Add SET(CMAKE_C_FLAGS_DEBUG  "-DSQLITE_HAS_CODEC")
- Add wxsqlite/sqlite3secure.c and wxsqlite/sqlite3.h to CMakelist.txt
- #include "sqlite3.h"
**********************************************************************************/

namespace Constants{

    const std::string GAME_VERSION = "1.1.1";
    const std::string GAME_AUTHOR = "Studio: lStudio";
    const std::string GAME_PRIVACY_POLICY_LINK = "https://ngthluu.github.io/eng/privacy/2019/03/19/shootingbubblesprivacypolicy.html";

    const int NUM_OF_TEXTURE_PACKER_FILES = 4;

    const float DISTANCE_BETWEEN_SLIDES = 1.05f;

    // Map
    const int NUM_OF_FREE_MAP = 1;
    const int SCORE_FOR_NEW_MAP = 150;
    const int NUM_OF_MAP = 4;
    const std::string MAP_PATH = "mapSlide";

    const int SETTING_BACKGROUND_LAYOUT_TAG = 1;

    const int MENU_MAP_ID = 0;
    const int NORMAL_MAP_ID = 1;
    const int SPEED_MAP_ID = 2;
    const int WINDMILL_MAP_ID = 3;
    const int BOSS_MAP_ID = 4;

    const int BOSS_MAP_INIT_NUM = 3;

    //Sprite
    const std::string COMPANY_LOGO = "luuStudio.png";
    const std::string OVERLAY_BACKGROUND = "overlay.png";
    const std::string HELP_BACKGROUND = "help.png";

    const std::string INFO_BUTTON_0 = "infoButton0.png";
    const std::string INFO_BUTTON_1 = "infoButton1.png";

    const std::string PRIVACY_POLICY_BUTTON_0 = "privacypolicyButton0.png";
    const std::string PRIVACY_POLICY_BUTTON_1 = "privacypolicyButton1.png";

    const std::string HELP_BUTTON_0 = "helpButton0.png";
    const std::string HELP_BUTTON_1 = "helpButton1.png";

    const std::string PLAY_BUTTON_0 = "playButton0.png";
    const std::string PLAY_BUTTON_1 = "playButton1.png";

    const std::string SETTING_BUTTON_0 = "settingButton0.png";
    const std::string SETTING_BUTTON_1 = "settingButton1.png";

    const std::string STATS_BUTTON_0 = "statsButton0.png";
    const std::string STATS_BUTTON_1 = "statsButton1.png";

    const std::string CLOSE_BUTTON_0 = "closeBtn0.png";
    const std::string CLOSE_BUTTON_1 = "closeBtn1.png";

    const std::string RESTART_BUTTON_0 = "restartButton0.png";
    const std::string RESTART_BUTTON_1 = "restartButton1.png";

    const std::string MUSIC_BUTTON_0 = "musicButton0.png";
    const std::string MUSIC_BUTTON_1 = "musicButton1.png";

    const std::string AUDIO_BUTTON_0 = "audioButton0.png";
    const std::string AUDIO_BUTTON_1 = "audioButton1.png";

    const std::string NEXT_BUTTON_0 = "nextButton0.png";
    const std::string NEXT_BUTTON_1 = "nextButton1.png";

    const std::string WHITE_CONTAINER_PATH = "infoBackground.png";

    const std::string BACKROUND_PATH = "background.png";

    const std::string ROUND_PANEL_PATH = "controllerPanel.png";
    const std::string ROUND_BUTTON_PATH = "controller_button.png";

    const std::string ARROW_LEFT = "arrowLeft.png";
    const std::string ARROW_RIGHT = "arrowRight.png";

    const std::string RANKING_SPRITE_PATH = "rank";

    const std::string SLIDE_PANEL_PATH = "slidePanel.png";
    const std::string SETTING_TAB_HEADER_PATH = "settingTab";
    const std::string SETTING_LAYOUT_PATH = "settingLayout.png";
    const std::string SETTING_BACKGROUND_PATH = "settingBackground.png";

    const std::string NEXT_SLIDE_BUTTON_PATH = "nextSlideButton.png";
    const std::string PREVIOUS_SLIDE_BUTTON_PATH = "previousSlideButton.png";

    //Fonts
    const std::string TITLE_FONT_PATH = "fonts/titleFont.ttf";
    const std::string DEFAULT_FONT_PATH = "fonts/defaultFont.ttf";
    const std::string DEFAULT_FONT_BOLD_PATH = "fonts/defaultFontBold.ttf";

    //Fonts size
    const float TITLE_FONT_SIZE = 100.0f;
    const float SCORE_FONT_SIZE = 100.0f;
    const float RECORD_FONT_SIZE = 45.0f;
    const float STATS_FONT_SIZE = 40.0f;
    const float NORMAL_FONT_SIZE = 45.0f;
    const float SLIDE_FONT_SIZE = 80.0f;

    //Label
    const std::string GAME_TITLE = "SHOOTING\n BUBBLES";
    const std::string SETTING_SPRITE_TITLE = "Settings";
    const std::string STATS_SPRITE_TITLE = "Records";
    const std::string INFO_SPRITE_TITLE = "About us";
    const std::string STATS_TABLE_HEADER_RANKING = "Ranking";
    const std::string STATS_TABLE_HEADER_SCORE = "Score";
    const std::string GAME_OVER_TITLE = "GAME OVER";
    const std::string SOUNDTRACKS = "Sounds";
    const std::string BACKGROUND_MUSICS = "Backgrounds";
    const std::string CHARACTER_COLOR_LABEL = "Colors";

    //Sounds:
    const std::string SHOOTING_AUDIO = "audio/Shoot.mp3";
    const std::string EXPLOSION_AUDIO = "audio/Explosion.mp3";
    const std::string DIE_AUDIO = "audio/Die.mp3";
    const std::string BOSS_SHOOTING_AUDIO = "audio/BossShoot.mp3";
    const std::string ITEM_PICKUP_AUDIO = "audio/ItemPickup.mp3";
    const std::string BUTTON_CLICK_AUDIO = "audio/ButtonClick.mp3";
    const std::string BACKGROUND_MUSIC = "audio/Background.mp3";
    const float AUDIO_VOLUME = 0.25f;

    //World

    const int OPACITY = 50;

    const int DIR_LEFT = 1;
    const int DIR_RIGHT = 2;
    const int DIR_UP = 3;
    const int DIR_DOWN = 4;

    const int OUT_SPACE = 20;
    const int BUBBLE_OUT_SPACE = 150;

    const float PI = 3.1415926f;

    const int NUM_OF_SETTING_TABS = 4;

    //Control layer
    const int CONTROLLER_PANEL_MARGIN = 40;
    const int CONTROLLER_ID = 10;
    const int CONTROLLER_PANEL_ID = 20;
    const int SHOOTING_PANEL_ID = 30;

    const int BULLET_BAR_CONTAINER_OPACITY = 100;
    const int BULLET_BAR_OPACITY = 150;

    const std::string SHOOTING_PANEL_PATH = "shootingPanel.png";
    const std::string BULLET_BAR_CONTAINER_PATH = "bulletBarContainer.png";
    const std::string BULLET_BAR_PATH = "bulletBar.png";

    //Windmill
    const std::string WINDMILL_PATH = "WindMill.png";
    const int WINDMILL_COLLISION_BITMASK = 15;
    const int WINDMILL_CATEGORY_BITMASK = 64;
    const int WINDMILL_ID = 30;

    //Bubble

    const int BUBBLE_BIG_TAG = 2;
    const int BUBBLE_MEDIUM_TAG = 1;
    const int BUBBLE_SMALL_TAG = 0;

    const int BUBBLE_COLLISION_BITMASK = 69;
    const int BUBBLE_CATEGORY_BITMASK = 2;

    const int BUBBLE_Z_ORDER = 1;

    const float BUBBLE_V = 90.0f;

    const int BUBBLE_NUM_COLOR = 25;
    const int BUBBLE_INIT_NUM = 20;
    const int BUBBLE_MAX_NUM = 200;

    const int BUBBLE_INCREASE_TIME = 7;

    const std::string BUBBLE_PATH = "bubble_";

    //Character's bullets
    const int CHARACTER_BULLET_Z_ORDER = 3;
    const int CHARACTER_BULLET_MAXIMUM = 5;
    const std::string CHARACTER_BULLET_PATH = "bullet.png";
    const int CHARACTER_BULLET_RESET_TIME = 2;

    const float CHARACTER_BULLET_V = 10.0f;

    const int CHARACTER_BULLET_COLLISION_BITMASK = 82;
    const int CHARACTER_BULLET_CATEGORY_BITMASK = 4;

    //Character

    const int SUM_SCORE_FOR_SKIN = 1500;
    const int NUM_OF_FREE_SKINS = 3;

    const int NUM_OF_CHARACTER_SKIN = 8;

    const float CHARACTER_V = 150.0f;

    const int CHARACTER_Z_ORDER = 3;
    const std::string CHARACTER_PATH = "charSkin";

    const float CHARACTER_ROTATE_SPEED = 2.5f;
    const int CHARACTER_COLLISION_BITMASK = 122;
    const int CHARACTER_CATEGORY_BITMASK = 1;

    //Packages:

    const int PACKAGE_COLLISION_BITMASK = 65;
    const int PACKAGE_CATEGORY_BITMASK = 8;
    const int PACKAGE_Z_ORDER = 2;

    const float PACKAGE_V = 150.0f;

    const int BULLET_PACKAGE_NUM = 3;
    const std::string BULLET_PACKAGE_PATH = "bulletPackage.png";
    const int BULLET_PACKAGE_ID = 1;

    const std::string INF_BULLET_PACKAGE_PATH = "infBulletPackage.png";
    const int INF_BULLET_PACKAGE_ID = 2;
    const int INF_BULLET_PACKAGE_OUT_SCREEN_TIME = 10;
    const int INF_BULLET_PACKAGE_ACTIVE_TIME = 5;

    //Boss
    const std::string BOSS_PATH = "Boss";

    const int BOSS_NUM_COLOR = 4;
    const int BOSS_NUM_OF_TYPES = 4;

    const int BOSS_Z_ORDER = 3;
    const float BOSS_ROTATE_SPEED = 2.25f;
    const float BOSS_V = 90.0f;

    const int BOSS_COLLISION_BITMASK = 5;
    const int BOSS_CATEGORY_BITMASK = 16;

    const int BOSS_SCORE = 10;

    const int BOSS_EXIST_BASE_SCORE = 50;

    const int BOSS_BULLET_COLLISION_BITMASK = 1;
    const int BOSS_BULLET_CATEGORY_BITMASK = 32;
    const int BOSS_BULLET_Z_ORDER = 3;
    const float BOSS_BULLET_V = 3.0f;
    const std::string BOSS_BULLET_PATH = "bossBullet.png";

    const std::string BOSS_HP_BAR = "hpBar.png";
    const std::string BOSS_HP_BAR_CONTAINER = "hpBarContainer.png";
    const int BOSS_HP_BAR_CONTAINER_OPACITY = 100;
    const int BOSS_HP_BAR_OPACITY = 150;
}


#endif
