#include "Song.h"

// Standard includes
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>

// Unreal includes
#include "Components/AudioComponent.h"
#include "Containers/UnrealString.h"
#include "Logging/LogMacros.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"

const FString Song::BASE_DIR_PATH{FPaths::ProjectContentDir() + TEXT("Songs/")};

Song::Song() : dirPath{""}, name{""}, artist{""}, genre{""}, length{0}, bpm{0}, rawNotes{}, world{nullptr}, audioComponent{nullptr}, soundBase{nullptr} {}

Song::Song(const FString& dirPath) : Song{} {
    this->setDirPath(dirPath);
    this->readDirPath();
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

void Song::setLength(const std::string& newLength) {
    int32_t minutes{std::stoi(newLength.substr(0, newLength.find(':')))};
    int32_t seconds{std::stoi(newLength.substr(newLength.find(':') + 1, newLength.length()))};
    this->length = minutes * 60 + seconds;
}

void Song::setWorld(UWorld* const newWorld) { this->world = newWorld; }

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

        std::string key{line.substr(0, line.find('='))};
        std::string value{line.substr(line.find('=') + 1, line.length())};

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
    // uint8_t chord{0};
    // uint16_t size{0};
    // uint16_t position{0};
    // uint16_t tempo{1};
    typedef struct Note {
        uint8_t chord;
        uint16_t size;
        uint16_t position;
        uint16_t tempo;
    } Note;
    uint16_t repeat{1}, notesToRepeat{1};
    std::list<Note*> notes;

    UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Reading notes"));

    std::string path{std::string(TCHAR_TO_UTF8(*Song::BASE_DIR_PATH)) + std::string(TCHAR_TO_UTF8(*this->dirPath)) +
                     "/song.notes"};
    std::ifstream file{path};

    if (!file.is_open()) return;

    // Inicia a leitura das notas
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Line: %s"), *FString(line.c_str()));

        // Trata linhas vazias e comentários
        if (line == "") {
            UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Empty line"));
            continue;
        }
        if (line[0] == '#') {
            UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Comment line"));
            continue;
        }

        // Trata linhas de repetição
        // - Linha de repetição: * <número de repetições> <número de notas a serem repetidas>
        // - Exemplo: * 2 4
        // Caso nao tenha repetição, o número de repetições é 1 automaticamente
        if (line[0] == '*') {
            repeat = std::stoi(line.substr(line.find(' ') + 1, line.length()));
            line = line.substr(line.find(' ') + 1, line.length());
            UE_LOG(LogTemp, Log, TEXT("%s"), *FString(line.c_str()));
            if (line != "") notesToRepeat = std::stoi(line.substr(line.find(' ') + 1, line.length()));
            UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Repeat line"));
            UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Repeat: %d"), repeat);
            UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Notes to repeat: %d"), notesToRepeat);
            continue;
        }

        // Caso tenha passado pelas verificações acima, a linha é uma linha de nota
        UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Note line"));

        // Le as informacoes de cada nota e as adiciona na lista de notas a serem repetidas
        // Caso so tenha uma nota, so realiza a leitura uma vez
        // - Linha de nota: <acorde> <tamanho> <posição> <tempo>
        for (uint16_t i{1}; i <= notesToRepeat; ++i) {
            Note* note = new Note;
            note->chord = std::stoi(line.substr(0, line.find(' ')));
            line = line.substr(line.find(' ') + 1, line.length());

            note->size = std::stoi(line.substr(0, line.find(' ')));
            line = line.substr(line.find(' ') + 1, line.length());

            note->position = std::stoi(line.substr(0, line.find(' ')));
            line = line.substr(line.find(' ') + 1, line.length());

            if (line != "") note->tempo = std::stoi(line);

            note->position *= this->bpm / note->tempo;

            // UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Chord: %d, Size: %d, Position: %d"), chord, size, position);
            UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Chord: %d, Size: %d, Position: %d"), note->chord, note->size,
                   note->position);
            if (i < notesToRepeat) {
                std::getline(file, line);
            }
            notes.push_back(note);
        }

        // Adiciona as notas a lista de notas
        // Caso tenha que repetir, repete
        // Caso nao tenha que repetir, adiciona uma vez
        for (uint16_t i{1}; i <= repeat; ++i) {
            UE_LOG(LogTemp, Log, TEXT("Song::readNotes: Repeat: %d"), i);
            std::list<Note*>::iterator it{notes.begin()};
            for (; it != notes.end(); ++it) {
                this->rawNotes.push_back({(*it)->chord, (*it)->size, (*it)->position});
            }
        }
        repeat = 1;
        notesToRepeat = 1;

        // Limpa a lista de notas a serem repetidas
        std::list<Note*>::iterator it{notes.begin()};
        for (; it != notes.end(); ++it) {
            delete *it;
        }
        notes.clear();
    }
    file.close();
}

void Song::playSong() {
    UE_LOG(LogTemp, Log, TEXT("Song::playSong: Playing song"));

    if (!this->world) {
        UE_LOG(LogTemp, Warning, TEXT("Song::playSong: Unable to get the UWorld."));
        return;
    }

    // FStringAssetReference é criada usando o caminho relativo ao diretório raiz do projeto
    FStringAssetReference assetReference{"/Game/Songs/" + this->dirPath / "song.song"};
    UE_LOG(LogTemp, Log, TEXT("Song::playSong: Full path: %s"), *assetReference.ToString());

    // cria uma Soft Object Reference usando o caminho do Asset
    TSoftObjectPtr<USoundBase> soundBasePtr(assetReference);

    // carrega o USoundBase usando a Soft Object Reference
    this->soundBase = soundBasePtr.LoadSynchronous();
    if (!this->soundBase) {
        UE_LOG(LogTemp, Warning, TEXT("Song::playSong: Unable to load the soundBase."));
        return;
    }

    // se houver um audioComponent existente, significa que tem uma musica tocando
    // entao para a musica e destroi o componente
    if (this->audioComponent) {
        this->audioComponent->Stop();
        this->audioComponent->ConditionalBeginDestroy();
        this->audioComponent = nullptr;
    }

    // cria um UAudioComponent
    this->audioComponent = UGameplayStatics::SpawnSound2D(this->world, this->soundBase);
    if (!this->audioComponent) {
        UE_LOG(LogTemp, Warning, TEXT("Song::playSong: Unable to create the audioComponent."));
        return;
    }

    // inicia a reproducao do som
    this->audioComponent->Play();
}
