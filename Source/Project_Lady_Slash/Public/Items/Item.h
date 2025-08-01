#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Item Sine Parameters", BlueprintReadWrite)
	float Amplitude = 2.5f;
	
	UPROPERTY(EditAnywhere, Category = "Item Sine Parameters", BlueprintReadWrite)
	float TimeConstant = 10.f;

	UFUNCTION(BlueprintPure)
	float TransformedSine();

	UFUNCTION(BlueprintPure)
	float TransformedCosine();

	UFUNCTION()
	void OnSphereBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UPROPERTY(VisibleInstanceOnly, Category = "Item Sine Parameters", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	double RunningTime = 0;

	UPROPERTY(VisibleAnywhere) // Details and Component Panels
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* OverlapSphere;
	

	
};