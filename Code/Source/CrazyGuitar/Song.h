#pragma once

#include <cstdint>
#include <string>
#include "CrazyGuitar/Notes.h"
class Song {
public:
    Song();
    Song(const std::string dirPath);
    Song(const std::string name, const std::string artist, const std::string genre, const uint16_t length, const uint16_t bpm);
    virtual ~Song() = default;

    std::string getDirPath() const;
    void setDirPath(const std::string newDirPath);

    std::string getLength() const;

    int getBPM() const;

private:
    void readSongInfo();
    void readSongNotes();

    std::string dirPath;
    std::string name;
    std::string artist;
    std::string genre;
    uint16_t length;
    uint16_t bpm;

    Notes *notes;
};
