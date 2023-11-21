#include "InGameModeBase.h"

// Game Includes
#include "GameFramework/GameUserSettings.h"
#include "PlayerSave.h"
#include "ChartController.h"
#include "UI/ChartHUD.h"
#include "Chart.h"

AInGameModeBase::AInGameModeBase() {
    this->PlayerStateClass = APlayerSave::StaticClass();
    this->PlayerControllerClass = AChartController::StaticClass();
    this->HUDClass = AChartHUD::StaticClass();
    this->DefaultPawnClass = AChart::StaticClass();
}
