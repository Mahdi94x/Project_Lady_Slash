#include "Items/Item.h"
#include "Project_Lady_Slash/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickUpInterface.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(GetRootComponent());
	OverlapSphere->SetSphereRadius(300.f);
	OverlapSphere->bHiddenInGame = true;

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ItemNiagaraComponent"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

}

float AItem::TransformedSine()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCosine()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		PickUpInterface->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		PickUpInterface->SetOverlappingItem(nullptr);
	}
}

void AItem::SpawnPickupEffect()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			this->GetActorLocation()
		);
	}
}

void AItem::PlayPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			PickupSound,
			this->GetActorLocation()
		);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	if (ItemCurrentState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSine()));
		AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));
	}
}

