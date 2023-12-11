// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class USoundCue;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SHOOTERCOURSE_UDEMY_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	//==================================================================================================================
	//				PUBLIC PROPERTIES & FIELDS
	//==================================================================================================================

	//==================================================================================================================
	//				PUBLIC FUNCTIONS
	//==================================================================================================================

	/** Вызывается каждый кадр */
	virtual void Tick(float DeltaTime) override;

	/** Вызывается для привязки функциональности к вводу */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
		
	/** Устанавливает значения по умолчанию для свойств этого персонажа. */
	AShooterCharacter();
	
	/** Получить указатель на держатель камеры */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return  CameraBoom; }

	/**Получить указатель на камеру */
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return  FollowCamera; }

	/** Получить состояние прицеливания активно / не активно */
	FORCEINLINE bool GetAiming() const {return bAiming;}

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;


protected:
	/**
	 * Вызывается при запуске игры или при появлении
	 */
	virtual void BeginPlay() override;
	

private:
	
	//==================================================================================================================
	//				PRIVATE PROPERTIES & FIELDS
	//==================================================================================================================

	/** Держатель камеры */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Камера позади персонажа */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))	
	UCameraComponent* FollowCamera;
	
	/** Базовая скорость горизонтального вращения (взгляд вправо/влево), в град/сек */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))
	float BaseTurnRate;

	/** Базовая скорость вертикального вращения (взгляд вверх/вниз) в град/сек */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	/** Базовая скорость горизонтального вращения (взгляд вправо/влево), в град/сек, при стрельбе от бедра */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))
	float HipTurnRate;

	/** Базовая скорость вертикального вращения (взгляд вверх/вниз) в град/сек, при стрельбе от бедра */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))
	float HipLookUpRate;

	/** Базовая скорость горизонтального вращения (взгляд вправо/влево), в град/сек, при прицеливании*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))
	float AimingTurnRate;

	/** Базовая скорость вертикального вращения (взгляд вверх/вниз) в град/сек, при прицеливании*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))
	float AimingLookUpRate;

	/** Множитель обзора при использовании Х оси мыши, в режиме стрельбы от бедра */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))
	float MouseHipTurnRate;

	/** Множитель обзора при использовании Y оси мыши, в режиме стрельбы от бедра */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))
	float MouseHipLookUpRate;

	/** Множитель обзора при использовании Х оси мыши, в режиме прицеливания */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))
	float MouseAimingTurnRate;

	/** Множитель обзора при использовании Y оси мыши, в режиме прицеливания */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Camera", meta=(AllowPrivateAccess = "true"))
	float MouseAimingLookUpRate;
	
	/** Звук выстрела из оружия */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Combat", meta=(AllowPrivateAccess = "true"))
	USoundCue* FireSound;

	/** Вспышка у ствола при выстреле */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Combat", meta=(AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;

	/** Разрыв на месте попадания */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Combat", meta=(AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	/** Дымный след от выстрела */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Combat", meta=(AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;
	
	/** Анимационный монтаж стрельбы из оружия */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Combat", meta=(AllowPrivateAccess = "true"))
	UAnimMontage* HipFireMontege;

	/** Вкл/отк приближения при прицеливании */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Combat", meta=(AllowPrivateAccess = "true"))
	bool bAiming;

	/** Стандартное значение поля обзора камеры */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Combat", meta=(AllowPrivateAccess = "true"))
	float CameraDefaultFOV;

	/** Поле обзора при приближении */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Combat", meta=(AllowPrivateAccess = "true"))
	float CameraZoomedFOV;

	/** Текущее значение поля обзора  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Combat", meta=(AllowPrivateAccess = "true"))
	float CameraCurrentFOV;

	/** Скорость интерполяции при приближении (прицеливании) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Combat", meta=(AllowPrivateAccess = "true"))
	float ZoomInterpSpeed;

	/** Множитель расширения перекрестия прицела */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Crosshair", meta=(AllowPrivateAccess = "true"))
	float CrosshairSpredMultiplier;

	/** Фактор расширения перекрестия прицела от скорости */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Crosshair", meta=(AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor;

	/**  Фактор расширения перекрестия прицела, если персонаж в воздухе */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Crosshair", meta=(AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;

	/** Фактор расширения перекрестия прицела в режиме прицеливания */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Crosshair", meta=(AllowPrivateAccess = "true"))
	float CrosshairAimFactor;

	/** Фактор расширения перекрестия прицела при стрельбе */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Crosshair", meta=(AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;

	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShotTimer;

	/** Кнопка стрельбы нажата */
	bool bFireButtonPressed;

	/** Должен ли игрок стрелять */
	bool bShouldFire;

	/** Темп автоматической стрельбы */
	float AutomaticFireRate;

	/** Таймер для автоматической стрельбы */
	FTimerHandle AutoFireTimer;
	
	//==================================================================================================================
	//				PRIVATE FUNCTIONS
	//==================================================================================================================	

	/**
 * Движение вперед / назад 
 * @param Value 1.0f - вперед, -1,0f - назад
 */
	void MoveForward(float Value);

	/**
	 * Движение вправо / влево
	 * @param Value 1.0f - вправо, -1.0f - влево 
	 */
	void MoveRight(float Value);

	/**
	 * Вызывается посредством ввода для вращения вправо/влево с заданной скоростью.
	 * Рассчитывает угол поворота для каждого кадра.
	 * Применимо для джойстика и клавиш курсора.
	 * @param Rate Нормализованное значение скорости, т.е. 1.0f - 100% желаемой скорости вращения
	 */
	void TurnAtRate(float Rate);

	/**
	 * Вызывается посредством ввода для вращения вверх/вниз с заданной скоростью.
	 * Рассчитывает угол поворота для каждого кадра.
	 * Применимо для джойстика и клавиш курсора.
	 * @param Rate Нормализованное значение скорости, т.е. 1.0f - 100% желаемой скорости вращения
	 */
	void LookUpAtRate(float Rate);

	/**
	 * Вращение контроллера основанное от движения по  Х оси мыши
	 * @param Value Значение ввода от Х оси мыши
	 */
	void Turn(float Value);

	/**
	 * Вращение контроллера основанное от движения по  Y оси мыши
	 * @param Value Значение ввода от Y оси мыши
	 */
	void LookUp(float Value);
	
	/**
	 * Получение конечной точки дымного следа от выстрела
	 * @param MuzzleSocketLocation сокет на конце ствола
	 * @param OutBeamEndLocation ссылка на конечную точку дымного следа от выстрела
	 * @return true - было попадание
	 */
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamEndLocation) const;
	
	/**
	 * Стрельба из оружия 
	 */
	void FireWeapon();

	/**
	 * Устанавливает bAiming true
	 */
	void AimingButtonPressed();

	/**
	 * Устанавливает bAiming false
	 */
	void AimingButtonReleased();

	/**
	 * Интерполяция при приближении взгляда в начале и в конце прицеливания
	 * @param DeltaTime 
	 */
	void CameraInterpZoom(float DeltaTime);

	/**
	 * Установка скорости вращения при начале/окончании прицеливания
	 */
	void SetLookRates();

	/**
	 * Расчёт размера перекрестия прицела при различных факторах
	 * @param DeltaTime Время для интерполяции восстановления прицела
	 */
	void CalculateCrosshairSpread(float DeltaTime);

	/**
	 * Функция для запуска таймера влияющего на разброс прицела при стрельбе
	 */
	void StartCrosshairBulletFire();
	
	/**
	 * Функция вызывается по завершении таймера для разброса прицела при стрельбе 
	 */
	UFUNCTION()
	void FinishCrosshairBulletFire();

	//	Автоматическая стрельба
	
	/**
	 * Кнопка стрельбы нажата
	 */
	void FireButtonPressed();

	/**
	 * Кнопка стрельбы отпущена
	 */
	void FireButtonReleased();

	/**
	 * Запускает таймер 
	 */
	void StartFireTimer();

	/**
	 * Обратный вызов по завершении таймера для автоматической стрельбы
	 */ 
	UFUNCTION()
	void AutoFireReset();

	//-------------------------------------------------------
};
