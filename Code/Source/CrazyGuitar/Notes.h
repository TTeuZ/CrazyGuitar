#pragma once

// Standard includes
#include <list>

// Personal includes
#include "Constants.h"
#include "NoteAction.h"

// Unreal Includes
#include "Engine/World.h"

class Notes {
    friend void ANoteAction::setNotes(Notes* const newNotes);

   public:
    Notes();
    virtual ~Notes();

    void startNotes();

    void handleHit(const int8_t& chord);

    void removeNote(ANoteAction* const note);

    // TODO: Add a paramenter string (maybe) that will be used to create all the notes
    void createNotes(UWorld* const world);

    void clearNoteActions();

   private:
    std::list<ANoteAction*> noteActions;
    uint8_t noteSpeed;
};