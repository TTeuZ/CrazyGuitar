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
    // Notes(const Notes&) = delete;
    Notes(UWorld* const world, uint16_t bpm);

    /**
     * The Unreal Engine cleans up all the actors in game when the game is destroyed
     * So, as the noteActions list holds AActors pointers, if we try to delete them here we get segmentation fault
     */
    virtual ~Notes() = default;

    void setWorld(UWorld* const newWorld);

    int getBPM() const;
    void setBPM(const uint16_t newBPM);

    void startNotes();

    void removeNote(ANoteAction* const note);

    void createProceduralNotes(const int32_t n = 100);
    void createSongNotes(const Song* song);

    // bool addNoteAction(ANoteAction* const note);
    bool addNoteAction(const uint8_t chord, const float position);

    void clearNoteActions();

   private:
    const static FVector DEFAULT_NOTE_LOCATION;

    UWorld* world;
    std::list<ANoteAction*> noteActions;
    uint16_t bpm;
};
