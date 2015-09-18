// This source code file is protected by the MIT License (MIT)

#pragma once

#include "Engine/GameInstance.h"
#include "CubeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API UCubeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCubeGameInstance(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category=Stats)
	void SetPlayerName(const FString& PlayerName);

	UFUNCTION(BlueprintCallable, Category = Stats)
	const FString& GetPlayerName();

private:
	FString PlayerName;
};
