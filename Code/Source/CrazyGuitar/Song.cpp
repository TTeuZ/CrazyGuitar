#include "Song.h"

// Standard includes
#include <cstdint>
#include <fstream>
#include <iostream>

// Unreal includes
#include "Containers/UnrealString.h"
#include "Logging/LogMacros.h"
#include "Misc/Paths.h"

const FString Song::BASE_DIR_PATH{FPaths::ProjectContentDir() + TEXT("Songs/")};

Song::Song() : dirPath{""}, name{""}, artist{""}, genre{""}, length{0}, bpm{0} {}

Song::Song(const FString& dirPath) : Song{} {
    this->setDirPath(dirPath);
    this->readDirPath();
}

Song::Song(const std::string& name, const std::string& artist, const std::string& genre, const uint16_t length,
           const uint16_t bpm)
    : dirPath{""}, name{name}, artist{artist}, genre{genre}, length{length}, bpm{bpm} {}

std::string Song::getLength() const {
    int32_t minutes{this->length / 60};
    int32_t seconds{this->length % 60};
    std::string strLength{""};

    if (minutes < 10) strLength += "0";

    strLength += std::to_string(minutes);
    strLength += ":";

    if (seconds < 10) strLength += "0";

    strLength += std::to_string(seconds);
    return strLength;
}

uint16_t Song::getBPM() const { return this->bpm; }

const std::list<std::array<uint16_t, 3>>& Song::getRawNotes() const { return this->rawNotes; }

void Song::setDirPath(const FString& newDirPath) {
    if (!this->dirExists(newDirPath)) {
        UE_LOG(LogTemp, Warning, TEXT("Song::setDirPath: Directory does not exists"));
        return;
    }
    if (newDirPath == this->dirPath) {
        UE_LOG(LogTemp, Warning, TEXT("Song::setDirPath: Directory already set"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Song::setDirPath: Directory set"));
    UE_LOG(LogTemp, Log, TEXT("Song::setDirPath: Old directory: %s"), *this->dirPath);
    this->dirPath = newDirPath;
}

void Song::setLength(std::string& newLength) {
    int32_t minutes{std::stoi(newLength.substr(0, newLength.find(':')))};
    int32_t seconds{std::stoi(newLength.substr(newLength.find(':') + 1, newLength.length()))};
    this->length = minutes * 60 + seconds;
}

void Song::readDirPath() {
    UE_LOG(LogTemp, Log, TEXT("Song::setDirPath: Reading info and notes"));
    this->readInfo();
    this->readNotes();
}

bool Song::dirExists(const FString& path) const {
    UE_LOG(LogTemp, Log, TEXT("Song::dirExists: Path: %s"), *path);

    if (path.IsEmpty()) {
        UE_LOG(LogTemp, Warning, TEXT("Song::dirExists: Path is empty"));
        return false;
    }

    if (!path.StartsWith(Song::BASE_DIR_PATH)) {
        std::cout << "Song::dirExists: Path is not in the base directory" << std::endl;
        UE_LOG(LogTemp, Log, TEXT("Song::dirExists: Path is not in the base directory"));
        UE_LOG(LogTemp, Log, TEXT("Song::dirExists: Running again with base directory"));
        FString newPath{Song::BASE_DIR_PATH + path};
        return this->dirExists(newPath);
    }

    if (!FPaths::DirectoryExists(path)) {
        UE_LOG(LogTemp, Warning, TEXT("Song::dirExists: Directory does not exists"));
        return false;
    }

    return true;
}

void Song::readInfo() {
    UE_LOG(LogTemp, Log, TEXT("Song::readInfo: Reading info"));

    std::string path{std::string(TCHAR_TO_UTF8(*Song::BASE_DIR_PATH)) + std::string(TCHAR_TO_UTF8(*this->dirPath)) +
                     "/song.info"};
    std::ifstream file{path};

    if (!file.is_open()) return;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (line == "") continue;
        if (line[0] == '#') continue;

        std::string key = line.substr(0, line.find('='));
        std::string value = line.substr(line.find('=') + 1, line.length());

        // Not possible to use switch with string
        if (key == "name")
            this->name = value;
        else if (key == "artist")
            this->artist = value;
        else if (key == "genre")
            this->genre = value;
        else if (key == "length")
            this->setLength(value);
        else if (key == "bpm")
            this->bpm = std::stoi(value);
    }

    file.close();

    UE_LOG(LogTemp, Log, TEXT("Song::readInfo: Name: %s"), *FString(this->name.c_str()));
    UE_LOG(LogTemp, Log, TEXT("Song::readInfo: Artist: %s"), *FString(this->artist.c_str()));
    UE_LOG(LogTemp, Log, TEXT("Song::readInfo: Genre: %s"), *FString(this->genre.c_str()));
    UE_LOG(LogTemp, Log, TEXT("Song::readInfo: Length: %d"), this->length);
    UE_LOG(LogTemp, Log, TEXT("Song::readInfo: BPM: %d"), this->bpm);
}

void Song::readNotes() {
    uint8_t chord;
    uint16_t size;
    uint16_t position;

    UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Reading notes"));

    std::string path{std::string(TCHAR_TO_UTF8(*Song::BASE_DIR_PATH)) + std::string(TCHAR_TO_UTF8(*this->dirPath)) +
                     "/song.notes"};
    std::ifstream file{path};

    if (!file.is_open()) return;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (line == "") continue;
        if (line[0] == '#') continue;

        chord = std::stoi(line.substr(0, line.find(' ')));
        line = line.substr(line.find(' ') + 1, line.length());
        size = std::stoi(line.substr(0, line.find(' ')));
        line = line.substr(line.find(' ') + 1, line.length());
        position = std::stoi(line.substr(0, line.find(' '))) * (this->bpm / 8);
        UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Chord: %d, Size: %d, Position: %d"), chord, size, position);

        if (file.fail()) break;

        this->rawNotes.push_back({chord, size, position});
    }
    file.close();
}
