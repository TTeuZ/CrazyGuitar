#include "Notes.h"

// Personal Includes
#include "Chart.h"
#include "CrazyGuitar/Chord.h"

const float Notes::ZJUMP{(AChart::CHART_SIZE.Z * 2) / (AChart::MAX_CHORDS + 1)};
const FVector Notes::DEFAULT_NOTE_LOCATION{AChart::CHART_LOCATION.X + AChart::CHART_SIZE.Y * 1.1f,
                                           AChord::CHORD_BASE_POSITION.Z, AChart::CHART_LOCATION.Z + 20.f};

Notes::Notes() : noteSpeed{1} {}

void Notes::startNotes() {
    std::list<ANoteAction*>::iterator it{this->noteActions.begin()};
    for (; it != this->noteActions.end(); ++it) (*it)->setCanMove(true);
}

void Notes::removeNote(ANoteAction* const note) {
    this->noteActions.remove(note);
    note->Destroy();
}

void Notes::createNotes(UWorld* const world) {
    ANoteAction* aux{nullptr};
    ANoteAction* aux2{nullptr};

    for (size_t i{0}; i < 30; ++i) {
        aux = world->SpawnActor<ANoteAction>();
        aux->setChord(FMath::RandRange(0, 3));

        float prevXLocation{0.f};
        float min{0.f};
        if (i >= 1) {
            aux2 = this->noteActions.back();
            prevXLocation = aux2->getPosition().X;
            if (aux2->getChord() == aux->getChord()) min = 30.f;
        }

        float xLocation = prevXLocation + FMath::RandRange(min, 60.f);
        float yLocation = -AChord::CHORD_POS_JUMP * aux->getChord();
        aux->setPosition(Notes::DEFAULT_NOTE_LOCATION + FVector(xLocation, yLocation, 0.f));
        aux->setNotes(this);
    }
}

void Notes::clearNoteActions() {
    while (!this->noteActions.empty()) {
        ANoteAction* aux = this->noteActions.back();
        if (!aux->IsPendingKill()) aux->Destroy();

        this->noteActions.pop_back();
    }
}
