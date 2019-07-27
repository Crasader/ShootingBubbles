#include "cocos2d.h"
#include "SoundManager.h"
#include "SimpleAudioEngine.h"
#include "DataManager.h"

USING_NS_CC;

void SoundManager::init(){
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();

    audio->setEffectsVolume(SoundManager::AUDIO_VOLUME);
    audio->preloadEffect(SoundManager::SHOOTING_AUDIO.c_str());
    audio->preloadEffect(SoundManager::EXPLOSION_AUDIO.c_str());
    audio->preloadEffect(SoundManager::DIE_AUDIO.c_str());
    audio->preloadEffect(SoundManager::BOSS_SHOOTING_AUDIO.c_str());
    audio->preloadEffect(SoundManager::ITEM_PICKUP_AUDIO.c_str());
    audio->preloadEffect(SoundManager::BUTTON_CLICK_AUDIO.c_str());

    audio->preloadBackgroundMusic(SoundManager::BACKGROUND_MUSIC.c_str());

    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        isPlaySoundtrack = dataManager->isAudioIsTurningOn();
        isPlayBG = dataManager->isBGMusicIsTurningOn();
        dataManager->close();
    }
    delete dataManager;
}

void SoundManager::playSoundtrack(std::string name){

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(name.c_str());

}

void SoundManager::playBackgroundMusic(std::string name){

    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(name.c_str(), true);

}

void SoundManager::stopBackgroundMusic(){
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void SoundManager::setPlayingSoundtrack(bool isPlaying){
    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        dataManager->turnOnAudio(isPlaying);
        dataManager->close();
    }
    delete dataManager;
    isPlaySoundtrack = isPlaying;
}

void SoundManager::setPlayingBackgroundMusic(bool isPlaying){
    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        dataManager->turnOnBGMusic(isPlaying);
        dataManager->close();
    }
    delete dataManager;
    isPlayBG = isPlaying;
}

bool SoundManager::isPlayingSoundtrack(){
    return isPlaySoundtrack;
}

bool SoundManager::isPlayingBackgroundMusic(){
    return isPlayBG;
}

bool SoundManager::isPlayingBackgroundMusicCurrently(){
    return CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}
