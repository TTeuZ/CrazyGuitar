#pragma once

// Standard includes
#include <list>

// Personal includes
#include "NoteAction.h"

// Unreal Includes
#include "Engine/World.h"

class CRAZYGUITAR_API Notes {
    friend void ANoteAction::setNotes(Notes* const newNotes);

   public:
    Notes();

    /**
     * The Unreal Engine cleans up all the actors in game when the game is destroyed
     * So, as the noteActions list holds AActors pointers, if we try to delete them here we get segmentation fault
     */
    virtual ~Notes() = default;

    void startNotes();

    void removeNote(ANoteAction* const note);

    void createNotes(UWorld* const world);

    void clearNoteActions();

   private:
    const static float ZJUMP;
    const static FVector DEFAULT_NOTE_LOCATION;

    std::list<ANoteAction*> noteActions;
    uint8_t noteSpeed;
};