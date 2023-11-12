#include "CrazyGuitarGameModeBase.h"

// Game Includes
#include "CrazyGuitarController.h"
#include "Chart.h"

ACrazyGuitarGameModeBase::ACrazyGuitarGameModeBase() {
    this->PlayerControllerClass = ACrazyGuitarController::StaticClass();
    this->DefaultPawnClass = AChart::StaticClass();
}
