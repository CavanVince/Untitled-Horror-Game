// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Horror_GameCharacter.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HORROR_GAME_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerTransformSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCrouching;

private:
	AActor* OwningActor;
	AHorror_GameCharacter* HorrorGameCharacter;
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
