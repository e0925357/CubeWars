// Fill out your copyright notice in the Description page of Project Settings.

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