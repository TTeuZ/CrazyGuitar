#include "Notes.h"

// Personal Includes
#include "Chart.h"
#include "Chord.h"
#include "Song.h"

const FVector Notes::DEFAULT_NOTE_LOCATION{AChart::CHART_LOCATION.X + AChart::CHART_SIZE.Y * 1.2f,
                                           AChord::CHORD_BASE_POSITION.Z, AChart::CHART_LOCATION.Z + 20.f};

Notes::Notes() : world{nullptr}, noteActions{std::list<ANoteAction*>{}}, bpm{80} {}

Notes::Notes(UWorld* const world) : Notes() {
    this->world = world;
}

Notes::Notes(UWorld* const world, uint16_t bpm) : Notes(world) { this->bpm = bpm; }

void Notes::setWorld(UWorld* const newWorld) { this->world = newWorld; }

int Notes::getBPM() const { return this->bpm; }
void Notes::setBPM(const uint16_t newBPM) { this->bpm = newBPM; }

void Notes::startNotes() {
    std::list<ANoteAction*>::iterator it{this->noteActions.begin()};
    for (; it != this->noteActions.end(); ++it) (*it)->setCanMove(true);
}

void Notes::removeNote(ANoteAction* const note) {
    this->noteActions.remove(note);
    note->Destroy();
}

void Notes::createProceduralNotes(const int32_t n) {
    for (size_t i{0}; i < n; ++i) {
        int32_t min{0};
        int32_t max{4 * (this->bpm)};
        float prevXLocation{0.f};
        int32_t chord{FMath::RandRange(0, 3)};
        ANoteAction* last{this->noteActions.back()};
        if (last != nullptr) {
            prevXLocation = last->getPosition().X;
            if (last->getChord() == chord) min = 100;
        }
        int32_t position{FMath::RandRange(min, max)};
        int32_t multiple = this->bpm / 8;
        position = (position / multiple) * multiple;
        if (position % multiple != 0) position += multiple;
        if (position < 40) position = min;

        this->addNoteAction(chord, position);
    }
}

void Notes::createSongNotes(const Song* song) {
    for (auto& note : song->getNotes()) {
        this->addNoteAction(note[0], note[2]);
    }
    this->bpm = song->getBPM();
}

bool Notes::addNoteAction(const uint8_t chord, const float position) {
    ANoteAction* last{this->noteActions.back()};
    float lastXLocation{DEFAULT_NOTE_LOCATION.X};

    if (last != nullptr) {
        if (last->getChord() == chord && position <= 0) return false;
        lastXLocation = last->getPosition().X;
    }

    ANoteAction* aux{this->world->SpawnActor<ANoteAction>()};
    if (aux == nullptr) return false;
    aux->SetFolderPath(TEXT("Notes"));

    FVector location{
        /* X */ lastXLocation + position,
        /* Y */ Notes::DEFAULT_NOTE_LOCATION.Y - AChord::CHORD_POS_JUMP * chord,
        /* Z */ Notes::DEFAULT_NOTE_LOCATION.Z};

    aux->setChord(chord);
    aux->setPosition(location);
    aux->setNotes(this);
    this->noteActions.push_back(aux);

    return true;
}

void Notes::clearNoteActions() {
    while (!this->noteActions.empty()) {
        ANoteAction* aux{this->noteActions.back()};
        if (!aux->IsPendingKill()) aux->Destroy();

        this->noteActions.pop_back();
    }
}
