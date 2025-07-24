#include "Pawns/Crow.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"



ACrow::ACrow()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CrowCapsuleComponent"));
	Capsule->SetCapsuleHalfHeight(22.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CrowSkeletalMesh"));
	Mesh->SetupAttachment(Capsule);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CrowSpringArm"));
	SpringArm->SetupAttachment(Capsule);
	SpringArm->TargetArmLength = 300.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CrowCamera"));
	Camera->SetupAttachment(SpringArm);
	

}

void ACrow::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrow::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

