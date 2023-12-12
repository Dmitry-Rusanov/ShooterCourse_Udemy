// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UBoxComponent;

UCLASS()
class SHOOTERCOURSE_UDEMY_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	//==================================================================================================================
	//				PRIVATE PROPERTIES & FIELDS
	//==================================================================================================================
	
	/** Скелетный меш оружия */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | ItemProperties", meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemMesh;
	
	/** Бокс коллизий оружия для линейной трассировки, чтобы показывать виджеты HUD */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | ItemProperties", meta=(AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;


	//==================================================================================================================
	//				PRIVATE FUNCTIONS
	//==================================================================================================================


	
public:

	//==================================================================================================================
	//				PUBLIC PROPERTIES & FIELDS
	//==================================================================================================================

	//==================================================================================================================
	//				PUBLIC FUNCTIONS
	//==================================================================================================================
	
};
