// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterMovementComponent.h"

UMyCharacterMovementComponent::UMyCharacterMovementComponent(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {

}

void UMyCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	bWantsToSprint = (Flags&FSavedMove_Character::FLAG_Custom_0) != 0;
}

class FNetworkPredictionData_Client* UMyCharacterMovementComponent::GetPredictionData_Client() const
{
	//check(PawnOwner != NULL); check(PawnOwner->GetLocalRole() < ROLE_Authority);
	if (!ClientPredictionData)
	{
		UMyCharacterMovementComponent* MutableThis = const_cast<UMyCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_MyMovement(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void FSavedMove_MyMovement::Clear()
{
	Super::Clear();
	bSavedWantsToSprint = false;
}

uint8 FSavedMove_MyMovement::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	if (bSavedWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}
	return Result;
}

bool FSavedMove_MyMovement::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	const FSavedMove_MyMovement* NewestMove = static_cast<const FSavedMove_MyMovement*>(NewMove.Get());

	if(bSavedWantsToSprint != NewestMove->bSavedWantsToSprint)
	{
		return false;
	}
	
	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void FSavedMove_MyMovement::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);
	UMyCharacterMovementComponent* CharMov = Cast<UMyCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{
		bSavedWantsToSprint = CharMov->bWantsToSprint;
	}
}

void FSavedMove_MyMovement::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);
	UMyCharacterMovementComponent* CharMov = Cast<UMyCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{
		CharMov->bWantsToSprint = bSavedWantsToSprint;
	}
}

FNetworkPredictionData_Client_MyMovement::FNetworkPredictionData_Client_MyMovement(const UCharacterMovementComponent& ClientMovement):Super(ClientMovement)
{

}

FSavedMovePtr FNetworkPredictionData_Client_MyMovement::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_MyMovement());
}

void UMyCharacterMovementComponent::SetSprinting(bool bSprinting)
{
	bWantsToSprint = bSprinting;
}

float UMyCharacterMovementComponent::GetMaxSpeed() const
{
	if (bWantsToSprint)
	{
		return 600.0f;
	}

	return 300.0f;
}

float UMyCharacterMovementComponent::GetMaxAcceleration() const
{
	float MaxAccel = Super::GetMaxAcceleration();
	if (bWantsToSprint)
	{
		MaxAccel *= SprintAccelerationMultiplier;
	}

	return MaxAccel;
}