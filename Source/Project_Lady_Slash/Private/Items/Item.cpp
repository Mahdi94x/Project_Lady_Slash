#include "Items/Item.h"
#include "Project_Lady_Slash/DebugMacros.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	/*SetActorLocation(FVector(0.f, 0.f, 50.f));
	SetActorRotation(FRotator(0.f, 45.f, 0.f));*/

	/*FVector StartLocation = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector EndLocation = StartLocation + Forward * 100;
	DRAW_SPHERE(StartLocation);
	DRAW_VECTOR(StartLocation, EndLocation);*/
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant); // period = 2*pi/k. k is TimeConstant
	AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));

	/*MovementRate cm/s * DeltaTime (s/frame) => (cm/frame) framerate independent
	float MovementRate = 50.f;
	float RotationRate = 45.f;
	AddActorWorldOffset(FVector(MovementRate*DeltaTime, 0.f, 0.f));
	AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));*/

	FVector StartLocation = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector EndLocation = StartLocation + Forward * 100;
	DRAW_SPHERE_SingleFrame(StartLocation);
	DRAW_VECTOR_SingleFrame(StartLocation, EndLocation);
}

