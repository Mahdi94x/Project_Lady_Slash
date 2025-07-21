#include "Items/Item.h"

AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it (Boolean).
	PrimaryActorTick.bCanEverTick = true;

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay() Called From Item.cpp Visual Studio!"));

	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(
			1,               // Key
			60.f,            // Display time in seconds
			FColor::Cyan,    // Color
			FString(TEXT("Item On Screen Message Using GEngine!")) // Message
		);
	}*/
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime);*/
	//FString Message = FString::Printf(TEXT("DeltaTime: %f"), DeltaTime);

	/*FString Name = GetName();
	FString Message = FString::Printf(TEXT("Item Name: %s"), *Name);*/
	/*UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Name);*/

	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(
			0,               // Key
			60.f,            // Display time in seconds
			FColor::Cyan,    // Color
			Message // Message
		);
	}*/
}

