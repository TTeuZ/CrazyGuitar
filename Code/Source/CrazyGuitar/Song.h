#pragma once

// Standard includes
#include <array>
#include <cstdint>
#include <list>
#include <string>

// Unreal includes
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Notes.h"
#include "Sound/SoundWave.h"

class CRAZYGUITAR_API Song {
   public:
    Song();
    Song(const FString& dirPath);
    virtual ~Song() = default;

    void playSong();

    uint16_t getBPM() const;
    const std::list<std::array<uint16_t, 3>>& getRawNotes() const;

    /*
     *   Unreal Engine enforce us to use the name newChord instead of chord, otherwise
     *   we received a compilation error
     */
    void setDirPath(const FString& newDirPath);
    void setLength(const std::string& newLength);
    void setWorld(UWorld* const newWorld);

   private:
    const static FString BASE_DIR_PATH;

    void readDirPath();
    bool dirExists(const FString& path) const;
    void readInfo();
    void readNotes();

    FString dirPath;
    std::string name;
    std::string artist;
    std::string genre;
    uint32_t length;
    uint16_t bpm;
    std::list<std::array<uint16_t, 3>> rawNotes;
    UWorld* world;
    UAudioComponent* audioComponent;
    USoundBase* soundBase;
};
