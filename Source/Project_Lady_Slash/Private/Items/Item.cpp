#include "Items/Item.h"

AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it (Boolean).
	PrimaryActorTick.bCanEverTick = true;

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay() Called From Item.cpp"));
	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

