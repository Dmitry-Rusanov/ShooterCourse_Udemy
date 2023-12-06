// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AShooterCharacter::AShooterCharacter() :
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f)
{
 	// Установите этот символ для вызова Tick() в каждом кадре. Вы можете отключить эту функцию,
 	// чтобы повысить производительность, если она вам не нужна.
	PrimaryActorTick.bCanEverTick = true;
	//Создание и настройка рычага камеры
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;//Длина рычага
	CameraBoom->bUsePawnControlRotation = true;//Рычаг использует вращение контроллера игрока

	// Создание и настройка камеры
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;//Что бы камера не крутилась на рычаге от контроллера

	//Персонаж не вращается от контроллера, управление только камерой
	bUseControllerRotationPitch = false;//не использовать наклон контроллера
	bUseControllerRotationYaw = false;	//не использовать рысканье контроллера
	bUseControllerRotationRoll = false; //не использовать поворот контроллера

	//Настройка движения персонажа
	GetCharacterMovement()->bOrientRotationToMovement = true;//Персонаж двигается в направлении ввода
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);// ... с этой скоростью вращения
	GetCharacterMovement()->JumpZVelocity = 600.f;//Скорость прыжка
	GetCharacterMovement()->AirControl = 0.2f;  
}

//Вызывается при запуске игры или при появлении
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

/**
 * Движение вперед / назад 
 * @param Value 1.0f - вперед, -1,0f - назад
 */
void AShooterCharacter::MoveForward(float Value)
{
	if((Controller != nullptr) && (Value != 0.0f))
	{
		//Получаем вращение персонажа
		const FRotator Rotation { Controller->GetControlRotation() };
		//Выделяем вращение по горизонтали (рысканье), наклон (Pitch) и вращение (Roll), будут обнулены
		const FRotator RotationYaw{0.f, Rotation.Yaw, 0.f};
		//Получаем направление персонажа
		const FVector Direction {FRotationMatrix{RotationYaw}.GetUnitAxis(EAxis::X)};//еденичный вектор
		//Добавляем движение в заданном направлении
		AddMovementInput(Direction, Value);
	}
}

/**
 * Движение вправо / влево
 * @param Value 1.0f - вправо, -1.0f - влево 
 */
void AShooterCharacter::MoveRight(float Value)
{
	if((Controller != nullptr) && (Value != 0.0f))
	{
		//Получаем вращение персонажа
		const FRotator Rotation { Controller->GetControlRotation() };
		//Выделяем вращение по горизонтали (рысканье), наклон (Pitch) и вращение (Roll), будут обнулены
		const FRotator RotationYaw{0.f, Rotation.Yaw, 0.f};
		//Получаем направление персонажа
		const FVector Direction {FRotationMatrix{RotationYaw}.GetUnitAxis(EAxis::Y)};//еденичный вектор
		//Добавляем движение в заданном направлении
		AddMovementInput(Direction, Value);
	}
}

/**
 * Вызывается посредством ввода для вращения вправо/влево с заданной скоростью.
 * Рассчитывает угол поворота для каждого кадра.
 * Применимо для джойстика и клавиш курсора.
 * @param Rate Нормализованное значение скорости, т.е. 1.0f - 100% желаемой скорости вращения
 */
void AShooterCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());// deg/sec * sec/frame = deg/frame unit
}

/**
 * Вызывается посредством ввода для вращения вверх/вниз с заданной скоростью.
 * Рассчитывает угол поворота для каждого кадра.
 * Применимо для джойстика и клавиш курсора.
 * @param Rate Нормализованное значение скорости, т.е. 1.0f - 100% желаемой скорости вращения
 */
void AShooterCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

/**
 * Стрельба из оружия 
 */
void AShooterCharacter::FireWeapon()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}
	}
}

/** Вызывается каждый кадр */
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/** Вызывается для привязки функциональности к вводу */
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	//Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &AShooterCharacter::FireWeapon);
	//PlayerInputComponent->BindAction("FireButton", IE_Released, this, &AShooterCharacter::FireWeapon);
	
}

