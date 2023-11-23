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
    Notes(UWorld* const world, uint16_t bpm);

    /**
     * The Unreal Engine cleans up all the actors in game when the game is destroyed
     * So, as the noteActions list holds AActors pointers, if we try to delete them here we get segmentation fault
     */
    virtual ~Notes() = default;

    uint16_t getBPM() const;

    void setWorld(UWorld* const newWorld);
    void setBPM(const uint16_t newBPM);

    void startNotes();
    void removeNote(ANoteAction* const note);
    void createProceduralNotes(const uint32_t n = 100);
    void createSongNotes(const Song& song);
    void clearNoteActions();

   private:
    const static FVector DEFAULT_NOTE_LOCATION;

    bool addNoteAction(const uint8_t chord, const float position);

    uint16_t bpm;
    UWorld* world;
    std::list<ANoteAction*> noteActions;
};
