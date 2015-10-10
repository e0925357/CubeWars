// This source code file is protected by the MIT License (MIT)

#pragma once

#include "GameFramework/PlayerController.h"
#include "NamedPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API ANamedPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	ANamedPlayerController();

	UFUNCTION(BlueprintCallable, Category = Stats)
	void setPlayerNameBP(const FString& name);

	UFUNCTION(Server, WithValidation, reliable)
	void setPlayerNameServer(const FString& name);

	const FString& getPlayerName();

	bool IsNameInitialized() const;

	UFUNCTION(Client, reliable)
	void UpdatePlayerNamesClient(const FString& Player1Name, const FString& Player2Name);

	UFUNCTION(BlueprintImplementableEvent, Category = Players)
	void OnUpdatePlayerNames(const FString& Player1Name, const FString& Player2Name);

private:
	bool bNameInitialized;

	UPROPERTY()
	FString playerName;
	
};
