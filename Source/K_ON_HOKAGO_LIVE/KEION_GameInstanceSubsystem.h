// NONE OF THE CONTENT IN THIS IS MINE all things are the authors creation of kyoto animataion  

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KEION_GameInstanceSubsystem.generated.h"

UENUM(BlueprintType)
enum class KeionDifficulty : uint8 {
	EASY UMETA(DisplayName = "EASY"),
	MEDIUM UMETA(DisplayName = "MEDIUM"),
	HARD UMETA(DisplayName = "HARD")
};

/**
 * 
 */
UCLASS()
class K_ON_HOKAGO_LIVE_API UKEION_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UKEION_GameInstanceSubsystem();
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	UFUNCTION(BlueprintCallable, Category="Keion_GameInstanceSubsytem|Difficulty")
	KeionDifficulty GetKeionDifficulty() { return CurrentKeionDifficulty; }

	UFUNCTION(BlueprintCallable, Category="Keion_GameInstanceSubsytem|Difficulty")
	void SetKeionDifficulty(KeionDifficulty NewDifficulty) { CurrentKeionDifficulty = NewDifficulty; }

private:
	KeionDifficulty CurrentKeionDifficulty;
};
