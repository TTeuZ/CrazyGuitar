#pragma once

#include <array>
#include <cstdint>
#include <list>
#include <string>

#include "Containers/UnrealString.h"
#include "Notes.h"

class Song {
public:
    Song();
    Song(const FString dirPath);
    Song(const std::string name, const std::string artist, const std::string genre, const uint16_t length,
         const uint16_t bpm);
    virtual ~Song();

    std::string getLength() const;
    int getBPM() const;
    std::list<std::array<uint16_t, 3>> const &getNotes() const;

private:
    const static FString BASE_DIR_PATH;

    void setDirPath(const FString newDirPath);
    void setLength(std::string newLength);
    void setLength(int32_t newLength);

    bool dirExists(const FString path) const;

    void readInfo();
    void readNotes();

    FString dirPath;
    std::string name;
    std::string artist;
    std::string genre;
    int32_t length;
    uint16_t bpm;

    std::list<std::array<uint16_t, 3>> *notes;
};
