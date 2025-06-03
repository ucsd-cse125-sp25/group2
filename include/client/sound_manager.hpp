#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <string>
#include <unordered_map>

class SoundManager {
public:
  SoundManager();
  ~SoundManager();

  static bool init();
  static void update(); // Call this every frame
  static void release();

    static bool loadSound(const std::string& name, const std::string& filepath, bool loop = false);
    static void playSound(const std::string& name);
    static void stopSound(const std::string& name);
    static void setVolume(const std::string& name, float volume);
    static void setMasterVolume(float volume);

private:
    static void preLoadSounds();
    static FMOD::System* system;
    static FMOD::ChannelGroup* masterGroup;
    static std::unordered_map<std::string, FMOD::Sound*> soundMap;
    static std::unordered_map<std::string, FMOD::Channel*> channelMap;
};