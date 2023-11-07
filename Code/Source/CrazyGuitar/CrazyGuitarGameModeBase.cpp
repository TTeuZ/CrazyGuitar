#include "CrazyGuitarGameModeBase.h"

// Game Includes
#include "ChartPawn.h"

ACrazyGuitarGameModeBase::ACrazyGuitarGameModeBase() {
    this->DefaultPawnClass = AChartPawn::StaticClass();
}