#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

namespace SoundManager{

    const std::string SHOOTING_AUDIO = "audio/Shoot.mp3";
    const std::string EXPLOSION_AUDIO = "audio/Explosion.mp3";
    const std::string DIE_AUDIO = "audio/Die.mp3";
    const std::string BOSS_SHOOTING_AUDIO = "audio/BossShoot.mp3";
    const std::string ITEM_PICKUP_AUDIO = "audio/ItemPickup.mp3";
    const std::string BUTTON_CLICK_AUDIO = "audio/ButtonClick.mp3";
    const std::string BACKGROUND_MUSIC = "audio/Background.mp3";
    const float AUDIO_VOLUME = 0.25f;

    void init();

    void playSoundtrack(std::string name);
    void playBackgroundMusic(std::string name);
    void stopBackgroundMusic();

    void setPlayingSoundtrack(bool isPlaying);
    void setPlayingBackgroundMusic(bool isPlaying);

    bool isPlayingSoundtrack();
    bool isPlayingBackgroundMusic();
    bool isPlayingBackgroundMusicCurrently();

}

#endif
