#include "CrazyGuitarGameModeBase.h"

// Game Includes
#include "Chart.h"

ACrazyGuitarGameModeBase::ACrazyGuitarGameModeBase() {
    this->DefaultPawnClass = AChart::StaticClass();
}
