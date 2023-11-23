#pragma once

// Standard includes
#include <array>
#include <cstdint>
#include <list>
#include <string>

// Unreal includes
#include "Containers/UnrealString.h"
#include "Notes.h"

class CRAZYGUITAR_API Song {
   public:
    Song();
    Song(const FString& dirPath);

    virtual ~Song() = default;

    uint16_t getBPM() const;
    const std::list<std::array<uint16_t, 3>>& getRawNotes() const;

    void setDirPath(const FString& newDirPath);
    void setLength(std::string& newLength);

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
    int32_t length;
    uint16_t bpm;
    std::list<std::array<uint16_t, 3>> rawNotes;
};
