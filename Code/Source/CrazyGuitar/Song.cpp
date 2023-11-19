#include "Song.h"
#include <fstream>

Song::Song() : dirPath(""), name(""), artist(""), genre(""), length(0), bpm(0), notes(nullptr) {}

Song::Song(const std::string dirPath) : Song() {
    readSongInfo();
    readSongNotes();
}

Song::Song(const std::string name, const std::string artist, const std::string genre, const uint16_t length, const uint16_t bpm) 
    : dirPath(""), name(name), artist(artist), genre(genre), length(length), bpm(bpm), notes(nullptr) {}

std::string Song::getDirPath() const {
    return dirPath;
}

void Song::setDirPath(const std::string newDirPath) {
    if (newDirPath == "") {
        return;
    }

    if (newDirPath == this->dirPath) {
        return;
    }

    if (!std::ifstream(dirPath + "/song.info").is_open()) {
        return;
    }

    if (this->dirPath != "") {
        delete notes;
    }

    if (dirPath[dirPath.length() - 1] == '/') {
        this->dirPath = newDirPath.substr(0, newDirPath.length() - 1);
    } else {
        this->dirPath = newDirPath;
    }
}

void Song::readSongInfo() {
    std::ifstream file(dirPath + "/song.info");

    if (!file.is_open()) {
        return;
    }

    file.close();
}

void Song::readSongNotes() {
    std::ifstream file(dirPath + "/song.notes");
    if (!file.is_open()) {
        return;
    }
    file.close();
}
