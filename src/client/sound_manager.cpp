#include "sound_manager.hpp"


// Static variable definitions
FMOD::System* SoundManager::system = nullptr;
std::unordered_map<std::string, FMOD::Sound*> SoundManager::soundMap;
std::unordered_map<std::string, FMOD::Channel*> SoundManager::channelMap;

bool SoundManager::init() {
    FMOD_RESULT result = FMOD::System_Create(&system);
    if (result != FMOD_OK) return false;

    result = system->init(512, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) return false;
    preLoadSounds();
    return true;
}

void SoundManager::preLoadSounds() {
    loadSound("CharacterSelectBGM", "../resources/audio/Character_Selection_Screen.mp3", true);
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

    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = system->playSound(it->second, nullptr, false, &channel);
    if (result == FMOD_OK) {
        channelMap[name] = channel;
    }
}