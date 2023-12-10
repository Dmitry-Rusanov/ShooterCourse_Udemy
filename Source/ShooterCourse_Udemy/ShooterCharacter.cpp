// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

//================================================================//
AShooterCharacter::AShooterCharacter() :
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	HipTurnRate(90.f),
	HipLookUpRate(90.f),
	AimingTurnRate(20.f),
	AimingLookUpRate(20.f),
	MouseHipTurnRate(1.f),
	MouseHipLookUpRate(1.f),
	MouseAimingTurnRate(0.2f),
	MouseAimingLookUpRate(0.2f),
	bAiming(false),
	CameraDefaultFOV(0.f),
	CameraZoomedFOV(35.f),
	CameraCurrentFOV(0.f),
	ZoomInterpSpeed(10.f)
	
{
 	// Установите этот символ для вызова Tick() в каждом кадре. Вы можете отключить эту функцию,
 	// чтобы повысить производительность, если она вам не нужна.
	PrimaryActorTick.bCanEverTick = true;
	//Создание и настройка рычага камеры
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 180.f;//Длина рычага
	CameraBoom->bUsePawnControlRotation = true;//Рычаг использует вращение контроллера игрока
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);
	
	// Создание и настройка камеры
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;//Что бы камера не крутилась на рычаге от контроллера

	//Персонаж не вращается от контроллера, управление только камерой
	bUseControllerRotationPitch = false;//не использовать наклон контроллера
	bUseControllerRotationYaw = true;	// использовать рысканье контроллера
	bUseControllerRotationRoll = false; //не использовать поворот контроллера

	//Настройка движения персонажа
	GetCharacterMovement()->bOrientRotationToMovement = false;//Персонаж не двигается в направлении ввода
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);// ... с этой скоростью вращения
	GetCharacterMovement()->JumpZVelocity = 600.f;//Скорость прыжка
	GetCharacterMovement()->AirControl = 0.2f;  
}
//================================================================//
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
	}
}
//================================================================//
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
//================================================================//
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
//================================================================//
void AShooterCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());// deg/sec * sec/frame = deg/frame unit
}
//================================================================//
void AShooterCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
//================================================================//
void AShooterCharacter::Turn(float Value)
{
	float TurnScaleFactor{1.f};

	TurnScaleFactor = (bAiming)? MouseAimingTurnRate : MouseHipTurnRate;
	AddControllerYawInput(Value * TurnScaleFactor);
}
//================================================================//
void AShooterCharacter::LookUp(float Value)
{
	float TurnScaleFactor{1.f};

	TurnScaleFactor = (bAiming)? MouseAimingLookUpRate : MouseHipLookUpRate;
	AddControllerPitchInput(Value * TurnScaleFactor);
}
//================================================================//
bool AShooterCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamEndLocation)
{
	//Viewport size
	FVector2d ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	//Crosshair location
	FVector2d CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	CrosshairLocation.Y -= 50.f;
	//Проецирование экраннго положения прицела в мировое
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		FHitResult ScreenTraceHit;
		const FVector Start{CrosshairWorldPosition}; //Начало луча трассировки
		const FVector End{CrosshairWorldPosition + CrosshairWorldDirection * 50'000.f}; //Конец луча трассировки
		OutBeamEndLocation = End;//Конечная точка дымного следа, если никуда не попали

		//Трассировка от перекрестия прицела в мировое пространство
		GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECollisionChannel::ECC_Visibility);
		if (ScreenTraceHit.bBlockingHit) //Попадание луча трассировки
		{
			OutBeamEndLocation = ScreenTraceHit.Location; //Новое положение дымного следа
		}

		//Вторая трассировка от ствола оружия до перекрестия прицела
		FHitResult WeaponTraceHit;
		const FVector WeaponTraceStart{MuzzleSocketLocation};
		const FVector WeaponTraceEnd{OutBeamEndLocation};

		GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponTraceStart, WeaponTraceEnd,
		                                     ECollisionChannel::ECC_Visibility);
		if (WeaponTraceHit.bBlockingHit)
		{
			OutBeamEndLocation = WeaponTraceHit.Location;
		}
		return true;
	}
	return false;
}
//================================================================//
void AShooterCharacter::FireWeapon()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
	if (const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket"))
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}
		FVector BeamEnd;
		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamEnd);

		if (bBeamEnd)
		{
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticles, BeamEnd);
			}
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && HipFireMontege)
	{
		AnimInstance->Montage_Play(HipFireMontege);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}
}
//================================================================//
void AShooterCharacter::AimingButtonPressed()
{
	bAiming = true;
}
//================================================================//
void AShooterCharacter::AimingButtonReleased()
{
	bAiming = false;
}
//================================================================//
void AShooterCharacter::CameraInterpZoom(float DeltaTime)
{
	if (bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraZoomedFOV, DeltaTime, ZoomInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraDefaultFOV, DeltaTime, ZoomInterpSpeed);
	}
	FollowCamera->SetFieldOfView(CameraCurrentFOV);
}
//================================================================//
void AShooterCharacter::SetLookRates()
{
	if (bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipTurnRate;
	}
	
}
//================================================================//
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Интерполяция для масштабирования при прицеливании
	CameraInterpZoom(DeltaTime);
	// Изменение скорости вращения при прицеливании и без него
	SetLookRates();

}
//================================================================//
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	//Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::LookUp);

	//Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//Aiming
	PlayerInputComponent->BindAction("AimingButton", IE_Pressed, this, &AShooterCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("AimingButton", IE_Released, this, &AShooterCharacter::AimingButtonReleased);
	//Fire
	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &AShooterCharacter::FireWeapon);
	
	
}
//================================================================//

//================================================================//

//================================================================//

//================================================================//

//================================================================//

//================================================================//

//================================================================//

//================================================================//

//================================================================//