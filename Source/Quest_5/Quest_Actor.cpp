// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest_Actor.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"

UQuest_Actor::UQuest_Actor()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsMoving = false;
	bIsTurning = false;
	MoveSpeed = 4.0f;
	TurnSpeed = 2.0f;
	TotalDistance = 0.0f;
	Count = 0;
	TurnCount = 0;
}

void UQuest_Actor::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		FVector StartLocation(0.f, 50.f, 0.f);
		Owner->SetActorLocation(StartLocation);
		PreviousLocation = StartLocation;

		PrintMessage(TEXT("---초기 위치 설정 완료! 초기 위치는 {0,50,0} 입니다.---"));
		PrintMessage(TEXT("---이동 시작---"));

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UQuest_Actor::TriggerEvent, 1.0f, true);
	}
}


void UQuest_Actor::TriggerEvent()
{
	if (Count < 10)
	{
		Move();
		if (FMath::RandRange(0, 1) == 0)
		{
			Turn();
		}
	}
	if (Count == 10)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		PrintMessage(TEXT("---이동 완료!---"));
		PrintMessage(FString::Printf(TEXT("---이동 횟수 : %d"), Count));
		PrintMessage(FString::Printf(TEXT("---회전한 횟수 : %d"), TurnCount));
		PrintMessage(FString::Printf(TEXT("---총 이동 거리: %0.1f"), TotalDistance));
	}
}


void UQuest_Actor::Move()
{
	AActor* Owner = GetOwner();
	float RandomDist = FMath::RandRange(50.0f, 300.0f);

	FVector TargetDirection = Owner->GetActorForwardVector();

	TargetLocation = Owner->GetActorLocation() + (TargetDirection * RandomDist);

	TotalDistance += RandomDist;

	bIsMoving = true;
	Count++;

	PrintMessage(FString::Printf(TEXT("---이동 횟수 : %d "), Count));
	PrintMessage(FString::Printf(TEXT("---이동 거리 : %0.1f"), RandomDist));
}

void UQuest_Actor::Turn()
{
	PrintMessage(TEXT("50%의 확률로 회전 이벤트가 발동되었습니다!"));\
	TurnCount++;

	float RandomAngle = FMath::RandRange(-130.0f, 130.0f);
	TargetRotation = FRotator(0.0f, RandomAngle, 0.0f);
	bIsTurning = true;
}

void UQuest_Actor::PrintMessage(const FString& Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green, Message);
	}
}

void UQuest_Actor::TickComponent(float DeltaTime, ELevelTick TickType,
                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AActor* Owner = GetOwner();

	if (bIsMoving)
	{
		if (Owner)
		{
			FVector CurrentLocation = Owner->GetActorLocation();

			FVector InterpLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);

			Owner->SetActorLocation(InterpLocation);

			if (FVector::Dist(InterpLocation, TargetLocation) < 1.0f)
			{
				Owner->SetActorLocation(TargetLocation);
				PreviousLocation = TargetLocation;
				bIsMoving = false;
			}
		}
	}
	if (bIsTurning)
	{
		if (Owner)
		{
			FRotator CurrentRotation = Owner->GetActorRotation();

			FRotator InterpRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, TurnSpeed);

			Owner->SetActorRotation(InterpRotation);

			if (CurrentRotation.Equals(TargetRotation, 0.1f))
			{
				Owner->SetActorRotation(TargetRotation);
				bIsTurning = false;
			}
		}
	}
}
