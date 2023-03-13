// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"
#include "Horror_GameCharacter.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningActor = GetOwningActor();
	HorrorGameCharacter = Cast<AHorror_GameCharacter>(GetOwningActor());
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	PlayerTransformSpeed = OwningActor->GetVelocity();
	Speed = PlayerTransformSpeed.Length();
	if(HorrorGameCharacter)
	{
		IsFalling = HorrorGameCharacter->GetMovementComponent()->IsFalling();
		IsCrouching = HorrorGameCharacter->GetMovementComponent()->IsCrouching();


		Direction = UKismetAnimationLibrary::CalculateDirection(PlayerTransformSpeed, OwningActor->GetActorRotation());
		
		// Set player pitch for aim offset
		Pitch = HorrorGameCharacter->GetBaseAimRotation().Pitch;
		if(Pitch >= 180)
		{
			Pitch -= 360;
		}
	}
}
