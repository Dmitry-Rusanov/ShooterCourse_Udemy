// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"

#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


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
 
		//Получить базовое вращение прицеливания
		const FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();
		//Получить вращение движения
		const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation,AimRotation).Yaw;

		if (ShooterCharacter->GetVelocity().Size() > 0.f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}
	}
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}
