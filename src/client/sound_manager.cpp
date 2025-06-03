#include "sound_manager.hpp"


// Static variable definitions
FMOD::System* SoundManager::system = nullptr;
std::unordered_map<std::string, FMOD::Sound*> SoundManager::soundMap;
std::unordered_map<std::string, FMOD::Channel*> SoundManager::channelMap;
FMOD::ChannelGroup* SoundManager::masterGroup = nullptr;

bool SoundManager::init() {
    FMOD_RESULT result = FMOD::System_Create(&system);
    if (result != FMOD_OK) return false;

    result = system->init(512, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) return false;

    result = system->getMasterChannelGroup(&masterGroup);
    if (result != FMOD_OK) return false;

    SoundManager::setMasterVolume(1.0f); 
    preLoadSounds();

    SoundManager::setVolume("BounceSound", 4.0f);
    SoundManager::playSound("TitleBGM");
    return true;
}

void SoundManager::preLoadSounds() {
    //bgm
    loadSound("CharacterSelectBGM", "../resources/audio/Character_Selection_Screen.mp3", true);
    loadSound("TitleBGM", "../resources/audio/BarnyardBreakout.ogg", true);
    loadSound("GameBGM", "../resources/audio/Focus_Game_Puzzle_Music.ogg", true);

    // Interaction sounds
    loadSound("BounceSound", "../resources/audio/BounceSound.ogg", false);
    loadSound("BoxDraggingSound", "../resources/audio/BoxDraggingSound.ogg", false);
    loadSound("Barnyard_Secret_Sound", "../resources/audio/Barnyard_Secret_Sound.ogg", false);
    loadSound("JumpLandingSound", "../resources/audio/JumpLandingSound.ogg", false);
    loadSound("WalkingSound", "../resources/audio/WalkingSound.ogg", false);


    //animal sounds
    loadSound("ChickenSound", "../resources/audio/ChickenSound.ogg", false);
    loadSound("CowSound", "../resources/audio/CowSound.ogg", false);
    loadSound("HorseSound", "../resources/audio/HorseSound.ogg", false);
    loadSound("PigSound", "../resources/audio/PigSound.ogg", false);
    loadSound("SheepSound", "../resources/audio/SheepSound.ogg", false);

}

void SoundManager::update() {
    if (system) {
        system->update();
    }
}

void SoundManager::release() {
    for (auto& [name, sound] : soundMap) {
        if (sound) sound->release();
    }
    soundMap.clear();
    channelMap.clear();

    if (system) {
        system->close();
        system->release();
        system = nullptr;
    }
}

bool SoundManager::loadSound(const std::string& name, const std::string& filepath, bool loop) {
    if (soundMap.find(name) != soundMap.end()) return true;

    FMOD_MODE mode = FMOD_DEFAULT;
    if (loop) mode |= FMOD_LOOP_NORMAL;
    else mode |= FMOD_LOOP_OFF;

    FMOD::Sound* sound = nullptr;
    FMOD_RESULT result = system->createSound(filepath.c_str(), mode, nullptr, &sound);
    if (result != FMOD_OK) {
        std::cerr << "Failed to load sound: " << filepath << std::endl;
        return false;
    }

    soundMap[name] = sound;
    return true;
}

void SoundManager::playSound(const std::string& name) {
    auto it = soundMap.find(name);
    if (it == soundMap.end()) return;

    auto channelIt = channelMap.find(name);
    if (channelIt != channelMap.end() && channelIt->second) {
        bool isPlaying = false;
        channelIt->second->isPlaying(&isPlaying);
        if (isPlaying) {
            return; // Already playing; skip
        }
    }
    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = system->playSound(it->second, nullptr, false, &channel);
    if (result == FMOD_OK) {
        channelMap[name] = channel;
    }
}

void SoundManager::stopSound(const std::string& name) {
    auto it = channelMap.find(name);
    if (it != channelMap.end() && it->second) {
        bool isPlaying = false;
        it->second->isPlaying(&isPlaying);
        if (isPlaying) {
            it->second->stop();
        }
        channelMap.erase(it); 
    }
}

void SoundManager::setVolume(const std::string& name, float volume) {
    auto it = channelMap.find(name);
    if (it != channelMap.end() && it->second) {
        it->second->setVolume(volume); // Volume range: 0.0f (silent) to 1.0f (full)
    }
}

void SoundManager::setMasterVolume(float volume) {
    if (masterGroup) {
        masterGroup->setVolume(volume); // Controls all currently playing sounds
    }
}