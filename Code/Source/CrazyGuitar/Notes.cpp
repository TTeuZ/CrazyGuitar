#include "Notes.h"

Notes::Notes() : noteSpeed{1} {}

Notes::~Notes() { this->clearNoteActions(); }

void Notes::startNotes() {
    std::list<ANoteAction*>::iterator it{this->noteActions.begin()};
    for (; it != this->noteActions.end(); ++it) (*it)->setCanMove(true);
}

void Notes::handleHit(const int8_t& chord) {
    std::list<ANoteAction*>::iterator it{this->noteActions.begin()};
    for (; it != this->noteActions.end(); ++it) {
        FVector location{(*it)->getPosition()};
        if ((*it)->isHit(chord, location.Y)) {
            (*it)->playNote();

            (*it)->Destroy();
            it = this->noteActions.erase(it);
        }
    }
}

void Notes::createNotes(UWorld* const world) {
    ANoteAction* aux{nullptr};
    ANoteAction* aux2{nullptr};

    for (size_t i{0}; i < 30; ++i) {
        aux = world->SpawnActor<ANoteAction>();
        aux->setChord(FMath::RandRange(0, 3));

        float prevYLocation{0.f};
        float min{0.f};
        if (i >= 1) {
            aux2 = this->noteActions.back();
            prevYLocation = aux2->getPosition().Y;
            if (aux2->getChord() == aux->getChord()) min = 30.f;
        }

        float yLocation = prevYLocation + FMath::RandRange(min, 60.f);
        float zLocation = -Constants::ZJUMP * aux->getChord();
        aux->setPosition(Constants::DEFAULT_NOTE_LOCATION + FVector(0.f, yLocation, zLocation));
        this->noteActions.push_back(aux);
    }
}

/**
 * The Unreal Engine cleans up all the actors in game when the game is destroyed
 * So, as the noteActions list holds AActors pointers, if we try to delete them here we get segmentation fault
 */
void Notes::clearNoteActions() {
    while (!this->noteActions.empty()) this->noteActions.pop_back();
    // while (!this->noteActions.empty()) {
    //     ANoteAction* aux = this->noteActions.back();
    //     if (!aux->IsPendingKill()) {
    //         aux->Destroy();
    //         this->noteActions.pop_back();
    //     }
    // }
}