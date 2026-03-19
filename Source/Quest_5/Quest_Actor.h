// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest_Actor.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUEST_5_API UQuest_Actor : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuest_Actor();
	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	AActor* TargetCameraActor;

protected:
	virtual void BeginPlay() override;

public:
	void TriggerEvent();
	void Move();
	void Turn();
	void PrintMessage(const FString& Message);

private:
	FVector PreviousLocation;
	float TotalDistance;
	int32 Count;
	int32 TurnCount;

	FTimerHandle TimerHandle;
	int32 TurnDelay;

	FVector TargetLocation;
	bool bIsMoving;
	float MoveSpeed;
	FRotator TargetRotation;
	bool bIsTurning;
	float TurnSpeed;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
