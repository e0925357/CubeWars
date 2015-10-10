// This source code file is protected by the MIT License (MIT)

#pragma once

#include "GameFramework/GameMode.h"
#include "../NamedPlayerController.h"
#include "NameAwareGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API ANameAwareGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	
	/** Adds the player name to the name list */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Removes the player name from the name list */
	virtual void Logout(AController* Exiting) override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = Players)
	const TArray<FString>& GetPlayerNames() const;

	UFUNCTION(BlueprintImplementableEvent, Category = Players)
	void OnPlayerNamesChange();

	void PlayerNamesChange();
	
private:
	TArray<FString> NameArray;

	TArray<ANamedPlayerController*> UninitializedControllersArray;
};
