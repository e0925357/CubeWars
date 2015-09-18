// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "CubeWarsPlayerState.h"
#include "Net/UnrealNetwork.h"

ACubeWarsPlayerState::ACubeWarsPlayerState()
{
	bReplicates = true;
}

void ACubeWarsPlayerState::SetTeamNumber(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
}

int32 ACubeWarsPlayerState::GetTeamNumber() const
{
	return TeamNumber;
}

void ACubeWarsPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACubeWarsPlayerState, TeamNumber);
}

void ACubeWarsPlayerState::AddPoints(int32 amount)
{
	Points += amount;
}

int32 ACubeWarsPlayerState::GetPoints()
{
	return Points;
}
