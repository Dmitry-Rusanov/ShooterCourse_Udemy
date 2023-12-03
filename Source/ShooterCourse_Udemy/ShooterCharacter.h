// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SHOOTERCOURSE_UDEMY_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Вызывается при запуске игры или при появлении
	virtual void BeginPlay() override;
	
	// Движение вперед / назад 
	void MoveForward(float Value);

	// Движение вправо / влево 
	void MoveRight(float Value);

	//Вызывается посредством ввода для вращения вправо/влево с заданной скоростью (для джойстика)
	void TurnAtRate(float Rate);

	//Вызывается посредством ввода для вращения вверх/вниз с заданной скоростью (для джойстика)
	void LookUpAtRate(float Rate);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//**********************				MY SECTIONS				****************************************************
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
private:
	//==================================================================================================================
	//				PRIVATE FIELDS 
	//==================================================================================================================

	//==================================================================================================================
	//				PRIVATE PROPERTIES
	//==================================================================================================================

	/** Держатель камеры */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Камера позади персонажа */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess = "true"))	
	UCameraComponent* FollowCamera;
	
	/** Базовая скорость горизонтального вращения (взгляд вправо/влево), в град/сек */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess = "true"))
	float BaseTurnRate;

	/** Базовая скорость вертикального вращения (взгляд вверх/вниз) в град/сек */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess = "true"))
	float BaseLookUpRate;
	
	//==================================================================================================================
	//				PRIVATE FUNCTIONS
	//==================================================================================================================
public:
	//==================================================================================================================
	//				PUBLIC FIELDS 
	//==================================================================================================================

	//==================================================================================================================
	//				PUBLIC PROPERTIES
	//==================================================================================================================

	//==================================================================================================================
	//				PUBLIC FUNCTIONS
	//==================================================================================================================

	/** Получить указатель на держатель камеры */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return  CameraBoom; }

	/**Получить указатель на камеру */
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return  FollowCamera; }
};
