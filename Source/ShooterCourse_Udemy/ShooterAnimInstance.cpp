// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"

#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


/** Собственная реализация NativeUpdateAnimation
 * @param  DeltaTime Время обновления
 */
void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());	
	}
	if (ShooterCharacter)
	{
		// получение горизонтальной скорости персонажа без 'Z' составляющей из velocity
		FVector Velocity { ShooterCharacter->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();
		//персонаж в воздухе?
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();
		//Двигается ли персонаж?
		bIsAccelerating =  (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f) ? true: false;

	}
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}
