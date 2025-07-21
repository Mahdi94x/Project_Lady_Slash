#include "Items/Item.h"
#include "Project_Lady_Slash/DebugMacros.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	FVector StartLocation = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector EndLocation = StartLocation + Forward * 100;
	DRAW_SPHERE(StartLocation);
	DRAW_VECTOR(StartLocation, EndLocation);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

