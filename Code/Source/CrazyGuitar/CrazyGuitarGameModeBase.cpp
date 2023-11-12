#include "CrazyGuitarGameModeBase.h"

// Game Includes
#include "CrazyGuitarPlayerState.h"
#include "CrazyGuitarController.h"
#include "Chart.h"

ACrazyGuitarGameModeBase::ACrazyGuitarGameModeBase() {
    this->PlayerControllerClass = ACrazyGuitarController::StaticClass();
    this->DefaultPawnClass = AChart::StaticClass();
    this->PlayerStateClass = ACrazyGuitarPlayerState::StaticClass();
}
