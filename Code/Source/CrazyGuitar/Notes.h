#pragma once

// Standard includes
#include <cstdint>
#include <list>

// Personal includes
#include "NoteAction.h"

// Unreal Includes
#include "Engine/World.h"

class Song;  // Forward declaration

class CRAZYGUITAR_API Notes {
    friend void ANoteAction::setNotes(Notes* const newNotes);

   public:
    Notes();
    Notes(UWorld* const world);
    Notes(UWorld* const world, uint16_t songBPM);

    /**
     * The Unreal Engine cleans up all the actors in game when the game is destroyed
     * So, as the noteActions list holds AActors pointers, if we try to delete them here we get segmentation fault
     */
    virtual ~Notes() = default;

    uint16_t getBPM() const;
    float getGameBPM() const;

    /*
     *   Unreal Engine enforce us to use the name newChord instead of chord, otherwise
     *   we received a compilation error
     */
    void setWorld(UWorld* const newWorld);
    void setBPM(const uint16_t newBPM);
    void setGameBPM(const uint16_t newGameBPM);

    void startNotes();
    void removeNote(ANoteAction* const note);
    void createProceduralNotes(const uint32_t n = 100);
    void createSongNotes(const Song& song);
    void clearNoteActions();

   private:
    const static FVector DEFAULT_NOTE_LOCATION;
    constexpr static uint16_t BPM_MULTIPLIER{275};
    constexpr static float BPM_DIVIDER{100.f};

    bool addNoteAction(const uint8_t chord, const float position);

    int32_t gameBPM;
    UWorld* world;
    std::list<ANoteAction*> noteActions;
};
